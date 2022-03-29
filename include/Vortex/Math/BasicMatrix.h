#pragma once
#include "Vortex/Memory/Memory.h"

namespace Vortex {
	//	Basic NxM matrix class.
	template<SizeType Width, SizeType Height, typename T>
	struct BasicMatrix {
		constexpr BasicMatrix() = default;
		constexpr BasicMatrix(const BasicMatrix&) = default;
		constexpr BasicMatrix(BasicMatrix&&) noexcept = default;
		constexpr BasicMatrix(const T* data) { std::memcpy(m_Data, data, Width * Height * sizeof(T)); };
		inline ~BasicMatrix() = default;

	public:
		constexpr T* Data() { return m_Data; }
		constexpr const T* Data() const { return m_Data; }

	public:
		T& operator[](Vortex::SizeType i) { return m_Data[i]; }
		const T& operator[](Vortex::SizeType i) const { return m_Data[i]; }
		constexpr BasicMatrix& operator=(const BasicMatrix&) = default;
		constexpr BasicMatrix& operator=(BasicMatrix&&) noexcept = default;

	protected:
		constexpr void Zero() {
			for (int i = 0; i < Width * Height; ++i) {
				m_Data[i] = T{0};
			}
		}

	protected:
		T m_Data[Width * Height];
	};
}