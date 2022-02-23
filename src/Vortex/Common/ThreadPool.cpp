#include "Vortex/Common/ThreadPool.h"
#include "Vortex/Common/Logger.h"

namespace Vortex {
	ThreadPool::ThreadPool(SizeType thread_count)
		: m_Threads{thread_count},
		  m_Running{true} {

		auto thread_job = [this]() {
			ThreadPoolJob* thread_job;

			while (true) {
				std::unique_lock<std::mutex> lock(m_Mutex);
				m_StateSignal.wait(lock, [this] { return StateWaitFn(); });
				if (!m_Running) { break; }

				thread_job = m_JobQueue.Front();
				m_JobQueue.Pop();
				lock.unlock();

				++m_ActiveJobCount;
				try {
					thread_job->Execute();
				} catch (const std::exception& e) {
					Vortex::Logger::Log(Logger::LogType::Error, "Exception raised when executing job.\n%s\n", e.what());
				}
				GetHeapAllocator()->Delete(thread_job);
				--m_ActiveJobCount;

				m_JobCompleteSignal.notify_all();
			}

		};

		for (SizeType i = 0; i < m_Threads.Capacity(); ++i) {
			m_Threads.Emplace(i, thread_job);
		}
	}

	ThreadPool::~ThreadPool() {
		Join();
	}

	ThreadPool& ThreadPool::Await() {
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_JobCompleteSignal.wait(lock, [this] { return ThreadWaitFn(); });
		return *this;
	}

	ThreadPool& ThreadPool::Dispatch() {
		std::unique_lock<std::mutex> lock(m_Mutex);
		if (m_JobQueue.Empty()) { return *this; }

		m_StateSignal.notify_all();
		return *this;
	}

	void ThreadPool::Join() {
		m_Running = false;
		m_StateSignal.notify_all();

		for (auto& thread : m_Threads) {
			thread.join();
		}

		m_JobQueue.Clear();
		m_Threads.Clear();
	}

	void ThreadPool::Clear() {
		std::unique_lock<std::mutex> lock{m_Mutex};
		while (!m_JobQueue.Empty()) {
			delete m_JobQueue.Front();
			m_JobQueue.Pop();
		}
	}

	void ThreadPool::SplitForEach(SizeType array_count, SizeType& chunk_size, SizeType& job_count) {
		auto chunk_count = m_Threads.Size() > array_count ? array_count : m_Threads.Size(); //min(m_Threads.Size(),array_count)
		chunk_size = array_count / chunk_count;
		auto remaining_chunk_size = array_count % chunk_count;
		auto remaining_chunk_count = remaining_chunk_size / chunk_size;
		job_count = chunk_count + remaining_chunk_count;
	}
}