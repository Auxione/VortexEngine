#pragma once
#include <initializer_list>
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

		using iterator = std::vector<BufferElement>::iterator;
		using const_iterator = std::vector<BufferElement>::const_iterator;

	public:
		BufferLayout()
			: m_Stride{0}, m_Elements{}, ElementPerInstance{0} {}

		BufferLayout(std::initializer_list<ElementType::Enum> element_types)
			: m_Stride{0}, m_Elements(), ElementPerInstance{0} {

			UInt16 offset = 0;
			for (auto type : element_types) {
				BufferElement element{};
				element.Type = type;
				element.Size = ElementType::Size[type];
				element.Offset = offset;
				element.Normalized = false;

				offset += element.Size;
				m_Stride += element.Size;
				m_Elements.emplace_back(element);
			}
		}

	public:
		BufferElement& operator[](SizeType index) { return m_Elements[index]; }
		const BufferElement& operator[](SizeType index) const { return m_Elements[index]; }

		SizeType Count() const { return m_Elements.capacity(); }
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
		std::vector<BufferElement> m_Elements;
	};
}