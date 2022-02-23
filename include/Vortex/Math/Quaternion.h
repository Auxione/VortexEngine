#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/VortexMath.h"

namespace Vortex {
	/*
	struct Quaternion {
		constexpr float& x() { return m_Data[0]; }
		constexpr float& y() { return m_Data[1]; }
		constexpr float& z() { return m_Data[2]; }
		constexpr float& w() { return m_Data[3]; }
		constexpr const float& x() const { return m_Data[0]; }
		constexpr const float& y() const { return m_Data[1]; }
		constexpr const float& z() const { return m_Data[2]; }
		constexpr const float& w() const { return m_Data[3]; }

		constexpr operator float*() { return m_Data; }
		constexpr operator const float*() const { return m_Data; }

	public:
		constexpr Quaternion()
			: m_Data{0.0f, 0.0f, 0.0f, 1.0f} {}
		constexpr Quaternion(float x_, float y_, float z_, float w_)
			: m_Data{x_, y_, z_, w_} {}
		constexpr Quaternion(const Vector3& real, float scalar)
			: m_Data{real.x(), real.y(), real.z(), scalar} {}

		constexpr Quaternion(const Quaternion& other) = default;
		constexpr Quaternion(Quaternion&&) noexcept = default;

	public:
		constexpr Quaternion& operator=(const Quaternion& other) = default;
		constexpr Quaternion& operator=(Quaternion&&) noexcept = default;

		constexpr friend Quaternion operator*(const Quaternion& lhs, const Quaternion& rhs) {
			return Quaternion{
				rhs.w() * lhs.x() + rhs.x() * lhs.w() + rhs.y() * lhs.z() - rhs.z() * lhs.y()
				, rhs.w() * lhs.y() + rhs.y() * lhs.w() + rhs.z() * lhs.x() - rhs.x() * lhs.z()
				, rhs.w() * lhs.z() + rhs.z() * lhs.w() + rhs.x() * lhs.y() - rhs.y() * lhs.x()
				, rhs.w() * lhs.w() - rhs.x() * lhs.x() - rhs.y() * lhs.y() - rhs.z() * lhs.z()
			};
		}

		constexpr friend Quaternion& operator*=(Quaternion& lhs, const Quaternion& rhs) {
			lhs = lhs * rhs;
			return lhs;
		}

		constexpr friend bool operator==(const Quaternion& lhs, const Quaternion& rhs) {
			return lhs.x() == rhs.x()
				&& lhs.y() == rhs.y()
				&& lhs.z() == rhs.z()
				&& lhs.w() == rhs.w();
		}
		constexpr friend bool operator!=(const Quaternion& lhs, const Quaternion& rhs) {
			return lhs.x() == rhs.x()
				|| lhs.y() == rhs.y()
				|| lhs.z() == rhs.z()
				|| lhs.w() == rhs.w();
		}

	public:
		constexpr float Magnitude() const { return m_Data[0] * m_Data[0] + m_Data[1] * m_Data[1] + m_Data[2] * m_Data[2] + m_Data[3] * m_Data[3]; }
		float SqrMagnitude() const {
			float magnitude = Magnitude();
			if (magnitude > Math::Epsilon) {
				return Math::Sqrt(magnitude);
			} else {
				return 0.0f;
			}
		}

		void Normalize() {
			float magnitude = SqrMagnitude();
			if (magnitude > Math::Epsilon) {
				float rsqrt_magnitude = 1.0f / magnitude;
				m_Data[0] *= rsqrt_magnitude;
				m_Data[1] *= rsqrt_magnitude;
				m_Data[2] *= rsqrt_magnitude;
				m_Data[3] *= rsqrt_magnitude;
			} else {
				m_Data[0] = 0.0f;
				m_Data[1] = 0.0f;
				m_Data[2] = 0.0f;
				m_Data[3] = 0.0f;
			}
		}

		constexpr void Set(const Vector3& real, float scalar) {
			m_Data[0] = real[0];
			m_Data[1] = real[1];
			m_Data[2] = real[2];
			m_Data[3] = scalar;
		}
		constexpr void Set(float x, float y, float z, float scalar) {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = scalar;
		}

		constexpr void Identity() {
			m_Data[0] = 0.0f;
			m_Data[1] = 0.0f;
			m_Data[2] = 0.0f;
			m_Data[3] = 1.0f;
		}

		constexpr void Conjugate() {
			m_Data[0] = -m_Data[0];
			m_Data[1] = -m_Data[1];
			m_Data[2] = -m_Data[2];
		}

		void SetEulerXYZ(Angle roll, Angle pitch, Angle yaw);
		Vector3 ToEulerAngles() const;
		void SetEulerX(Angle angle);
		void SetEulerY(Angle angle);
		void SetEulerZ(Angle angle);
		void SetAngleAxis(const Vector3& axis, Angle angle);

		void Lerp(float ratio, const Quaternion& target) {
			for (std::size_t i = 0; i < 4; ++i) {
				this->m_Data[i] = Math::Lerp(this->m_Data[i], target.m_Data[i], ratio);
			}
			Normalize();
		}

	public:
		constexpr static float Dot(const Quaternion& lhs, const Quaternion& rhs) {
			return lhs.x() * rhs.x() + lhs.y() * rhs.y() + lhs.z() * rhs.z() + lhs.w() * rhs.w();
		}

		static float Angle(const Quaternion& lhs, const Quaternion& rhs) {
			float dot = Dot(lhs, rhs);
			return Math::Acos(Math::Min(Math::Abs(dot), 1.0f)) * 2.0f;
		}

	protected:
		float m_Data[4];
	};*/
}