#pragma once
#include <shared_mutex>
#include "Vortex/Debug/Assert.h"

namespace Vortex {
	template<typename T>
	class ThreadSafeReader {
	public:
		constexpr ThreadSafeReader(std::shared_mutex& shared_mutex, const T* obj)
			: m_Lock(shared_mutex),
			  m_Object{obj} {}

		constexpr ThreadSafeReader(const ThreadSafeReader&) = delete;
		constexpr ThreadSafeReader(ThreadSafeReader&&) noexcept = default;
		inline ~ThreadSafeReader() = default;

	public:
		constexpr void Lock() {
			m_Lock.lock();
#ifdef VORTEX_DEBUG
			d_IsLocked = true;
#endif
		}
		constexpr void Unlock() {
			m_Lock.unlock();
#ifdef VORTEX_DEBUG
			d_IsLocked = false;
#endif
		}

	public:
		constexpr const T* Get() const {
			VORTEX_ASSERT(d_IsLocked)
			return m_Object;
		}
		constexpr const T* operator->() const { return Get(); }
		constexpr const T& operator*() const { return *Get(); }

	protected:
		const T* m_Object;
		std::shared_lock<std::shared_mutex> m_Lock;

#ifdef VORTEX_DEBUG
	public:
		bool d_IsLocked{true};
#endif
	};

	template<typename T>
	class ThreadSafeWriter {
	public:
		constexpr ThreadSafeWriter(std::shared_mutex& shared_mutex, T* obj)
			: m_Lock(shared_mutex),
			  m_Object{obj} {}
		constexpr ThreadSafeWriter(const ThreadSafeWriter&) = delete;
		constexpr ThreadSafeWriter(ThreadSafeWriter&&) noexcept = default;
		inline ~ThreadSafeWriter() = default;

	public:
		constexpr void Lock() {
			m_Lock.lock();
#ifdef VORTEX_DEBUG
			d_IsLocked = true;
#endif
		}
		constexpr void Unlock() {
			m_Lock.unlock();
#ifdef VORTEX_DEBUG
			d_IsLocked = false;
#endif
		}

	public:
		constexpr T* Get() {
			VORTEX_ASSERT(d_IsLocked)
			return m_Object;
		}
		constexpr const T* Get() const {
			VORTEX_ASSERT(d_IsLocked)
			return m_Object;
		}

		constexpr T* operator->() { return Get(); }
		constexpr const T* operator->() const { return Get(); }

		constexpr T& operator*() { return *Get(); }
		constexpr const T& operator*() const { return *Get(); }

	protected:
		T* m_Object;
		std::unique_lock<std::shared_mutex> m_Lock;

#ifdef VORTEX_DEBUG
	public:
		bool d_IsLocked{true};
#endif
	};
}