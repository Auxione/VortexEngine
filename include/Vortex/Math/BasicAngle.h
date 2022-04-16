#pragma once
#include "Vortex/Math/VortexMathConstants.h"

namespace Vortex::Math {
	template<typename T>
	struct BasicAngle {
	public:
		constexpr BasicAngle(): m_Data{T{0}} {}
		constexpr BasicAngle(const BasicAngle&) = default;
		constexpr BasicAngle(BasicAngle&&) noexcept = default;

	protected:
		explicit constexpr BasicAngle(T value): m_Data{value} {}

	public:
		constexpr static BasicAngle FromDegrees(T value) { return BasicAngle{static_cast<T>(DegToRad<T>()) * value}; }
		constexpr static BasicAngle FromRadians(T value) { return BasicAngle{value}; }

	public:
		constexpr BasicAngle operator+(const BasicAngle& rhs) const { return BasicAngle{m_Data + rhs.m_Data}; }
		constexpr BasicAngle operator-(const BasicAngle& rhs) const { return BasicAngle{m_Data - rhs.m_Data}; }

		constexpr BasicAngle& operator+=(const BasicAngle& rhs) {
			m_Data += rhs.m_Data;
			return *this;
		}
		constexpr BasicAngle& operator-=(const BasicAngle& rhs) {
			m_Data -= rhs.m_Data;
			return *this;
		}

		constexpr BasicAngle operator*(T rhs) const { return BasicAngle{m_Data * rhs}; }
		constexpr BasicAngle operator/(T rhs) const { return BasicAngle{m_Data / rhs}; }

		constexpr BasicAngle& operator*=(T rhs) {
			m_Data *= rhs;
			return *this;
		}
		constexpr BasicAngle& operator/=(T rhs) {
			m_Data /= rhs;
			return *this;
		}

	public:
		constexpr friend bool operator==(const BasicAngle& lhs, const BasicAngle& rhs) { return lhs.m_Data == rhs.m_Data; }
		constexpr friend bool operator<(const BasicAngle& lhs, const BasicAngle& rhs) { return lhs.m_Data < rhs.m_Data; }
		constexpr friend bool operator>(const BasicAngle& lhs, const BasicAngle& rhs) { return lhs.m_Data > rhs.m_Data; }
		constexpr friend bool operator!=(const BasicAngle& lhs, const BasicAngle& rhs) { return lhs.m_Data != rhs.m_Data; }
		constexpr friend bool operator<=(const BasicAngle& lhs, const BasicAngle& rhs) { return lhs.m_Data <= rhs.m_Data; }
		constexpr friend bool operator>=(const BasicAngle& lhs, const BasicAngle& rhs) { return lhs.m_Data >= rhs.m_Data; }

		constexpr BasicAngle operator-() const { return BasicAngle{-m_Data}; }
		constexpr BasicAngle& operator=(const BasicAngle&) = default;
		constexpr BasicAngle& operator=(BasicAngle&&) noexcept = default;

	public:
		constexpr T ToDegrees() const { return static_cast<T>(RadToDeg<T>()) * m_Data; }
		constexpr T ToRadians() const { return m_Data; }

		constexpr void SetDegrees(T value) { m_Data = DegToRad<T>() * value; }
		constexpr void SetRadians(T value) { m_Data = value; }

	protected:
		T m_Data;
	};
}