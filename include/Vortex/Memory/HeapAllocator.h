#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Common/Logger.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Memory/Allocator.h"

#include <iostream>

#ifdef VORTEX_DEBUG
#include <unordered_map>
#endif

namespace Vortex::Memory {
	class HeapAllocator: public Allocator {
	public:
		HeapAllocator() = default;
		~HeapAllocator() override;

	public:
		inline void* Allocate(SizeType size) override {
			//auto ptr = malloc(size);
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
		inline void Deallocate(void* ptr) override {
#ifdef VORTEX_DEBUG
			auto byte_ptr = static_cast<Byte*>(ptr);
			d_AllocRegistry.erase(byte_ptr);
			d_SizeRegistry.erase(byte_ptr);
#endif
			delete[] static_cast<Byte*>(ptr);
			//free(ptr);
		}

#ifdef VORTEX_DEBUG
	private:
		SizeType d_NextAllocationID{1};
		static SizeType d_MarkedID;
		std::unordered_map<Byte*, SizeType> d_AllocRegistry;
		std::unordered_map<Byte*, SizeType> d_SizeRegistry;

	public:
		inline static void MarkAllocation(SizeType id) { d_MarkedID = id; }
#endif
	};
}

namespace Vortex {
	Memory::HeapAllocator* GetHeapAllocator();
}