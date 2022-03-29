#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/BasicVector.h"

namespace Vortex {
	template<typename T>
	struct BasicCircle {
		constexpr T& x() { return m_Data[0]; }
		constexpr T& y() { return m_Data[1]; }
		constexpr T& r() { return m_Data[2]; }
		constexpr const T& x() const { return m_Data[0]; }
		constexpr const T& y() const { return m_Data[1]; }
		constexpr const T& r() const { return m_Data[2]; }

		constexpr T* Data() { return m_Data; }
		constexpr const T* Data() const { return m_Data; }

	public:
		constexpr BasicCircle()
			: m_Data{} {}
		constexpr BasicCircle(T x, T y, T radius)
			: m_Data{x, y, radius} {}
		constexpr BasicCircle(const T* values)
			: m_Data{values[0], values[1], values[2]} {}
		inline ~BasicCircle() = default;

	public:
		constexpr bool Contains(const BasicVector<2, T>& point) const {
			T dx = m_Data[0] - point[0];
			T dy = m_Data[1] - point[1];
			return dx * dx + dy * dy < r() * r();
		}

		constexpr bool Intersects(const BasicCircle<T>& other) {
			T radius_sum_sqr = r() + other.r();
			T dx = x() - other.x();
			T dy = y() - other.y();
			return radius_sum_sqr * radius_sum_sqr < dx * dx + dy * dy;
		}

	protected:
		T m_Data[3];
	};
}