
#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
  #error "Platform not implemented"
#endif

#include "Vortex/Common/DynamicLibrary.h"

namespace Vortex {
	DynamicLibrary::DynamicLibrary(std::filesystem::path path, bool can_hot_reload)
		: m_CanHotReload{can_hot_reload},
		  m_Loaded{false},
		  m_Path{std::move(path)},
		  m_InstancePtr{nullptr},
		  m_Functions{} {}

	DynamicLibrary::~DynamicLibrary() {
		if (IsLoaded()) {
			Unload();

			if (m_CanHotReload) {
				m_Path.replace_extension(HotReloadPrefix);
				std::filesystem::remove(m_Path);
			}
		}
	}

	DynamicLibrary::Address DynamicLibrary::operator[](const char* name) {
		VORTEX_ASSERT(IsLoaded())
		auto it = m_Functions.find(name);
		if (it != m_Functions.end()) {
			return Address{it->second};
		}

		FARPROC ptr = GetProcAddress(reinterpret_cast<HMODULE>(m_InstancePtr), name);
		if (ptr != nullptr) {
			m_Functions.emplace(std::pair(name, ptr));
		}
		VORTEX_ASSERT_MSG(ptr != nullptr, "Function signature not found!")
		return Address{ptr};
	}

	void DynamicLibrary::Load() {
		VORTEX_ASSERT(!IsLoaded())
		VORTEX_ASSERT(Exists())

		std::filesystem::path lib_path{m_Path};
		if (m_CanHotReload) {
			lib_path.replace_extension(HotReloadPrefix);
			std::filesystem::copy_file(m_Path, lib_path, std::filesystem::copy_options::update_existing);
		}

		m_InstancePtr = LoadLibrary(lib_path.string().c_str());
		m_Loaded = m_InstancePtr != nullptr;
	}

	void DynamicLibrary::Unload() {
		VORTEX_ASSERT(IsLoaded())

		FreeLibrary(m_InstancePtr);
		m_Loaded = false;

		m_Functions.clear();
	}
}