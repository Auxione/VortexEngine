#ifdef VORTEX_DEBUG
#include "Vortex/Debug/Profiler.h"

namespace Vortex::DebugProfiler {
	MapType ProfileMap;
	MapType FrameProfileMap;
	std::chrono::steady_clock::time_point ProfileStartTime;
	std::chrono::steady_clock::time_point FrameStartTime;

	bool FrameCall{false};
	std::size_t FrameCount{0};
	DurationType TotalFrameTime{0};

	ProfilerResultData::ProfilerResultData(NameType name,
										   DurationType total_time,
										   DurationType max_time,
										   DurationType average_time,
										   CallCountType call_count,
										   double utilization):
		Name{std::move(name)},
		TotalTime{total_time},
		MaxTime{max_time},
		AverageTime{average_time},
		CallCount{call_count},
		Utilization{utilization} {}

	void BeginProfile() {
		ProfileMap.clear();
		ProfileStartTime = std::chrono::steady_clock::now();
	}

	ProfilerResult EndProfile() {
		auto profile_duration = std::chrono::steady_clock::now() - ProfileStartTime;

		ProfilerResult results;
		results.TotalTime = std::chrono::duration_cast<DurationType>(profile_duration);
		results.TotalFrameTime = TotalFrameTime;
		results.FrameCount = FrameCount;

		results.Data.reserve(ProfileMap.size());
		for (const auto& data : ProfileMap) {
			const auto& name = data.first;
			const auto& profile_data = data.second;

			auto average_time = profile_data.TotalTime / profile_data.CallCount;
			auto utilization = static_cast<double>(profile_data.TotalTime.count()) / static_cast<double>(results.TotalTime.count());

			results.Data.emplace_back(
				name,
				profile_data.TotalTime,
				profile_data.MaxTime,
				average_time,
				profile_data.CallCount,
				utilization
			);
		}

		results.FrameData.reserve(FrameProfileMap.size());
		for (const auto& data : FrameProfileMap) {
			const auto& name = data.first;
			const auto& profile_data = data.second;

			auto average_time = profile_data.TotalTime / profile_data.CallCount;
			auto utilization = static_cast<double>(profile_data.TotalTime.count()) / static_cast<double>(results.TotalTime.count());

			results.FrameData.emplace_back(
				name,
				profile_data.TotalTime,
				profile_data.MaxTime,
				average_time,
				profile_data.CallCount,
				utilization
			);
		}
		return results;
	}

	void BeginFrame() {
		FrameCall = true;
		FrameStartTime = std::chrono::steady_clock::now();
	}

	void EndFrame() {
		TotalFrameTime += std::chrono::duration_cast<DebugProfiler::DurationType>(std::chrono::steady_clock::now() - FrameStartTime);;
		FrameCall = false;
		++FrameCount;
	}

	void RawProfileData::AddTime(const DurationType& elapsed) {
		if (MaxTime < elapsed) {
			MaxTime = elapsed;
		}
		TotalTime += elapsed;
		++CallCount;
	}

	void WriteResultsSorted(std::ostream& ostream, const std::vector<ProfilerResultData>& result, std::size_t count) {
		char line_buffer[1024];

		std::vector<ProfilerResultData> sorted_data{result};
		count = count > sorted_data.size() ? sorted_data.size() : count;

		sprintf_s(line_buffer, "%-2s : %-16s - %-16s\n", "#", "AverageTime (ms)", "Method Name");
		ostream << "\t" << line_buffer;
		std::sort(sorted_data.begin(), sorted_data.end(), [](const ProfilerResultData& a, const ProfilerResultData& b) {
			return a.AverageTime > b.AverageTime;
		});
		for (std::size_t i = 0; i < count; ++i) {
			sprintf_s(line_buffer, "%-2zu : %-16f - %-16s\n", i + 1, sorted_data[i].AverageTime.count(), sorted_data[i].Name.c_str());
			ostream << "\t" << line_buffer;
		}

		ostream << "\n";
		sprintf_s(line_buffer, "%-2s : %-16s - %-16s\n", "#", "MaxTime (ms)", "Method Name");
		ostream << "\t" << line_buffer;
		std::sort(sorted_data.begin(), sorted_data.end(), [](const ProfilerResultData& a, const ProfilerResultData& b) {
			return a.MaxTime > b.MaxTime;
		});
		for (std::size_t i = 0; i < count; ++i) {
			sprintf_s(line_buffer, "%-2zu : %-16f - %-16s\n", i + 1, sorted_data[i].MaxTime.count(), sorted_data[i].Name.c_str());
			ostream << "\t" << line_buffer;
		}

		ostream << "\n";
		sprintf_s(line_buffer, "%-2s : %-16s - %-16s\n", "#", "Utilization (%)", "Method Name");
		ostream << "\t" << line_buffer;
		std::sort(sorted_data.begin(), sorted_data.end(), [](const ProfilerResultData& a, const ProfilerResultData& b) {
			return a.Utilization > b.Utilization;
		});
		for (std::size_t i = 0; i < count; ++i) {
			sprintf_s(line_buffer, "%-2zu : %-16.3f - %-16s\n", i + 1, sorted_data[i].Utilization * 100, sorted_data[i].Name.c_str());
			ostream << "\t" << line_buffer;
		}
	}

	void WriteExtendedTable(std::ostream& ostream, const std::vector<ProfilerResultData>& result) {
		std::vector<ProfilerResultData> sorted_data{result};
		std::sort(sorted_data.begin(), sorted_data.end(), [](const ProfilerResultData& a, const ProfilerResultData& b) {
			return a.CallCount > b.CallCount;
		});

		char line_buffer[1024];
		sprintf_s(line_buffer, "%-16s | %-16s | %-16s | %-16s | %-16s\n",
				  "Call Count",
				  "Utilization (%)",
				  "AverageTime (ms)",
				  "MaximumTime (ms)",
				  "Method Name");
		ostream << "\t" << line_buffer;

		sprintf_s(line_buffer, "%-16s | %-16s | %-16s | %-16s | %-16s\n",
				  "----------------",
				  "----------------",
				  "----------------",
				  "----------------",
				  "----------------");
		ostream << "\t" << line_buffer;

		for (auto& i : sorted_data) {
			sprintf_s(line_buffer, "%-16llu | %-16.3f | %-16f | %-16f | %s\n",
					  i.CallCount,
					  i.Utilization * 100,
					  i.AverageTime.count(),
					  i.MaxTime.count(),
					  i.Name.c_str()
			);
			ostream << "\t" << line_buffer;
		}
	}

	void WriteToStream(std::ostream& ostream, const ProfilerResult& result) {
		auto finish_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

		auto& app_data = result.Data;
		auto& app_time = result.TotalTime;

		auto& frame_data = result.FrameData;
		auto& frame_time = result.TotalFrameTime;
		auto& frame_count = result.FrameCount;

		std::vector<ProfilerResultData> sorted_data{result.Data};

		ostream << "Vortex Profiler\n";
		ostream << "\t" << "Creation Time    : " << std::put_time(std::localtime(&finish_time), "%Y-%m-%d %X") << "\n";
		ostream << "\t" << "Application Time : " << app_time.count() / 1000 << " seconds\n";
		ostream << "\t" << "Frame Time       : " << frame_time.count() / 1000 << " seconds\n";
		ostream << "\t" << "Frame Count      : " << frame_count << "\n";
		ostream << "\t" << "Average FPS      : " << static_cast<double>(frame_count) / (frame_time.count() / 1000) << "\n";

		ostream << "\n";
		ostream << "Frame Profiling Data:\n";
		WriteResultsSorted(ostream, result.FrameData, 5);
		ostream << "\n";
		WriteExtendedTable(ostream, result.FrameData);

		ostream << "\n";
		ostream << "Application Profiling Data:\n";
		WriteResultsSorted(ostream, result.Data, 5);
		ostream << "\n";
		WriteExtendedTable(ostream, result.Data);

	}
	void WriteToFile(const std::filesystem::path& path, const ProfilerResult& result) {
		std::ofstream file{path};
		WriteToStream(file, result);
	}
}

namespace Vortex {
	UniqueProfiler::UniqueProfiler(DebugProfiler::KeyType name):
		m_Name(std::move(name)),
		m_Start(std::chrono::steady_clock::now()) {}

	UniqueProfiler::~UniqueProfiler() {
		auto end_time = std::chrono::steady_clock::now();
		auto duration = std::chrono::duration_cast<DebugProfiler::DurationType>(end_time - m_Start);

		if (DebugProfiler::FrameCall) {
			DebugProfiler::FrameProfileMap[m_Name].AddTime(duration);
		} else {
			DebugProfiler::ProfileMap[m_Name].AddTime(duration);
		}
	}
}

#endif