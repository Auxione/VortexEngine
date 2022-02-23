#include "Vortex/Math/Quaternion.h"

namespace Vortex {
	/*
	void Quaternion::SetEulerXYZ(Angle roll, Angle pitch, Angle yaw) {
		float x = roll.ToRadians();
		float y = pitch.ToRadians();
		float z = yaw.ToRadians();

		float c_yaw = Math::Cos(z * 0.5f);
		float s_yaw = Math::Sin(z * 0.5f);
		float c_pitch = Math::Cos(y * 0.5f);
		float s_pitch = Math::Sin(y * 0.5f);
		float c_roll = Math::Cos(x * 0.5f);
		float s_roll = Math::Sin(x * 0.5f);

		m_Data[0] = s_roll * c_pitch * c_yaw - c_roll * s_pitch * s_yaw;
		m_Data[1] = c_roll * s_pitch * c_yaw + s_roll * c_pitch * s_yaw;
		m_Data[2] = c_roll * c_pitch * s_yaw - s_roll * s_pitch * c_yaw;
		m_Data[3] = c_roll * c_pitch * c_yaw + s_roll * s_pitch * s_yaw;
	}

	Vector3 Quaternion::ToEulerAngles() const {
		Vector3 euler;
		// roll (x-axis rotation)
		float sinr_cosp = 2 * (w() * x() + y() * z());
		float cosr_cosp = 1 - 2 * (x() * x() + y() * y());
		euler.x() = Math::Atan2(sinr_cosp, cosr_cosp);

		// pitch (y-axis rotation)
		float sinp = 2 * (w() * y() - z() * x());
		if (Math::Abs(sinp) >= 1) {
			// use 90 degrees if out of range
			euler.y() = Math::CopySign(Math::PI / 2, sinp);
		} else {
			euler.y() = Math::Asin(sinp);
		}

		// yaw (z-axis rotation)
		float siny_cosp = 2 * (w() * z() + x() * y());
		float cosy_cosp = 1 - 2 * (y() * y() + z() * z());
		euler.z() = Math::Atan2(siny_cosp, cosy_cosp);

		return euler * Math::RadToDeg;
	}

	void Quaternion::SetEulerX(Angle angle) {
		float radians = angle.ToRadians() * 0.5f;

		m_Data[0] = Math::Sin(radians);
		m_Data[1] = 0.0f;
		m_Data[2] = 0.0f;
		m_Data[3] = Math::Cos(radians);
	}

	void Quaternion::SetEulerY(Angle angle) {
		float radians = angle.ToRadians() * 0.5f;

		m_Data[0] = 0.0f;
		m_Data[1] = Math::Sin(radians);
		m_Data[2] = 0.0f;
		m_Data[3] = Math::Cos(radians);
	}

	void Quaternion::SetEulerZ(Angle angle) {
		float radians = angle.ToRadians() * 0.5f;

		m_Data[0] = 0.0f;
		m_Data[1] = 0.0f;
		m_Data[2] = Math::Sin(radians);
		m_Data[3] = Math::Cos(radians);
	}

	//Vector must be normalized!
	void Quaternion::SetAngleAxis(const Vector3& axis, Angle angle) {
		float half_angle_rad = angle.ToRadians() * 0.5f;
		float sin = Math::Sin(half_angle_rad);

		m_Data[0] = axis.x() * sin;
		m_Data[1] = axis.y() * sin;
		m_Data[2] = axis.z() * sin;
		m_Data[3] = Math::Cos(half_angle_rad);
	}*/
}