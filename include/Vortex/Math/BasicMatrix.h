#pragma once

namespace Vortex {
	//	Basic NxM matrix class.
	template<SizeType Width, SizeType Height, typename T>
	struct BasicMatrix {
		constexpr BasicMatrix() = default;
		constexpr BasicMatrix(const T* data) { std::memcpy(m_Data, data, Width * Height * sizeof(T)); };
		inline ~BasicMatrix() = default;

	public:
		inline operator T*() { return m_Data; }
		inline operator const T*() const { return m_Data; }

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