#include "Vortex/Memory/HeapAllocator.h"

namespace Vortex::Memory {
	HeapAllocator::~HeapAllocator() {
#ifdef VORTEX_DEBUG
		for (const auto allocs : d_AllocRegistry) {
			VORTEX_LOG_ERROR("[HeapAllocator] Allocation %zu is leaked %zu bytes from memory address %p.", allocs.second, d_SizeRegistry[allocs.first], allocs.first);
		}
#endif
	}

	static HeapAllocator s_HeapAllocator;

#ifdef VORTEX_DEBUG
	SizeType HeapAllocator::d_MarkedID = 0;
#endif
}

namespace Vortex {
	Memory::HeapAllocator* GetHeapAllocator() { return &Memory::s_HeapAllocator; }
}