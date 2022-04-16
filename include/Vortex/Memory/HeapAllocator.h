#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Common/Console.h"
#include "Vortex/Debug/Assert.h"

#include <iostream>

#ifdef VORTEX_DEBUG
#include <unordered_map>
#endif

namespace Vortex::Memory {
	class HeapAllocator {
#ifdef VORTEX_DEBUG
		~HeapAllocator() {
			for (auto allocations : d_AllocRegistry) {
				Console::WriteError("[HeapAllocator] Allocation %zu is leaked %zu bytes from memory address %p.", allocations.second, d_SizeRegistry[allocations.first], allocations.first);
			}
		}
#endif

	private:
		inline static Byte* GetBasePtr(void* ptr) {
			return static_cast<Byte*>(ptr) - sizeof(UInt64);
		}
		inline static UInt64& Count(void* ptr) {
			auto* byte_ptr = GetBasePtr(ptr);
			return *reinterpret_cast<UInt64*>(byte_ptr);
		}

	public:
		inline void* Allocate(SizeType size) {
			auto ptr = new Byte[size];

#ifdef VORTEX_DEBUG
			VORTEX_ASSERT(d_MarkedID != d_NextAllocationID)
			auto byte_ptr = static_cast<Byte*>(ptr);
			d_AllocRegistry[byte_ptr] = d_NextAllocationID;
			d_SizeRegistry[byte_ptr] = size;
			++d_NextAllocationID;
#endif

			return ptr;
		}

		inline void Deallocate(void* ptr) {
#ifdef VORTEX_DEBUG
			auto byte_ptr = static_cast<Byte*>(ptr);
			d_AllocRegistry.erase(byte_ptr);
			d_SizeRegistry.erase(byte_ptr);
#endif

			delete[] static_cast<Byte*>(ptr);
		}

#ifdef VORTEX_DEBUG
	private:
		SizeType d_NextAllocationID{1};
		SizeType d_MarkedID;
		std::unordered_map<Byte*, SizeType> d_AllocRegistry;
		std::unordered_map<Byte*, SizeType> d_SizeRegistry;

	public:
		inline void MarkAllocation(SizeType id) { d_MarkedID = id; }
#endif

	public:
		template<typename T, typename ...Args>
		inline T* New(Args&& ...args) {
			auto alloc_size = sizeof(UInt64) + sizeof(T);
			auto* ptr = Allocate(alloc_size);
			auto* byte_ptr = static_cast<Byte*>(ptr);

			new(byte_ptr) UInt64(1);
			return new(byte_ptr + sizeof(UInt64)) T(std::forward<Args>(args)...);
		}

		template<typename T>
		inline T* NewArray(SizeType count) {
			auto alloc_size = sizeof(UInt64) + sizeof(T) * count;
			auto* ptr = Allocate(alloc_size);
			auto* byte_ptr = static_cast<Byte*>(ptr);

			new(byte_ptr) UInt64(count);
			return new(byte_ptr + sizeof(UInt64)) T[count];
		}

		template<typename T>
		inline void Delete(T* ptr) {
			if (ptr != nullptr) {
				auto count = Count(ptr);
				for (int i = 0; i < count; ++i) {
					ptr[i].~T();
				}

				Deallocate(GetBasePtr(ptr));
			}
		}
	};
}