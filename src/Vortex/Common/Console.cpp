#include "Vortex/Common/Console.h"

namespace Vortex {
	Console* Console::s_Instance{nullptr};

	Console::Console()
		: m_Entries(),
		  m_TotalEntryCount{0},
		  m_Mutex() {
	}
	Console::~Console() = default;

	void Console::Write(EntryType::Enum log_type, const char* log) {
		printf("[%-7s] %s\n", EntryType::ToString[log_type], log);

		Console::Entry entry{log, log_type};
		std::unique_lock lock{m_Mutex};
		m_Entries.push_back(entry);
		++m_TotalEntryCount;
	}
}