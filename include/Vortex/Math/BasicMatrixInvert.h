#pragma once
#include "Vortex/Math/BasicMatrix.h"
#include "Vortex/Math/VortexMathFunctions.h"
#include "Vortex/Math/VortexMathConstants.h"

namespace Vortex::Math {
	template<>
	inline float BasicMatrix<float, 2, 2>::Determinant() const {
		return this->Data[0] * this->Data[3] - this->Data[1] * this->Data[2];
	}
	template<>
	inline bool BasicMatrix<float, 2, 2>::Invert() {
		float det = Determinant();

		if (Abs<float>(det) > Epsilon<float>()) {
			float inv_det = 1.0f / det;
			float temp = this->Data[0];

			this->Data[0] = inv_det * this->Data[3];
			this->Data[1] = -inv_det * this->Data[1];
			this->Data[2] = -inv_det * this->Data[2];
			this->Data[3] = inv_det * temp;
			return true;
		}
		return false;
	}

	template<>
	inline float BasicMatrix<float, 3, 3>::Determinant() const {
		return this->Data[0] * this->Data[4] * this->Data[8] - this->Data[0] * this->Data[5] * this->Data[7] +
			this->Data[1] * this->Data[5] * this->Data[6] - this->Data[1] * this->Data[3] * this->Data[8] +
			this->Data[2] * this->Data[3] * this->Data[7] - this->Data[2] * this->Data[4] * this->Data[6];
	}
	template<>
	inline bool BasicMatrix<float, 3, 3>::Invert() {
		float temp[9];
		temp[0] = this->Data[4] * this->Data[8] - this->Data[5] * this->Data[7];
		temp[3] = this->Data[5] * this->Data[6] - this->Data[3] * this->Data[8];
		temp[6] = this->Data[3] * this->Data[7] - this->Data[4] * this->Data[6];

		float determinant = this->Data[0] * temp[0] + this->Data[1] * temp[3] + this->Data[2] * temp[6];

		if (determinant > 0.0f) {
			temp[1] = this->Data[7] * this->Data[2] - this->Data[8] * this->Data[1];
			temp[2] = this->Data[1] * this->Data[5] - this->Data[2] * this->Data[4];

			temp[4] = this->Data[0] * this->Data[8] - this->Data[2] * this->Data[6];
			temp[5] = this->Data[2] * this->Data[3] - this->Data[0] * this->Data[5];

			temp[7] = this->Data[6] * this->Data[1] - this->Data[7] * this->Data[0];
			temp[8] = this->Data[0] * this->Data[4] - this->Data[1] * this->Data[3];

			// divide by the determinant
			float invDeterminant = 1.0f / determinant;
			this->Data[0] = invDeterminant * temp[0];
			this->Data[1] = invDeterminant * temp[1];
			this->Data[2] = invDeterminant * temp[2];
			this->Data[3] = invDeterminant * temp[3];
			this->Data[4] = invDeterminant * temp[4];
			this->Data[5] = invDeterminant * temp[5];
			this->Data[6] = invDeterminant * temp[6];
			this->Data[7] = invDeterminant * temp[7];
			this->Data[8] = invDeterminant * temp[8];
			return true;
		}
		return false;
	}

	template<>
	inline bool BasicMatrix<float, 4, 4>::Invert() {
		// John Carmack Implementation of 4x4 Matrix Inversion. (2011 - Doom 3 GPL Source Code)
		float det2_01_01 = this->Data[0] * this->Data[5] - this->Data[1] * this->Data[4];
		float det2_01_02 = this->Data[0] * this->Data[6] - this->Data[2] * this->Data[4];
		float det2_01_03 = this->Data[0] * this->Data[7] - this->Data[3] * this->Data[4];
		float det2_01_12 = this->Data[1] * this->Data[6] - this->Data[2] * this->Data[5];
		float det2_01_13 = this->Data[1] * this->Data[7] - this->Data[3] * this->Data[5];
		float det2_01_23 = this->Data[2] * this->Data[7] - this->Data[3] * this->Data[6];

		// 3x3 sub-determinants required to calculate 4x4 determinant
		float det3_201_012 = this->Data[8] * det2_01_12 - this->Data[9] * det2_01_02 + this->Data[10] * det2_01_01;
		float det3_201_013 = this->Data[8] * det2_01_13 - this->Data[9] * det2_01_03 + this->Data[11] * det2_01_01;
		float det3_201_023 = this->Data[8] * det2_01_23 - this->Data[10] * det2_01_03 + this->Data[11] * det2_01_02;
		float det3_201_123 = this->Data[9] * det2_01_23 - this->Data[10] * det2_01_13 + this->Data[11] * det2_01_12;

		float det = (-det3_201_123 * this->Data[12] + det3_201_023 * this->Data[13] - det3_201_013 * this->Data[14] + det3_201_012 * this->Data[15]);

		// remaining 2x2 sub-determinants
		float det2_03_01 = this->Data[0] * this->Data[13] - this->Data[1] * this->Data[12];
		float det2_03_02 = this->Data[0] * this->Data[14] - this->Data[2] * this->Data[12];
		float det2_03_03 = this->Data[0] * this->Data[15] - this->Data[3] * this->Data[12];
		float det2_03_12 = this->Data[1] * this->Data[14] - this->Data[2] * this->Data[13];
		float det2_03_13 = this->Data[1] * this->Data[15] - this->Data[3] * this->Data[13];
		float det2_03_23 = this->Data[2] * this->Data[15] - this->Data[3] * this->Data[14];

		float det2_13_01 = this->Data[4] * this->Data[13] - this->Data[5] * this->Data[12];
		float det2_13_02 = this->Data[4] * this->Data[14] - this->Data[6] * this->Data[12];
		float det2_13_03 = this->Data[4] * this->Data[15] - this->Data[7] * this->Data[12];
		float det2_13_12 = this->Data[5] * this->Data[14] - this->Data[6] * this->Data[13];
		float det2_13_13 = this->Data[5] * this->Data[15] - this->Data[7] * this->Data[13];
		float det2_13_23 = this->Data[6] * this->Data[15] - this->Data[7] * this->Data[14];

		// remaining 3x3 sub-determinants
		float det3_203_012 = this->Data[8] * det2_03_12 - this->Data[9] * det2_03_02 + this->Data[10] * det2_03_01;
		float det3_203_013 = this->Data[8] * det2_03_13 - this->Data[9] * det2_03_03 + this->Data[11] * det2_03_01;
		float det3_203_023 = this->Data[8] * det2_03_23 - this->Data[10] * det2_03_03 + this->Data[11] * det2_03_02;
		float det3_203_123 = this->Data[9] * det2_03_23 - this->Data[10] * det2_03_13 + this->Data[11] * det2_03_12;

		float det3_213_012 = this->Data[8] * det2_13_12 - this->Data[9] * det2_13_02 + this->Data[10] * det2_13_01;
		float det3_213_013 = this->Data[8] * det2_13_13 - this->Data[9] * det2_13_03 + this->Data[11] * det2_13_01;
		float det3_213_023 = this->Data[8] * det2_13_23 - this->Data[10] * det2_13_03 + this->Data[11] * det2_13_02;
		float det3_213_123 = this->Data[9] * det2_13_23 - this->Data[10] * det2_13_13 + this->Data[11] * det2_13_12;

		float det3_301_012 = this->Data[12] * det2_01_12 - this->Data[13] * det2_01_02 + this->Data[14] * det2_01_01;
		float det3_301_013 = this->Data[12] * det2_01_13 - this->Data[13] * det2_01_03 + this->Data[15] * det2_01_01;
		float det3_301_023 = this->Data[12] * det2_01_23 - this->Data[14] * det2_01_03 + this->Data[15] * det2_01_02;
		float det3_301_123 = this->Data[13] * det2_01_23 - this->Data[14] * det2_01_13 + this->Data[15] * det2_01_12;

		if (det > 0.0f) {
			float det_inv = 1.0f / det;
			this->Data[0] = -det3_213_123 * det_inv;
			this->Data[4] = +det3_213_023 * det_inv;
			this->Data[8] = -det3_213_013 * det_inv;
			this->Data[12] = +det3_213_012 * det_inv;

			this->Data[1] = +det3_203_123 * det_inv;
			this->Data[5] = -det3_203_023 * det_inv;
			this->Data[9] = +det3_203_013 * det_inv;
			this->Data[13] = -det3_203_012 * det_inv;

			this->Data[2] = +det3_301_123 * det_inv;
			this->Data[6] = -det3_301_023 * det_inv;
			this->Data[10] = +det3_301_013 * det_inv;
			this->Data[14] = -det3_301_012 * det_inv;

			this->Data[3] = -det3_201_123 * det_inv;
			this->Data[7] = +det3_201_023 * det_inv;
			this->Data[11] = -det3_201_013 * det_inv;
			this->Data[15] = +det3_201_012 * det_inv;
			return true;
		}
		return false;
	}
}