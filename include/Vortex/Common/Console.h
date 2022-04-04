#pragma once
#include <vector>
#include <shared_mutex>

#include "Vortex/Common/Singleton.h"

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
		explicit Console();
		~Console();

	public:
		void Write(EntryType::Enum type, const char* log);

		template<typename ...Args>
		inline void Write(EntryType::Enum type, const char* format, Args ... args) {
			char buffer[512];
			sprintf_s(buffer, 512, format, args...);
			Write(type, buffer);
		}

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
		std::shared_lock<std::shared_mutex> GetSharedLock() {
			return std::shared_lock<std::shared_mutex>{m_Mutex};
		}
		inline SizeType Size() const { return m_Entries.size(); }
		inline const Console::Entry& Get(SizeType i) const { return m_Entries[i]; }

	public:
		inline SizeType GetTotalEntryCount() const { return m_TotalEntryCount; }

	protected:
		static Console* s_Instance;

	protected:
		ContainerType m_Entries;
		SizeType m_TotalEntryCount;
		mutable std::shared_mutex m_Mutex;
	};
}