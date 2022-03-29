#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/BasicVector.h"

namespace Vortex {
	template<typename T>
	struct BasicTriangle {
		constexpr T& x1() { return m_Data[0]; }
		constexpr T& y1() { return m_Data[1]; }
		constexpr T& x2() { return m_Data[2]; }
		constexpr T& y2() { return m_Data[3]; }
		constexpr T& x3() { return m_Data[4]; }
		constexpr T& y3() { return m_Data[5]; }

		constexpr const T& x1() const { return m_Data[0]; }
		constexpr const T& y1() const { return m_Data[1]; }
		constexpr const T& x2() const { return m_Data[2]; }
		constexpr const T& y2() const { return m_Data[3]; }
		constexpr const T& x3() const { return m_Data[4]; }
		constexpr const T& y3() const { return m_Data[5]; }

		constexpr T* Data() { return m_Data; }
		constexpr const T* Data() const { return m_Data; }

	public:
		constexpr BasicTriangle()
			: m_Data{} {}
		constexpr BasicTriangle(T x1, T y1, T x2, T y2, T x3, T y3)
			: m_Data{x1, y1, x2, y2, x3, y3} {}
		constexpr BasicTriangle(const T* values)
			: m_Data{values[0], values[1], values[2], values[3], values[4], values[5]} {}
		inline ~BasicTriangle() = default;

	private:
		constexpr static T Sign(T p_x, T p_y, T o1_x, T o1_y, T o2_x, T o2_y) {
			return (p_x - o2_x) * (o1_y - o2_y) - (o1_x - o2_x) * (p_y - o2_y);
		}

	public:
		constexpr bool Contains(const BasicVector<2, T>& point) const {
			T d1 = Sign(point[0], point[1], m_Data[0], m_Data[1], m_Data[2], m_Data[3]);
			T d2 = Sign(point[0], point[1], m_Data[2], m_Data[3], m_Data[4], m_Data[5]);
			T d3 = Sign(point[0], point[1], m_Data[4], m_Data[5], m_Data[0], m_Data[1]);

			bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
			bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);

			return !(has_neg && has_pos);
		}

	protected:
		T m_Data[6];
	};
}