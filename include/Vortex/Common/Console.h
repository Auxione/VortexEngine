#pragma once
#include <filesystem>
#include <vector>
#include <shared_mutex>

#include "Vortex/Common/ThreadSafeRW.h"

namespace Vortex {
	namespace EntryType {
		enum Enum {
			Debug = 0,
			Info,
			Warning,
			Error
		};

		constexpr static const char* ToString[]{
			" Debug "
			, " Info  "
			, "Warning"
			, " Error "
		};
	}

	class Console {
	public:
		struct Entry {
			Vortex::String Log;
			EntryType::Enum Type;
		};

		using ContainerType = std::vector<Console::Entry>;

	public:
		static void Initialize() {
			VORTEX_ASSERT(s_Instance == nullptr)
			s_Instance = new Console();
		}
		static Console& Get() {
			VORTEX_ASSERT(s_Instance != nullptr)
			return *s_Instance;
		}
		static void Shutdown() {
			VORTEX_ASSERT(s_Instance != nullptr)
			delete s_Instance;
		}

	public:
		explicit Console(std::filesystem::path file_path = "log.txt", SizeType max_value = 20);
		~Console();

	public:
		inline void SetLogFilePath(const std::filesystem::path& file_path) {
			std::unique_lock write_lock{m_Mutex};
			m_LogFilePath = file_path;
		}
		inline void SetLogFileBufferEntryCount(SizeType max_value) {
			std::unique_lock write_lock{m_Mutex};
			m_LogFileBufferEntryCount = max_value;
		}
		void Flush();

	public:
		void Write(EntryType::Enum type, const char* log);

		template<typename ...Args>
		inline void Write(EntryType::Enum type, const char* format, Args ... args) {
			char buffer[512];
			sprintf_s(buffer, 512, format, args...);
			Write(type, buffer);
		}

		ThreadSafeReader<ContainerType> GetEntries() const { return {m_Mutex, &m_Entries}; }

	public:
		template<typename ...Args>
		inline static void WriteDebug(const char* format, Args ... args) {
			Get().Write<Args...>(EntryType::Debug, format, args...);
		}
		template<typename ...Args>
		inline static void WriteInfo(const char* format, Args ... args) {
			Get().Write<Args...>(EntryType::Info, format, args...);
		}
		template<typename ...Args>
		inline static void WriteWarning(const char* format, Args ... args) {
			Get().Write<Args...>(EntryType::Warning, format, args...);
		}
		template<typename ...Args>
		inline static void WriteError(const char* format, Args ... args) {
			Get().Write<Args...>(EntryType::Error, format, args...);
		}

	public:
		inline SizeType GetTotalEntryCount() const { return m_TotalEntryCount; }

	protected:
		static Console* s_Instance;

	protected:
		std::filesystem::path m_LogFilePath;
		SizeType m_LogFileBufferEntryCount;
		ContainerType m_LogFileBuffer;

		ContainerType m_Entries;

		SizeType m_TotalEntryCount;
		mutable std::shared_mutex m_Mutex;
	};
}