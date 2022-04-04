#pragma once
#include "Vortex/Math/VortexMath.h"

namespace Vortex {
	Vector2 Matrix2::operator*(const Vector2& rhs) const {
		// | 0 2 |   | 0 |
		// | 1 3 | * | 1 |
		return Vector2{
			m_Data[0] * rhs[0] + m_Data[2] * rhs[1]
			, m_Data[1] * rhs[0] + m_Data[3] * rhs[1]
		};
	}
	Matrix2 Matrix2::operator*(const Matrix2& rhs) const {
		// | 0 2 |   | 0 2 |
		// | 1 3 | * | 1 3 |
		return Matrix2{
			m_Data[0] * rhs[0] + m_Data[2] * rhs[1]
			, m_Data[1] * rhs[0] + m_Data[3] * rhs[1]
			, m_Data[0] * rhs[2] + m_Data[2] * rhs[3]
			, m_Data[1] * rhs[2] + m_Data[3] * rhs[3]
		};
	}
	Matrix2& Matrix2::operator*=(const Matrix2& rhs) {
		float temp[4];
		temp[0] = m_Data[0] * rhs[0] + m_Data[2] * rhs[1];
		temp[1] = m_Data[1] * rhs[0] + m_Data[3] * rhs[1];
		temp[2] = m_Data[0] * rhs[2] + m_Data[2] * rhs[3];
		temp[3] = m_Data[1] * rhs[2] + m_Data[3] * rhs[3];

		std::memcpy(m_Data, temp, 4 * sizeof(float));
		return *this;
	}

	void Matrix2::Identity() {
		m_Data[0] = 1.0f;
		m_Data[1] = 0.0f;
		m_Data[2] = 1.0f;
		m_Data[3] = 0.0f;
	}
	float Matrix2::Determinant() const {
		return m_Data[0] * m_Data[3] - m_Data[1] * m_Data[2];
	}
	bool Matrix2::Invert() {
		float det = Determinant();

		if (Math::Abs(det) > Math::Epsilon) {
			float inv_det = 1.0f / det;
			float temp = m_Data[0];

			m_Data[0] = inv_det * m_Data[3];
			m_Data[1] = -inv_det * m_Data[1];
			m_Data[2] = -inv_det * m_Data[2];
			m_Data[3] = inv_det * temp;
			return true;
		}
		return false;
	}
	void Matrix2::Transpose() {
		std::swap(m_Data[1], m_Data[2]);
	}
	void Matrix2::SetRotation(const Angle& angle) {
		float cos_alpha = Math::Cos(angle.ToRadians());
		float sin_alpha = Math::Sin(angle.ToRadians());

		m_Data[0] = cos_alpha;
		m_Data[2] = -sin_alpha;

		m_Data[1] = sin_alpha;
		m_Data[3] = cos_alpha;
	}
	bool Matrix2::ExtractRotation(Angle& angle) const {
		if (!Math::Compare(m_Data[0], m_Data[3])
			|| !Math::Compare(m_Data[1], -m_Data[2])) {
			return false;
		}
		angle.SetRadians(Math::Acos(m_Data[0]));
		return true;
	}
}

namespace Vortex {
	Vector3 Matrix3::operator*(const Vector3& rhs) const {
		// |0  3  6|   |0|
		// |1  4  7| * |1|
		// |2  5  8|   |2|
		return Vector3{
			m_Data[0] * m_Data[0] + m_Data[3] * m_Data[1] + m_Data[6] * m_Data[2]
			, m_Data[1] * m_Data[0] + m_Data[4] * m_Data[1] + m_Data[7] * m_Data[2]
			, m_Data[2] * m_Data[0] + m_Data[5] * m_Data[1] + m_Data[8] * m_Data[2]
		};
	}
	Matrix3 Matrix3::operator*(const Matrix3& rhs) const {
		// |0  3  6|   |0  3  6|
		// |1  4  7| * |1  4  7|
		// |2  5  8|   |2  5  8|
		return Matrix3{
			m_Data[0] * rhs[0] + m_Data[3] * rhs[1] + m_Data[6] * rhs[2]
			, m_Data[1] * rhs[0] + m_Data[4] * rhs[1] + m_Data[7] * rhs[2]
			, m_Data[2] * rhs[0] + m_Data[5] * rhs[1] + m_Data[8] * rhs[2]
			, m_Data[0] * rhs[3] + m_Data[3] * rhs[4] + m_Data[6] * rhs[5]
			, m_Data[1] * rhs[3] + m_Data[4] * rhs[4] + m_Data[7] * rhs[5]
			, m_Data[2] * rhs[3] + m_Data[5] * rhs[4] + m_Data[8] * rhs[5]
			, m_Data[0] * rhs[6] + m_Data[3] * rhs[7] + m_Data[6] * rhs[8]
			, m_Data[1] * rhs[6] + m_Data[4] * rhs[7] + m_Data[7] * rhs[8]
			, m_Data[2] * rhs[6] + m_Data[5] * rhs[7] + m_Data[8] * rhs[8]
		};
	}
	Matrix3& Matrix3::operator*=(const Matrix3& rhs) {
		float temp[9];
		temp[0] = m_Data[0] * rhs[0] + m_Data[3] * rhs[1] + m_Data[6] * rhs[2];
		temp[1] = m_Data[1] * rhs[0] + m_Data[4] * rhs[1] + m_Data[7] * rhs[2];
		temp[2] = m_Data[2] * rhs[0] + m_Data[5] * rhs[1] + m_Data[8] * rhs[2];
		temp[3] = m_Data[0] * rhs[3] + m_Data[3] * rhs[4] + m_Data[6] * rhs[5];
		temp[4] = m_Data[1] * rhs[3] + m_Data[4] * rhs[4] + m_Data[7] * rhs[5];
		temp[5] = m_Data[2] * rhs[3] + m_Data[5] * rhs[4] + m_Data[8] * rhs[5];
		temp[6] = m_Data[0] * rhs[6] + m_Data[3] * rhs[7] + m_Data[6] * rhs[8];
		temp[7] = m_Data[1] * rhs[6] + m_Data[4] * rhs[7] + m_Data[7] * rhs[8];
		temp[8] = m_Data[2] * rhs[6] + m_Data[5] * rhs[7] + m_Data[8] * rhs[8];

		std::memcpy(m_Data, temp, 9 * sizeof(float));
		return *this;
	}

	void Matrix3::Identity() {
		m_Data[0] = 1.0f;
		m_Data[1] = 0.0f;
		m_Data[2] = 0.0f;
		m_Data[3] = 0.0f;
		m_Data[4] = 1.0f;
		m_Data[5] = 0.0f;
		m_Data[6] = 0.0f;
		m_Data[7] = 0.0f;
		m_Data[8] = 1.0f;
	}
	float Matrix3::Determinant() const {
		return m_Data[0] * m_Data[4] * m_Data[8] - m_Data[0] * m_Data[5] * m_Data[7] +
			m_Data[1] * m_Data[5] * m_Data[6] - m_Data[1] * m_Data[3] * m_Data[8] +
			m_Data[2] * m_Data[3] * m_Data[7] - m_Data[2] * m_Data[4] * m_Data[6];
	}
	bool Matrix3::Invert() {
		float temp[9];
		temp[0] = m_Data[4] * m_Data[8] - m_Data[5] * m_Data[7];
		temp[3] = m_Data[5] * m_Data[6] - m_Data[3] * m_Data[8];
		temp[6] = m_Data[3] * m_Data[7] - m_Data[4] * m_Data[6];

		float determinant = m_Data[0] * temp[0] + m_Data[1] * temp[3] + m_Data[2] * temp[6];

		if (determinant > 0.0f) {
			temp[1] = m_Data[7] * m_Data[2] - m_Data[8] * m_Data[1];
			temp[2] = m_Data[1] * m_Data[5] - m_Data[2] * m_Data[4];

			temp[4] = m_Data[0] * m_Data[8] - m_Data[2] * m_Data[6];
			temp[5] = m_Data[2] * m_Data[3] - m_Data[0] * m_Data[5];

			temp[7] = m_Data[6] * m_Data[1] - m_Data[7] * m_Data[0];
			temp[8] = m_Data[0] * m_Data[4] - m_Data[1] * m_Data[3];

			// divide by the determinant
			float invDeterminant = 1.0f / determinant;
			m_Data[0] = invDeterminant * temp[0];
			m_Data[1] = invDeterminant * temp[1];
			m_Data[2] = invDeterminant * temp[2];
			m_Data[3] = invDeterminant * temp[3];
			m_Data[4] = invDeterminant * temp[4];
			m_Data[5] = invDeterminant * temp[5];
			m_Data[6] = invDeterminant * temp[6];
			m_Data[7] = invDeterminant * temp[7];
			m_Data[8] = invDeterminant * temp[8];
			return true;
		}
		return false;
	}
	void Matrix3::Transpose() {
		std::swap(m_Data[1], m_Data[3]);
		std::swap(m_Data[2], m_Data[6]);
		std::swap(m_Data[5], m_Data[7]);
	}

	void Matrix3::SetTranslation(const Vector2& vector) {
		m_Data[6] = vector[0];
		m_Data[7] = vector[1];
	}
	void Matrix3::Translate(const Vector2& vector) {
		m_Data[6] += vector[0];
		m_Data[7] += vector[1];
	}

	//void Matrix3::SetRotation(const Quaternion& rotation) {}
	void Matrix3::SetAngleAxis(const Angle& angle, const float* axis) {
		float rad = angle.ToRadians();
		float c = Math::Cos(rad);
		float s = Math::Sin(rad);
		float omc = 1.0f - c;

		m_Data[0] = axis[0] * axis[0] * omc + c;
		m_Data[1] = axis[1] * axis[0] * omc + axis[2] * s;
		m_Data[2] = axis[2] * axis[0] * omc - axis[1] * s;

		m_Data[3] = axis[0] * axis[1] * omc - axis[2] * s;
		m_Data[4] = axis[1] * axis[1] * omc + c;
		m_Data[5] = axis[2] * axis[1] * omc + axis[0] * s;

		m_Data[6] = axis[0] * axis[2] * omc + axis[1] * s;
		m_Data[7] = axis[1] * axis[2] * omc - axis[0] * s;
		m_Data[8] = axis[2] * axis[2] * omc + c;
	}
	void Matrix3::SetRotationXYZ(const Angle& x, const Angle& y, const Angle& z) {
		float cos_x = Math::Cos(x.ToRadians());
		float sin_x = Math::Sin(x.ToRadians());

		float cos_y = Math::Cos(y.ToRadians());
		float sin_y = Math::Sin(y.ToRadians());

		float cos_z = Math::Cos(z.ToRadians());
		float sin_z = Math::Sin(z.ToRadians());

		m_Data[0] = cos_y * cos_z;
		m_Data[1] = -cos_y * sin_z;
		m_Data[2] = sin_y;

		m_Data[3] = sin_x * sin_y * cos_z + cos_x * sin_z;
		m_Data[4] = cos_x * cos_z - sin_x * sin_y * sin_z;
		m_Data[5] = -sin_x * cos_y;

		m_Data[6] = -cos_x * sin_y * cos_z + sin_x * sin_z;
		m_Data[7] = cos_x * sin_y * sin_z + sin_x * cos_z;
		m_Data[8] = cos_x * cos_y;
	}
	void Matrix3::SetRotationX(const Angle& angle) {
		float cos_gamma = Math::Cos(angle.ToRadians());
		float sin_gamma = Math::Sin(angle.ToRadians());

		m_Data[0] = 1.0f;

		m_Data[4] = cos_gamma;
		m_Data[5] = sin_gamma;

		m_Data[7] = -sin_gamma;
		m_Data[8] = cos_gamma;
	}
	void Matrix3::SetRotationY(const Angle& angle) {
		float cos_beta = Math::Cos(angle.ToRadians());
		float sin_beta = Math::Sin(angle.ToRadians());

		m_Data[0] = cos_beta;
		m_Data[6] = sin_beta;

		m_Data[4] = 1.0f;

		m_Data[2] = -sin_beta;
		m_Data[8] = cos_beta;
	}
	void Matrix3::SetRotationZ(const Angle& angle) {
		float cos_alpha = Math::Cos(angle.ToRadians());
		float sin_alpha = Math::Sin(angle.ToRadians());

		m_Data[0] = cos_alpha;
		m_Data[3] = -sin_alpha;

		m_Data[1] = sin_alpha;
		m_Data[4] = cos_alpha;
	}
	void Matrix3::SetLookAt(const Vector3& position, const Vector3& target, const Vector3& upwards) {
		VORTEX_ASSERT_MSG(false, "Not tested.")
		// compute forward vector and normalize
		Vector3 forward{target - position};
		forward.Normalize();

		Vector3 up;             // up vector of object
		Vector3 left;           // left vector of object

		// compute left vector
		left = Vector3::Cross(upwards, left);
		left.Normalize();

		// re-compute up vector
		up = Vector3::Cross(forward, left);
		up.Normalize();

		m_Data[0] = left.x();
		m_Data[1] = left.y();
		m_Data[2] = left.z();

		m_Data[3] = up.x();
		m_Data[4] = up.y();
		m_Data[5] = up.z();

		m_Data[6] = forward.x();
		m_Data[7] = forward.y();
		m_Data[8] = forward.z();
	}

	void Matrix3::Scale(const Vector3& vector) {
		m_Data[0] *= vector[0];
		m_Data[4] *= vector[1];
		m_Data[8] *= vector[2];
	}

	void Matrix3::ExtractTranslation(Vector2& vector) const {
		vector[0] = m_Data[6];
		vector[1] = m_Data[7];
	}
	//bool ExtractRotation(Quaternion& angle) const;

	void Matrix3::TransformPosition(Vector2& vector) const {
		float temp[2];
		temp[0] = m_Data[0] * vector[0] + m_Data[3] * vector[1] + m_Data[6];
		temp[1] = m_Data[1] * vector[0] + m_Data[4] * vector[1] + m_Data[7];
		vector.Set(temp[0], temp[1]);
	}
	void Matrix3::TransformDirection(Vector2& vector) const {
		float temp[2];
		temp[0] = m_Data[0] * vector[0] + m_Data[3] * vector[1];
		temp[1] = m_Data[1] * vector[0] + m_Data[4] * vector[1];
		vector.Set(temp[0], temp[1]);
	}
}

namespace Vortex {
	Vector4 Matrix4::operator*(const Vector4& rhs) const {
		// |  0  4  8  12 |   | 0 |
		// |  1  5  9  13 | * | 1 |
		// |  2  6 10  14 |   | 2 |
		// |  3  7 11  15 |   | 3 |
		return Vector4{
			m_Data[0] * rhs[0] + m_Data[4] * rhs[1] + m_Data[8] * rhs[2] + m_Data[12] * rhs[3]
			, m_Data[1] * rhs[0] + m_Data[5] * rhs[1] + m_Data[9] * rhs[2] + m_Data[13] * rhs[3]
			, m_Data[2] * rhs[0] + m_Data[6] * rhs[1] + m_Data[10] * rhs[2] + m_Data[14] * rhs[3]
			, m_Data[3] * rhs[0] + m_Data[7] * rhs[1] + m_Data[11] * rhs[2] + m_Data[15] * rhs[3]
		};
	}
	Matrix4 Matrix4::operator*(const Matrix4& rhs) const {
		// |  0  4  8 12 |   |  0  4  8 12 |
		// |  1  5  9 13 | * |  1  5  9 13 |
		// |  2  6 10 14 |   |  2  6 10 14 |
		// |  3  7 11 15 |   |  3  7 11 15 |
		return Matrix4{
			m_Data[0] * rhs[0] + m_Data[4] * rhs[1] + m_Data[8] * rhs[2] + m_Data[12] * rhs[3]
			, m_Data[1] * rhs[0] + m_Data[5] * rhs[1] + m_Data[9] * rhs[2] + m_Data[13] * rhs[3]
			, m_Data[2] * rhs[0] + m_Data[6] * rhs[1] + m_Data[10] * rhs[2] + m_Data[14] * rhs[3]
			, m_Data[3] * rhs[0] + m_Data[7] * rhs[1] + m_Data[11] * rhs[2] + m_Data[15] * rhs[3]
			, m_Data[0] * rhs[4] + m_Data[4] * rhs[5] + m_Data[8] * rhs[6] + m_Data[12] * rhs[7]
			, m_Data[1] * rhs[4] + m_Data[5] * rhs[5] + m_Data[9] * rhs[6] + m_Data[13] * rhs[7]
			, m_Data[2] * rhs[4] + m_Data[6] * rhs[5] + m_Data[10] * rhs[6] + m_Data[14] * rhs[7]
			, m_Data[3] * rhs[4] + m_Data[7] * rhs[5] + m_Data[11] * rhs[6] + m_Data[15] * rhs[7]
			, m_Data[0] * rhs[8] + m_Data[4] * rhs[9] + m_Data[8] * rhs[10] + m_Data[12] * rhs[11]
			, m_Data[1] * rhs[8] + m_Data[5] * rhs[9] + m_Data[9] * rhs[10] + m_Data[13] * rhs[11]
			, m_Data[2] * rhs[8] + m_Data[6] * rhs[9] + m_Data[10] * rhs[10] + m_Data[14] * rhs[11]
			, m_Data[3] * rhs[8] + m_Data[7] * rhs[9] + m_Data[11] * rhs[10] + m_Data[15] * rhs[11]
			, m_Data[0] * rhs[12] + m_Data[4] * rhs[13] + m_Data[8] * rhs[14] + m_Data[12] * rhs[15]
			, m_Data[1] * rhs[12] + m_Data[5] * rhs[13] + m_Data[9] * rhs[14] + m_Data[13] * rhs[15]
			, m_Data[2] * rhs[12] + m_Data[6] * rhs[13] + m_Data[10] * rhs[14] + m_Data[14] * rhs[15]
			, m_Data[3] * rhs[12] + m_Data[7] * rhs[13] + m_Data[11] * rhs[14] + m_Data[15] * rhs[15]
		};
	}
	Matrix4& Matrix4::operator*=(const Matrix4& rhs) {
		float temp[16];
		temp[0] = m_Data[0] * rhs[0] + m_Data[4] * rhs[1] + m_Data[8] * rhs[2] + m_Data[12] * rhs[3];
		temp[1] = m_Data[1] * rhs[0] + m_Data[5] * rhs[1] + m_Data[9] * rhs[2] + m_Data[13] * rhs[3];
		temp[2] = m_Data[2] * rhs[0] + m_Data[6] * rhs[1] + m_Data[10] * rhs[2] + m_Data[14] * rhs[3];
		temp[3] = m_Data[3] * rhs[0] + m_Data[7] * rhs[1] + m_Data[11] * rhs[2] + m_Data[15] * rhs[3];
		temp[4] = m_Data[0] * rhs[4] + m_Data[4] * rhs[5] + m_Data[8] * rhs[6] + m_Data[12] * rhs[7];
		temp[5] = m_Data[1] * rhs[4] + m_Data[5] * rhs[5] + m_Data[9] * rhs[6] + m_Data[13] * rhs[7];
		temp[6] = m_Data[2] * rhs[4] + m_Data[6] * rhs[5] + m_Data[10] * rhs[6] + m_Data[14] * rhs[7];
		temp[7] = m_Data[3] * rhs[4] + m_Data[7] * rhs[5] + m_Data[11] * rhs[6] + m_Data[15] * rhs[7];
		temp[8] = m_Data[0] * rhs[8] + m_Data[4] * rhs[9] + m_Data[8] * rhs[10] + m_Data[12] * rhs[11];
		temp[9] = m_Data[1] * rhs[8] + m_Data[5] * rhs[9] + m_Data[9] * rhs[10] + m_Data[13] * rhs[11];
		temp[10] = m_Data[2] * rhs[8] + m_Data[6] * rhs[9] + m_Data[10] * rhs[10] + m_Data[14] * rhs[11];
		temp[11] = m_Data[3] * rhs[8] + m_Data[7] * rhs[9] + m_Data[11] * rhs[10] + m_Data[15] * rhs[11];
		temp[12] = m_Data[0] * rhs[12] + m_Data[4] * rhs[13] + m_Data[8] * rhs[14] + m_Data[12] * rhs[15];
		temp[13] = m_Data[1] * rhs[12] + m_Data[5] * rhs[13] + m_Data[9] * rhs[14] + m_Data[13] * rhs[15];
		temp[14] = m_Data[2] * rhs[12] + m_Data[6] * rhs[13] + m_Data[10] * rhs[14] + m_Data[14] * rhs[15];
		temp[15] = m_Data[3] * rhs[12] + m_Data[7] * rhs[13] + m_Data[11] * rhs[14] + m_Data[15] * rhs[15];
		std::memcpy(m_Data, temp, 16 * sizeof(float));
		return *this;
	}

	void Matrix4::Identity() {
		m_Data[0] = 1.0f;
		m_Data[1] = 0.0f;
		m_Data[2] = 0.0f;
		m_Data[3] = 0.0f;
		m_Data[4] = 0.0f;
		m_Data[5] = 1.0f;
		m_Data[6] = 0.0f;
		m_Data[7] = 0.0f;
		m_Data[8] = 0.0f;
		m_Data[9] = 0.0f;
		m_Data[10] = 1.0f;
		m_Data[11] = 0.0f;
		m_Data[12] = 0.0f;
		m_Data[13] = 0.0f;
		m_Data[14] = 0.0f;
		m_Data[15] = 1.0f;
	}
	float Matrix4::Determinant() const {
		VORTEX_ASSERT(false)
		return 0;
	}
	bool Matrix4::Invert() {
		// John Carmack Implementation of 4x4 Matrix Inversion. (2011 - Doom 3 GPL Source Code)
		float det2_01_01 = m_Data[0] * m_Data[5] - m_Data[1] * m_Data[4];
		float det2_01_02 = m_Data[0] * m_Data[6] - m_Data[2] * m_Data[4];
		float det2_01_03 = m_Data[0] * m_Data[7] - m_Data[3] * m_Data[4];
		float det2_01_12 = m_Data[1] * m_Data[6] - m_Data[2] * m_Data[5];
		float det2_01_13 = m_Data[1] * m_Data[7] - m_Data[3] * m_Data[5];
		float det2_01_23 = m_Data[2] * m_Data[7] - m_Data[3] * m_Data[6];

		// 3x3 sub-determinants required to calculate 4x4 determinant
		float det3_201_012 = m_Data[8] * det2_01_12 - m_Data[9] * det2_01_02 + m_Data[10] * det2_01_01;
		float det3_201_013 = m_Data[8] * det2_01_13 - m_Data[9] * det2_01_03 + m_Data[11] * det2_01_01;
		float det3_201_023 = m_Data[8] * det2_01_23 - m_Data[10] * det2_01_03 + m_Data[11] * det2_01_02;
		float det3_201_123 = m_Data[9] * det2_01_23 - m_Data[10] * det2_01_13 + m_Data[11] * det2_01_12;

		float det = (-det3_201_123 * m_Data[12] + det3_201_023 * m_Data[13] - det3_201_013 * m_Data[14] + det3_201_012 * m_Data[15]);

		// remaining 2x2 sub-determinants
		float det2_03_01 = m_Data[0] * m_Data[13] - m_Data[1] * m_Data[12];
		float det2_03_02 = m_Data[0] * m_Data[14] - m_Data[2] * m_Data[12];
		float det2_03_03 = m_Data[0] * m_Data[15] - m_Data[3] * m_Data[12];
		float det2_03_12 = m_Data[1] * m_Data[14] - m_Data[2] * m_Data[13];
		float det2_03_13 = m_Data[1] * m_Data[15] - m_Data[3] * m_Data[13];
		float det2_03_23 = m_Data[2] * m_Data[15] - m_Data[3] * m_Data[14];

		float det2_13_01 = m_Data[4] * m_Data[13] - m_Data[5] * m_Data[12];
		float det2_13_02 = m_Data[4] * m_Data[14] - m_Data[6] * m_Data[12];
		float det2_13_03 = m_Data[4] * m_Data[15] - m_Data[7] * m_Data[12];
		float det2_13_12 = m_Data[5] * m_Data[14] - m_Data[6] * m_Data[13];
		float det2_13_13 = m_Data[5] * m_Data[15] - m_Data[7] * m_Data[13];
		float det2_13_23 = m_Data[6] * m_Data[15] - m_Data[7] * m_Data[14];

		// remaining 3x3 sub-determinants
		float det3_203_012 = m_Data[8] * det2_03_12 - m_Data[9] * det2_03_02 + m_Data[10] * det2_03_01;
		float det3_203_013 = m_Data[8] * det2_03_13 - m_Data[9] * det2_03_03 + m_Data[11] * det2_03_01;
		float det3_203_023 = m_Data[8] * det2_03_23 - m_Data[10] * det2_03_03 + m_Data[11] * det2_03_02;
		float det3_203_123 = m_Data[9] * det2_03_23 - m_Data[10] * det2_03_13 + m_Data[11] * det2_03_12;

		float det3_213_012 = m_Data[8] * det2_13_12 - m_Data[9] * det2_13_02 + m_Data[10] * det2_13_01;
		float det3_213_013 = m_Data[8] * det2_13_13 - m_Data[9] * det2_13_03 + m_Data[11] * det2_13_01;
		float det3_213_023 = m_Data[8] * det2_13_23 - m_Data[10] * det2_13_03 + m_Data[11] * det2_13_02;
		float det3_213_123 = m_Data[9] * det2_13_23 - m_Data[10] * det2_13_13 + m_Data[11] * det2_13_12;

		float det3_301_012 = m_Data[12] * det2_01_12 - m_Data[13] * det2_01_02 + m_Data[14] * det2_01_01;
		float det3_301_013 = m_Data[12] * det2_01_13 - m_Data[13] * det2_01_03 + m_Data[15] * det2_01_01;
		float det3_301_023 = m_Data[12] * det2_01_23 - m_Data[14] * det2_01_03 + m_Data[15] * det2_01_02;
		float det3_301_123 = m_Data[13] * det2_01_23 - m_Data[14] * det2_01_13 + m_Data[15] * det2_01_12;

		if (det > 0.0f) {
			float det_inv = 1.0f / det;
			m_Data[0] = -det3_213_123 * det_inv;
			m_Data[4] = +det3_213_023 * det_inv;
			m_Data[8] = -det3_213_013 * det_inv;
			m_Data[12] = +det3_213_012 * det_inv;

			m_Data[1] = +det3_203_123 * det_inv;
			m_Data[5] = -det3_203_023 * det_inv;
			m_Data[9] = +det3_203_013 * det_inv;
			m_Data[13] = -det3_203_012 * det_inv;

			m_Data[2] = +det3_301_123 * det_inv;
			m_Data[6] = -det3_301_023 * det_inv;
			m_Data[10] = +det3_301_013 * det_inv;
			m_Data[14] = -det3_301_012 * det_inv;

			m_Data[3] = -det3_201_123 * det_inv;
			m_Data[7] = +det3_201_023 * det_inv;
			m_Data[11] = -det3_201_013 * det_inv;
			m_Data[15] = +det3_201_012 * det_inv;
			return true;
		}
		return false;
	}
	void Matrix4::Transpose() {
		std::swap(m_Data[1], m_Data[4]);
		std::swap(m_Data[2], m_Data[8]);
		std::swap(m_Data[3], m_Data[12]);
		std::swap(m_Data[6], m_Data[9]);
		std::swap(m_Data[7], m_Data[13]);
		std::swap(m_Data[11], m_Data[14]);
	}

	void Matrix4::SetPerspective(Angle angle, float aspect_ratio, float near, float far) {
		float cot = 1.0f / Math::Tan(0.5f * angle.ToRadians());

		m_Data[0] = cot / aspect_ratio;
		m_Data[5] = cot;
		m_Data[10] = (near + far) / (near - far);

		m_Data[11] = -1.0f;
		m_Data[14] = (2.0f * far * near) / (near - far);
		m_Data[15] = 0.0f;
	}
	void Matrix4::SetOrthographic(float left, float right, float bottom, float top, float near, float far) {
		float dRL_inv = 1.0f / (right - left);
		float dTB_inv = 1.0f / (top - bottom);
		float dNF_inv = 1.0f / (near - far);

		m_Data[0] = 2.0f * dRL_inv;
		m_Data[5] = 2.0f * dTB_inv;
		m_Data[10] = 2.0f * dNF_inv;

		m_Data[12] = -(right + left) * dRL_inv;
		m_Data[13] = -(top + bottom) * dTB_inv;
		m_Data[14] = -(far + near) * dNF_inv;
		m_Data[15] = 1.0f;
	}

	void Matrix4::SetTranslation(const Vector3& vector) {
		m_Data[12] = vector[0];
		m_Data[13] = vector[1];
		m_Data[14] = vector[2];
	}
	void Matrix4::Translate(const Vector3& vector) {
		m_Data[12] += vector[0];
		m_Data[13] += vector[1];
		m_Data[14] += vector[2];
	}

	void Matrix4::SetAngleAxis(Angle angle, const Vector3& axis) {
		float rad = angle.ToRadians();
		float c = Math::Cos(rad);
		float s = Math::Sin(rad);
		float omc = 1.0f - c;

		m_Data[0] = axis[0] * axis[0] * omc + c;
		m_Data[1] = axis[1] * axis[0] * omc + axis[2] * s;
		m_Data[2] = axis[2] * axis[0] * omc - axis[1] * s;

		m_Data[4] = axis[0] * axis[1] * omc - axis[2] * s;
		m_Data[5] = axis[1] * axis[1] * omc + c;
		m_Data[6] = axis[2] * axis[1] * omc + axis[0] * s;

		m_Data[8] = axis[0] * axis[2] * omc + axis[1] * s;
		m_Data[9] = axis[1] * axis[2] * omc - axis[0] * s;
		m_Data[10] = axis[2] * axis[2] * omc + c;
	}
	//void Matrix4::SetRotation(const Quaternion& quaternion){}
	void Matrix4::SetRotationXYZ(Angle x, Angle y, Angle z) {
		float cos_x = Math::Cos(x.ToRadians());
		float sin_x = Math::Sin(x.ToRadians());

		float cos_y = Math::Cos(y.ToRadians());
		float sin_y = Math::Sin(y.ToRadians());

		float cos_z = Math::Cos(z.ToRadians());
		float sin_z = Math::Sin(z.ToRadians());

		m_Data[0] = cos_y * cos_z;
		m_Data[1] = -cos_y * sin_z;
		m_Data[2] = sin_y;

		m_Data[4] = sin_x * sin_y * cos_z + cos_x * sin_z;
		m_Data[5] = cos_x * cos_z - sin_x * sin_y * sin_z;
		m_Data[6] = -sin_x * cos_y;

		m_Data[8] = -cos_x * sin_y * cos_z + sin_x * sin_z;
		m_Data[9] = cos_x * sin_y * sin_z + sin_x * cos_z;
		m_Data[10] = cos_x * cos_y;
	}
	void Matrix4::SetRotationX(Angle angle) {
		float cos_gamma = Math::Cos(angle.ToRadians());
		float sin_gamma = Math::Sin(angle.ToRadians());

		m_Data[0] = 1.0f;

		m_Data[5] = cos_gamma;
		m_Data[6] = sin_gamma;

		m_Data[9] = -sin_gamma;
		m_Data[10] = cos_gamma;
	}
	void Matrix4::SetRotationY(Angle angle) {
		float cos_beta = Math::Cos(angle.ToRadians());
		float sin_beta = Math::Sin(angle.ToRadians());

		m_Data[0] = cos_beta;
		m_Data[2] = -sin_beta;

		m_Data[5] = 1.0f;

		m_Data[8] = sin_beta;
		m_Data[10] = cos_beta;
	}
	void Matrix4::SetRotationZ(Angle angle) {
		float cos_alpha = Math::Cos(angle.ToRadians());
		float sin_alpha = Math::Sin(angle.ToRadians());

		m_Data[0] = cos_alpha;
		m_Data[1] = sin_alpha;

		m_Data[4] = -sin_alpha;
		m_Data[5] = cos_alpha;
	}

	void Matrix4::SetLookAt(const Vector3& position, const Vector3& target, const Vector3& upwards) {
		VORTEX_ASSERT_MSG(false, "Not tested.")
	}

	void Matrix4::Scale(const Vector3& vector) {
		m_Data[0] *= vector[0];
		m_Data[5] *= vector[1];
		m_Data[10] *= vector[2];
	}

	void Matrix4::ExtractTranslation(Vector3& vector) const {
		vector[0] = m_Data[12];
		vector[1] = m_Data[13];
		vector[2] = m_Data[14];
	}
	//bool Matrix4::ExtractRotation(Quaternion& angle) const{}

	void Matrix4::TransformPosition(Vector3& vector) const {
		// |0  4  8  12|   |x|
		// |1  5  9  13|   |y|
		// |2  6 10  14| * |z|
		// |3  7 11  15|   |1|
		float temp[3];
		temp[0] = m_Data[0] * vector[0] + m_Data[4] * vector[1] + m_Data[8] * vector[2] + m_Data[12];
		temp[1] = m_Data[1] * vector[0] + m_Data[5] * vector[1] + m_Data[9] * vector[2] + m_Data[13];
		temp[2] = m_Data[2] * vector[0] + m_Data[6] * vector[1] + m_Data[10] * vector[2] + m_Data[14];

		vector.Set(temp[0], temp[1], temp[2]);
	}
	void Matrix4::TransformDirection(Vector3& vector) const {
		// |0  4  8  12|   |x|
		// |1  5  9  13|   |y|
		// |2  6 10  14| * |z|
		// |3  7 11  15|   |0|
		float temp[3];
		temp[0] = m_Data[0] * vector[0] + m_Data[4] * vector[1] + m_Data[8] * vector[2];
		temp[1] = m_Data[1] * vector[0] + m_Data[5] * vector[1] + m_Data[9] * vector[2];
		temp[2] = m_Data[2] * vector[0] + m_Data[6] * vector[1] + m_Data[10] * vector[2];
		vector.Set(temp[0], temp[1], temp[2]);
	}
}