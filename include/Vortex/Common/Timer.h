#pragma once
#include <chrono>

namespace Vortex::TimerTraits {
	using ClockType = std::chrono::steady_clock;
	using Timepoint = ClockType::time_point;

	template<typename T>
	using Seconds = std::chrono::duration<T, std::ratio<1, 1>>;

	template<typename T>
	using Milliseconds = std::chrono::duration<T, std::ratio<1, 1000>>;

	using TimeNowFn = Timepoint (*)();

	constexpr static TimeNowFn TimeNow = []() -> Timepoint {
		return TimerTraits::ClockType::now();
	};
}

namespace Vortex {
	template<typename T, typename DurationType = TimerTraits::Seconds<T>>
	class Timer {
	public:
		Timer(): m_Begin{}, m_Time{} {}
		~Timer() = default;

	public:
		void Start() {
			m_Begin = TimerTraits::TimeNow();
		}
		void Stop() {
			auto duration = TimerTraits::TimeNow() - m_Begin;
			m_Time = std::chrono::duration_cast<DurationType>(duration);
		}

		T Get() const { return m_Time.count(); }
		operator T() const { return m_Time.count(); }

	private:
		TimerTraits::Timepoint m_Begin;
		DurationType m_Time;
	};

	template<typename T, typename DurationType= TimerTraits::Seconds<T>>
	class RAIITimer {
	public:
		explicit RAIITimer(T* data_ptr)
			: m_Begin{TimerTraits::TimeNow()}, m_DataPtr{data_ptr} {}
		~RAIITimer() {
			auto duration = TimerTraits::TimeNow() - m_Begin;
			*m_DataPtr = std::chrono::duration_cast<DurationType>(duration).count();
		}

	private:
		TimerTraits::Timepoint m_Begin;
		T* m_DataPtr;
	};
}