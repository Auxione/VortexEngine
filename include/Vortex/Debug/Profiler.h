//	Profiler.h - Vortex Engine
//
//	Created on 20.10.2021.
//
//	Include this file in whatever places need to refer to it.
//	if the selected build mode is debug, the provided CMakeLists.txt will automatically
//	define VORTEX_ENABLE_PROFILER and VORTEX_PROJECT_NAME.
//
//	The data storage and other helper functions is in the Vortex::DebugProfiler namespace.
//	Most of the macros use UniqueProfiler to gather function/scope info. UniqueProfiler can be found on Vortex namespace.
//
//	=============== Example Code ===============
//
//	#include <thread> // for this_thread and sleep_for
//
//	#define  VORTEX_DEBUG_PROFILER_IMPLEMENTATION
//	#include "Vortex/Debug/DebugProfiler.h"
//
//	void MyFunction() {
//		VORTEX_DEBUG_PROFILER_FUNCTION
//		/* ... */
//	}
//
//	void MyOtherFunction() {
//		VORTEX_DEBUG_PROFILER_FUNCTION
//		std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Dummy workload.
//		/* ... */
//	}
//
//	int main() {
//		//Begin profiling
//		VORTEX_DEBUG_PROFILER_BEGIN
//		MyFunction();
//
//		{
//			VORTEX_DEBUG_PROFILER_SCOPE("ForLoop")
//			for (int i = 0; i < 16; ++i) {
//				MyOtherFunction();
//				std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Another dummy workload.
//			}
//		}
//
//		VORTEX_DEBUG_PROFILER_END // This macro will save the profile results into a .txt file with project name on binary directory.
//		//VORTEX_DEBUG_PROFILER_END_TOSTREAM(std::cout) // Alternatively profiler can write output to stream
//		//VORTEX_DEBUG_PROFILER_END_TOFILE("file_name") // or files.
//	}
//
//	=============== Example Code ===============

#pragma once
#ifdef VORTEX_DEBUG

#include <unordered_map>
#include <chrono>
#include <string>
#include <numeric>
#include <filesystem>
#include <fstream>
#include <cstdio>
#include <utility>

namespace Vortex::DebugProfiler {
	struct RawProfileData;
	using KeyType = std::string;
	using NameType = std::string;
	using MapType = std::unordered_map<KeyType, RawProfileData>;

	using CallCountType = std::uint64_t;
	using DurationType = std::chrono::duration<double, std::ratio<1, 1000>>;

	struct RawProfileData {
		DurationType TotalTime;
		DurationType MaxTime;
		CallCountType CallCount;

		void AddTime(const DurationType& elapsed);
	};

	struct ProfilerResultData {
		NameType Name;
		DurationType TotalTime;
		DurationType MaxTime;
		DurationType AverageTime;
		CallCountType CallCount;

		double Utilization;

		ProfilerResultData(NameType name,
						   DurationType total_time,
						   DurationType max_time,
						   DurationType average_time,
						   CallCountType call_count,
						   double utilization);
	};

	struct ProfilerResult {
		std::vector<ProfilerResultData> Data;
		std::vector<ProfilerResultData> FrameData;
		DurationType TotalTime;
		DurationType TotalFrameTime;
		std::size_t FrameCount;
	};

	void BeginProfile();
	ProfilerResult EndProfile();

	void BeginFrame();
	void EndFrame();

	void WriteToStream(std::ostream& ostream, const ProfilerResult& result);
	void WriteToFile(const std::filesystem::path& path, const ProfilerResult& result);
}
namespace Vortex {
	class UniqueProfiler {
	private:
		DebugProfiler::KeyType m_Name;
		std::chrono::steady_clock::time_point m_Start;

	public:
		explicit UniqueProfiler(DebugProfiler::KeyType name);
		~UniqueProfiler();
	};
}

#define VORTEX_DEBUG_PROFILER_BEGIN Vortex::DebugProfiler::BeginProfile();

#define VORTEX_DEBUG_PROFILER_FUNCTION Vortex::UniqueProfiler dpf(__FUNCTION__);
#define VORTEX_DEBUG_PROFILER_SCOPE(var) Vortex::UniqueProfiler dps(var);
#define VORTEX_DEBUG_PROFILER_FRAME_BEGIN Vortex::DebugProfiler::BeginFrame();
#define VORTEX_DEBUG_PROFILER_FRAME_END Vortex::DebugProfiler::EndFrame();

#define VORTEX_DEBUG_PROFILER_END_TOFILE(path)\
auto profile = Vortex::DebugProfiler::EndProfile();\
Vortex::DebugProfiler::WriteToFile(path,profile);

#define VORTEX_DEBUG_PROFILER_END_TOSTREAM(ostream)\
auto profile = Vortex::DebugProfiler::EndProfile();\
Vortex::DebugProfiler::WriteToStream(ostream,profile);

#define VORTEX_DEBUG_PROFILER_END \
std::string file_name{"Profiler_Output.txt"}; \
VORTEX_DEBUG_PROFILER_END_TOFILE(std::filesystem::path{file_name})

#else

#define VORTEX_DEBUG_PROFILER_BEGIN

#define VORTEX_DEBUG_PROFILER_FUNCTION
#define VORTEX_DEBUG_PROFILER_SCOPE(var)
#define VORTEX_DEBUG_PROFILER_FRAME_BEGIN
#define VORTEX_DEBUG_PROFILER_FRAME_END

#define VORTEX_DEBUG_PROFILER_END_TOFILE(path)
#define VORTEX_DEBUG_PROFILER_END_TOSTREAM(ostream)
#define VORTEX_DEBUG_PROFILER_END

#endif