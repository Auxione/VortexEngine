#pragma once
#include <vector>
#include "Vortex/Graphics/BufferLayout.h"

namespace Vortex::Graphics {
	template<SizeType Cap>
	struct BasicMeshLayout {
		BufferLayout BufferLayouts[Cap];
		SizeType Count;
	};

	using MeshLayout = BasicMeshLayout<16>;

	template<typename ... Args>
	constexpr MeshLayout CreateMeshLayout(Args ... args) {
		MeshLayout layout;
		int i = 0;
		([&](auto element) {
			layout.BufferLayouts[i] = CreateBufferLayout(element);

			++i;
		}(args), ...);

		layout.Count = i;
		return layout;
	}
}