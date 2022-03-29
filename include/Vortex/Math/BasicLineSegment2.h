#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/BasicVector.h"

namespace Vortex {
	template<typename T>
	struct BasicLineSegment2 {
		constexpr T& x1() { return m_Data[0]; }
		constexpr T& y1() { return m_Data[1]; }
		constexpr T& x2() { return m_Data[2]; }
		constexpr T& y2() { return m_Data[3]; }

		constexpr const T& x1() const { return m_Data[0]; }
		constexpr const T& y1() const { return m_Data[1]; }
		constexpr const T& x2() const { return m_Data[2]; }
		constexpr const T& y2() const { return m_Data[3]; }

		constexpr T* Data() { return m_Data; }
		constexpr const T* Data() const { return m_Data; }

	public:
		constexpr BasicLineSegment2()
			: m_Data{} {}
		constexpr BasicLineSegment2(T x1, T y1, T x2, T y2)
			: m_Data{x1, y1, x2, y2} {}
		constexpr BasicLineSegment2(const T* values)
			: m_Data{values[0], values[1], values[2], values[3]} {}

	public:
		constexpr bool Intersects(const BasicLineSegment2<T>& other) {
			T D = 1.0f / ((other.y2() - other.y1()) * (x2() - x1()) - (other.x2() - other.x1()) * (y2() - y1()));

			T uA = ((other.x2() - other.x1()) * (y1() - other.y1()) - (other.y2() - other.y1()) * (x1() - other.x1())) * D;
			T uB = ((x2() - x1()) * (y1() - other.y1()) - (y2() - y1()) * (x1() - other.x1())) * D;

			return uA >= 0.0f && uA <= 1.0f && uB >= 0.0f && uB <= 1.0f;
		}

		constexpr bool Intersects(const BasicLineSegment2<T>& other, BasicVector<2, T>& intersection_point) {
			T D = 1.0f / ((other.y2() - other.y1()) * (x2() - x1()) - (other.x2() - other.x1()) * (y2() - y1()));

			T uA = ((other.x2() - other.x1()) * (y1() - other.y1()) - (other.y2() - other.y1()) * (x1() - other.x1())) * D;
			T uB = ((x2() - x1()) * (y1() - other.y1()) - (y2() - y1()) * (x1() - other.x1())) * D;

			if (uA >= T{0} && uA <= T{1} && uB >= T{0} && uB <= T{1}) {
				intersection_point[0] = x1() + (uA * (x2() - x1()));
				intersection_point[1] = y1() + (uA * (y2() - y1()));
				return true;
			} else {
				return false;
			}
		}

		T Length() const {
			T dx = x2() - x1();
			T dy = y2() - y1();
			return Math::Sqrt(dx * dx + dy * dy);
		}

		T Distance(const BasicVector<2, T>& point) {
			T line_a = (x2() - x1()) * (y1() - point[1]);
			T line_b = (x1() - point[0]) * (y2() - y1());
			return Math::Abs(line_a - line_b) / Length();
		}

	protected:
		T m_Data[4];
	};
}