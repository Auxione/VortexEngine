#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Containers/DynamicArray.h"

#include "Vortex/Graphics/GraphicsEnum.h"

namespace Vortex::Graphics {
	class BufferLayout {
	public:
		struct BufferElement {
			ElementType::Enum Type;
			UInt16 Size;
			UInt16 Offset;
			bool Normalized;
		};

		using iterator = DynamicArray<BufferElement>::iterator;
		using const_iterator = DynamicArray<BufferElement>::const_iterator;

	public:
		BufferLayout()
			: m_Stride{0}, m_Elements{}, ElementPerInstance{0} {}

		template<SizeType N>
		BufferLayout(const ElementType::Enum (& types)[N])
			: m_Stride{0}, m_Elements{N}, ElementPerInstance{0} {

			UInt16 offset = 0;
			for (int i = 0; i < N; ++i) {
				auto type = types[i];
				BufferElement element{
					type
					, ElementType::Size[type]
					, 0
					, false
				};

				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
				m_Elements[i] = element;
			}
		}

	public:
		BufferElement& operator[](SizeType index) { return m_Elements[index]; }
		const BufferElement& operator[](SizeType index) const { return m_Elements[index]; }

		SizeType Count() const { return m_Elements.Capacity(); }
		SizeType GetStride() const { return m_Stride; }

		SizeType CalculateBufferSize(SizeType count) const { return count * GetStride(); }

	public:
		BufferLayout::iterator begin() { return m_Elements.begin(); }
		BufferLayout::iterator end() { return m_Elements.end(); }
		BufferLayout::const_iterator cbegin() const { return m_Elements.cbegin(); }
		BufferLayout::const_iterator cend() const { return m_Elements.cend(); }

	public:
		UInt16 ElementPerInstance;

	private:
		SizeType m_Stride;
		DynamicArray<BufferElement> m_Elements;
	};
}