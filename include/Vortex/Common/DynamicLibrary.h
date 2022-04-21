#pragma once

#include <filesystem>
#include <utility>
#include <unordered_map>

#ifdef WIN32
  #include "minwindef.h"
#else
  #error "Platform not implemented"
#endif

namespace Vortex {
	class DynamicLibrary final {
	public:
		constexpr static const char* HotReloadPrefix = "Loaded";

		class Address {
		public:
			explicit Address(FARPROC ptr): m_Ptr(ptr) {}

			template<typename T, typename = std::enable_if_t<std::is_function_v<T>>>
			operator T*() const {
				VORTEX_ASSERT(m_Ptr != nullptr)
				return reinterpret_cast<T*>(m_Ptr);
			}

			template<typename Ret_Val = void, typename ... Args>
			constexpr decltype(auto) Invoke(Args ... args) const {
				VORTEX_ASSERT(m_Ptr != nullptr)
				using Fn_Type = Ret_Val(*)(Args...);

				auto func = reinterpret_cast<Fn_Type>(m_Ptr);
				return func(std::forward<Args>(args)...);
			}

		private:
			FARPROC m_Ptr;
		};

	public:
		explicit DynamicLibrary(std::filesystem::path path, bool can_hot_reload = false);
		~DynamicLibrary();

	public:
		Address operator[](const char* name);

	public:
		void Load();
		void Unload();

		inline void Reload() {
			VORTEX_ASSERT(CanHotReload())
			if (IsLoaded()) {
				Unload();
			}
			Load();
		}

	public:
		inline bool Exists() const { return std::filesystem::exists(m_Path); }
		constexpr bool IsLoaded() const { return m_Loaded; }
		constexpr bool CanHotReload() const { return m_CanHotReload; }

	private:
		bool m_CanHotReload;
		bool m_Loaded;
		std::filesystem::path m_Path;

		HINSTANCE m_InstancePtr;
		std::unordered_map<LPCSTR, FARPROC> m_Functions;
	};
}