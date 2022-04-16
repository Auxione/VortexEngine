#include <fstream>
#include <utility>

#include "Vortex/Common/Console.h"

namespace Vortex {
	Console* Console::s_Instance{nullptr};

	Console::Console(std::filesystem::path file_path, SizeType max_value)
		: m_LogFilePath{std::move(file_path)},
		  m_LogFileBufferEntryCount{max_value},
		  m_LogFileBuffer{},

		  m_Entries(),

		  m_TotalEntryCount{0},
		  m_Mutex(),

		  m_DebugOutput{false} {

#if VORTEX_DEBUG
		m_DebugOutput = true;
#endif

		std::ofstream file{m_LogFilePath};
		if (file.is_open()) {
			file << "\nVortex Engine Console Log\n\n";
		}
		printf("Vortex Engine Console Log");
	}
	Console::~Console() {
		Flush();
	}

	void Console::Flush() {
		std::shared_lock read_lock{m_Mutex};
		if (m_Entries.empty()) {
			return;
		}
		read_lock.unlock();

		std::ofstream file{m_LogFilePath, std::ios_base::app};
		if (file.is_open()) {
			std::unique_lock write_lock{m_Mutex};
			std::swap(m_LogFileBuffer, m_Entries);
			write_lock.unlock();

			for (const auto& entry : m_LogFileBuffer) {
				file << "[" << EntryType::ToString[entry.Type] << "] " << entry.Log << "\n";
			}
			m_LogFileBuffer.clear();
		}
	}

	void Console::Write_(EntryType::Enum log_type, const char* log) {
		printf("\n[%-7s] %s", EntryType::ToString[log_type], log);

		Console::Entry entry{log, log_type};

		std::unique_lock lock{m_Mutex};
		m_Entries.push_back(entry);
		++m_TotalEntryCount;
		auto size = m_Entries.size();
		lock.unlock();

		if (size > m_LogFileBufferEntryCount) { Flush(); }
	}
	void Console::Append_(const char* log) {
		printf("%s", log);
		std::unique_lock write_lock{m_Mutex};
		m_Entries.back().Log.append(log);
	}

}