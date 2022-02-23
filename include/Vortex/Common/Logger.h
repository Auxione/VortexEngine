#pragma once
#include <cstdio>
#include <string>
#include <filesystem>

#include "Vortex/Debug/Assert.h"

namespace Vortex::Logger {
	namespace LogType {
		enum Enum {
			Debug = 0,
			Info,
			Error,
			Warning
		};

		constexpr static const char* ToString[]{
			" Debug "
			, " Info  "
			, " Error "
			, "Warning"
		};
	};

	constexpr static SizeType BufferSize = 1024;

	template<typename ...Args>
	inline void Log(LogType::Enum log_type, const char* format, Args&& ... args) {
		char buffer[BufferSize];

		int pos = 0;
		pos += sprintf_s(buffer + pos, BufferSize - pos, "[%-7s] ", LogType::ToString[log_type]);
		//pos += sprintf(buffer + pos,  "[%-7s] ", LogType::ToString[log_type]);

		sprintf_s(buffer + pos, 1024 - pos, format, args...);
		//sprintf(buffer + pos, format, args...);

		if (log_type == LogType::Error) {
			fprintf(stderr, "%s\n", buffer);
		} else {
			fprintf(stdout, "%s\n", buffer);
		}
	}

#ifdef VORTEX_DEBUG
#define VORTEX_LOG_DEBUG(fmt, ...) Vortex::Logger::Log(Vortex::Logger::LogType::Debug, fmt, ##__VA_ARGS__);
#else
#define VORTEX_LOG_DEBUG(fmt, ...);
#endif

#define VORTEX_LOG_INFO(fmt, ...) Vortex::Logger::Log(Vortex::Logger::LogType::Info, fmt, ##__VA_ARGS__)
#define VORTEX_LOG_WARNING(fmt, ...) Vortex::Logger::Log(Vortex::Logger::LogType::Warning, fmt, ##__VA_ARGS__)
#define VORTEX_LOG_ERROR(fmt, ...) Vortex::Logger::Log(Vortex::Logger::LogType::Error, fmt, ##__VA_ARGS__)
}


