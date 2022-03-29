#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/BasicVector.h"

namespace Vortex {
	//Axis-Aligned Rectangle class

	template<typename T>
	struct BasicRectangle {
		constexpr T& x() { return m_Data[0]; }
		constexpr T& y() { return m_Data[1]; }
		constexpr T& Width() { return m_Data[2]; }
		constexpr T& Height() { return m_Data[3]; }
		constexpr const T& x() const { return m_Data[0]; }
		constexpr const T& y() const { return m_Data[1]; }
		constexpr const T& Width() const { return m_Data[2]; }
		constexpr const T& Height() const { return m_Data[3]; }

		constexpr T* Data() { return m_Data; }
		constexpr const T* Data() const { return m_Data; }

	public:
		constexpr BasicRectangle()
			: m_Data{} {}
		constexpr BasicRectangle(const T* values)
			: m_Data{values[0], values[1], values[2], values[3]} {}
		constexpr BasicRectangle(T x, T y, T width, T height)
			: m_Data{x, y, width, height} {}

		inline ~BasicRectangle() = default;

	public:
		constexpr bool Contains(const Vortex::BasicVector<2, T>& point) const {
			return point[0] >= x()
				&& point[0] <= x() + Width()
				&& point[1] >= y()
				&& point[1] <= y() + Height();
		}

		constexpr bool Intersects(const BasicRectangle<T>& other) {
			return x() + Width() > other.x() &&
				y() + Height() > other.y() &&
				x() < other.x() + other.Width() &&
				y() < other.y() + other.Height();
		}

	protected:
		T m_Data[4];
	};
}