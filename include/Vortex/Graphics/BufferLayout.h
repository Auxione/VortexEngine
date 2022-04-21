#pragma once
#include <initializer_list>
#include "Vortex/Graphics/GraphicsEnum.h"

namespace Vortex::Graphics {
	template<SizeType Cap>
	struct BasicBufferLayout {
		ElementType::Enum Types[Cap];
		UInt16 Sizes[Cap];
		UInt16 Offsets[Cap];
		bool Normalized[Cap];

		UInt16 ElementPerInstance;
		SizeType Count;
		SizeType Stride;
	};
	using BufferLayout = BasicBufferLayout<16>;

	template<typename ... Args>
	constexpr BufferLayout CreateBufferLayout(Args ... args) {
		BufferLayout layout;
		layout.Stride = 0;
		layout.ElementPerInstance = 0;
		UInt16 offset = 0;
		int i = 0;
		([&](auto element) {
			layout.Types[i] = element;
			layout.Sizes[i] = ElementType::Size[element];
			layout.Offsets[i] = offset;
			layout.Normalized[i] = false;

			offset += layout.Sizes[i];
			layout.Stride += layout.Sizes[i];

			++i;
		}(args), ...);
		layout.Count = i;
		return layout;
	}
}