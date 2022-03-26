#pragma once
#include <thread>
#include <future>
#include <condition_variable>
#include <utility>
#include <vector>
#include <queue>

namespace Vortex {
	struct ThreadPoolJob {
		virtual ~ThreadPoolJob() = default;
		virtual void Execute() = 0;
	};
}

namespace Vortex {
	class ThreadPool {
	public:
		template<typename T>
		using ParallelJobFn = std::function<void(SizeType, T & )>;

		using SingleTaskJobFn = std::function<void()>;

	protected:
		template<typename T>
		struct ParallelForJob: public ThreadPoolJob {
			T* m_ArrayPtr;
			SizeType m_Index;
			SizeType m_Count;
			ParallelJobFn<T> m_Job;

			ParallelForJob(T* array_ptr, SizeType index, SizeType count, ParallelJobFn<T> job)
				: m_ArrayPtr{array_ptr}, m_Index{index}, m_Count{count}, m_Job{std::move(job)} {
			}

			void Execute() override {
				for (SizeType i = 0; i < m_Count; ++i) {
					auto index = m_Index + i;
					m_Job(index, m_ArrayPtr[index]);
				}
			}
		};

		struct SingleTaskJob: public ThreadPoolJob {
			SingleTaskJobFn m_Job;
			SingleTaskJob(SingleTaskJobFn job): m_Job{std::move(job)} {}
			void Execute() override { m_Job(); }
		};

	public:
		ThreadPool(SizeType thread_count = std::thread::hardware_concurrency());
		virtual ~ThreadPool();

		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;

/*
	public:
		template<typename ... T>
		using ParallelMultiVectorJobFn = std::function<void(SizeType, T& ...)>;

	private:
		template<typename ... T>
		struct ParallelForMultiVectorJob: public ThreadPoolJob {
			MultiVector<T...>* m_VectorPtr;
			SizeType m_Index;
			ParallelMultiVectorJobFn<T...> m_Job;

			ParallelForMultiVectorJob(MultiVector<T...>* vector_ptr, SizeType index, ParallelMultiVectorJobFn<T...> job)
				: m_VectorPtr{vector_ptr}, m_Index{index}, m_Job{job} {
			}

			void Execute() override {
				m_Job(m_Index, m_VectorPtr->Get<T>(m_Index)...);
			}
		};
*/

	public:
		template<typename T, typename ... Args>
		inline ThreadPool& EmplaceJob(Args&& ... args) {
			std::unique_lock<std::mutex> lock{m_Mutex};
			m_JobQueue.emplace(new T(std::forward<Args...>(args)...));
			return *this;
		}

		template<typename T>
		inline ThreadPool& PushJob(T* job) {
			std::unique_lock<std::mutex> lock{m_Mutex};
			m_JobQueue.emplace(job);
			return *this;
		}

	public:
		inline ThreadPool& DoTask(const SingleTaskJobFn& job_fn) {
			return EmplaceJob<SingleTaskJob>(job_fn);
		}

		template<typename T>
		inline ThreadPool& Foreach(T* array, SizeType count, const ParallelJobFn<T>& foreach_fn) {
			SizeType job_count;
			SizeType chunk_size;
			SplitForEach(count, chunk_size, job_count);

			std::unique_lock<std::mutex> lock{m_Mutex};
			for (SizeType i = 0; i < job_count; ++i) {
				auto start_index = i * chunk_size;
				m_JobQueue.emplace(new ParallelForJob<T>(array, start_index, chunk_size, foreach_fn));
			}
			return *this;
		}

		template<typename T>
		inline ThreadPool& Foreach(std::vector<T>& vector, const ParallelJobFn<T>& foreach_fn) {
			return Foreach(vector.Data(), vector.Size(), foreach_fn);
		}
/*
		template<typename ...T>
		ThreadPool& ForeachMV(MultiVector<T...>* vector, const ParallelMultiVectorJobFn<T...>& foreach_fn) {
			std::unique_lock<std::mutex> lock{m_Mutex};
			for (SizeType i = 0; i < vector->Size(); ++i) {
				m_JobQueue.Emplace(new ParallelForMultiVectorJob<T...>(vector, i, foreach_fn));
			}
			return *this;
		}
*/
	public:
		ThreadPool& Await();
		ThreadPool& Dispatch();

	public:
		void Join();
		void Clear();

	protected:
		inline bool ThreadWaitFn() { return m_ActiveJobCount == 0 && m_JobQueue.empty(); }
		inline bool StateWaitFn() { return !m_Running || !m_JobQueue.empty(); }

		void SplitForEach(SizeType array_count, SizeType& chunk_size, SizeType& job_count);

	protected:
		std::vector<std::thread> m_Threads;
		std::queue<ThreadPoolJob*> m_JobQueue;

		std::mutex m_Mutex;

		std::condition_variable m_StateSignal;
		std::condition_variable m_JobCompleteSignal;

		std::atomic<bool> m_Running;
		std::atomic<SizeType> m_ActiveJobCount;
	};
}