#pragma once
#include "Vortex/Debug/Assert.h"
#include "Vortex/Math/BasicAngle.h"
#include "Vortex/Math/BasicVector.h"
#include "Vortex/Math/VortexMathFunctions.h"

namespace Vortex::Math {
	template<typename T, SizeType Rows, SizeType Columns>
	struct BasicMatrixBase {
		T Data[Rows * Columns];
	};

	// Column-Major ordered 2x2 matrix class.
	// |0  2| |00  01|
	// |1  3| |10  11|
	template<typename T>
	struct BasicMatrixBase<T, 2, 2> {
		union {
			T Data[2 * 2];
			struct {
				T m00;
				T m10;

				T m01;
				T m11;
			};
		};
	};

	// Column-Major ordered 3x3 matrix class.
	// |0  3  6| |00  01  02|
	// |1  4  7| |10  11  12|
	// |2  5  8| |20  21  22|
	template<typename T>
	struct BasicMatrixBase<T, 3, 3> {
		union {
			T Data[3 * 3];
			struct {
				T m00;
				T m10;
				T m20;

				T m01;
				T m11;
				T m21;

				T m02;
				T m12;
				T m22;
			};
		};
	};

	// Column-Major ordered 3x3 matrix class.
	// |0  4  8   12| |00  01  02  03|
	// |1  5  9   13| |10  11  12  13|
	// |2  6  10  14| |20  21  22  23|
	// |3  7  11  15| |30  31  32  33|
	template<typename T>
	struct BasicMatrixBase<T, 4, 4> {
		union {
			T Data[4 * 4];
			struct {
				T m00;
				T m10;
				T m20;
				T m30;

				T m01;
				T m11;
				T m21;
				T m31;

				T m02;
				T m12;
				T m22;
				T m32;

				T m03;
				T m13;
				T m23;
				T m33;
			};
		};
	};

	//	Basic NxM matrix class.
	template<typename T, SizeType Rows, SizeType Columns>
	struct BasicMatrix: BasicMatrixBase<T, Rows, Columns> {
		using MatrixType = BasicMatrix<T, Rows, Columns>;
		using BaseType = BasicMatrixBase<T, Rows, Columns>;
		using UnderlyingType = T;
		constexpr static SizeType Length() { return Rows * Columns; }
		constexpr static SizeType ColumnOffset() { return Rows; }
		constexpr static SizeType ColumnMajorIndex(SizeType row, SizeType column) {
			return row + column * ColumnOffset();
		}

	private:
		template<bool A>
		using EIF = typename std::enable_if_t<A>;

	public:
		constexpr UnderlyingType& operator[](Vortex::SizeType i) {
			VORTEX_ASSERT(i < Length())
			return this->Data[i];
		}
		constexpr const UnderlyingType& operator[](Vortex::SizeType i) const {
			VORTEX_ASSERT(i < Length())
			return this->Data[i];
		}
		constexpr UnderlyingType& operator()(SizeType row, SizeType column) {
			VORTEX_ASSERT(ColumnMajorIndex(row, column) < Length())
			return this->Data[ColumnMajorIndex(row, column)];
		}
		constexpr const UnderlyingType& operator()(SizeType row, SizeType column) const {
			VORTEX_ASSERT(ColumnMajorIndex(row, column) < Length())
			return this->Data[ColumnMajorIndex(row, column)];
		}

	public: // ======= General =======
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ == Columns_ && std::is_arithmetic_v<T_>)>
		>
		constexpr static MatrixType Identity() {
			MatrixType out{0};
			for (SizeType i = 0; i < Columns_; ++i) {
				out(i, i) = T_{1};
			}
			return out;
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ == Columns_ && std::is_arithmetic_v<T_>)>
		>
		constexpr void SetIdentity() {
			for (SizeType i = 0; i < Length(); ++i) {
				this->Data[i] = T_{0};
			}
			for (SizeType i = 0; i < Rows_; ++i) {
				for (int j = 0; j < Columns_; ++j) {
					if (i == j) {
						operator()(i, j) = T_{1};
					} else {
						operator()(i, j) = T_{0};
					}
				}
			}
		}

	public: // ======= Translations =======
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		constexpr void Set2DTranslation(T_ x, T_ y) {
			// |00  01 [02] . |
			// |10  11 [12] . |
			// |.   .   .   . |
			this->operator()(0, 2) = x;
			this->operator()(1, 2) = y;
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		constexpr void Set2DTranslation(const BasicVector<T_, Rows_>& vector) {
			// |00  01 [02] . |
			// |10  11 [12] . |
			// |.   .   .   . |
			this->operator()(0, 2) = vector[0];
			this->operator()(1, 2) = vector[1];
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		constexpr void Translate2D(T_ x, T_ y) {
			// |00  01 [02] . |
			// |10  11 [12] . |
			// |.   .   .   . |
			this->operator()(0, 2) += x;
			this->operator()(1, 2) += y;
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		constexpr void Translate2DTranslate2D(const BasicVector<T_, Rows_>& vector) {
			// |00  01 [02] . |
			// |10  11 [12] . |
			// |.   .   .   . |
			this->operator()(0, 2) += vector[0];
			this->operator()(1, 2) += vector[1];
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		constexpr void Extract2DTranslation(BasicVector<T_, 2>& vector) {
			// |00  01 [02] . |
			// |10  11 [12] . |
			// |.   .   .   . |
			vector[0] = this->operator()(0, 2);
			vector[1] = this->operator()(1, 2);
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		constexpr void Set3DTranslation(T_ x, T_ y, T_ z) {
			// |00  01  02 [03] . |
			// |10  11  12 [13] . |
			// |20  21  22 [23] . |
			// |.   .   .   .   . |
			this->operator()(0, 3) = x;
			this->operator()(1, 3) = y;
			this->operator()(2, 3) = z;
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		constexpr void Set3DTranslation(const BasicVector<T_, Rows_>& vector) {
			// |00  01  02 [03] . |
			// |10  11  12 [13] . |
			// |20  21  22 [23] . |
			// |.   .   .   .   . |
			this->operator()(0, 3) = vector[0];
			this->operator()(1, 3) = vector[1];
			this->operator()(2, 3) = vector[2];
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		constexpr void Translate3D(T_ x, T_ y, T_ z) {
			// |00  01  02 [03] . |
			// |10  11  12 [13] . |
			// |20  21  22 [23] . |
			// |.   .   .   .   . |
			this->operator()(0, 3) += x;
			this->operator()(1, 3) += y;
			this->operator()(2, 3) += z;
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		constexpr void Translate3D(const BasicVector<T_, Rows_>& vector) {
			// |00  01  02 [03] . |
			// |10  11  12 [13] . |
			// |20  21  22 [23] . |
			// |.   .   .   .   . |
			this->operator()(0, 3) += vector[0];
			this->operator()(1, 3) += vector[1];
			this->operator()(2, 3) += vector[2];
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		constexpr void Extract3DTranslation(BasicVector<T_, 3>& vector) {
			// |00  01  02 [03] . |
			// |10  11  12 [13] . |
			// |20  21  22 [23] . |
			// |.   .   .   .   . |
			vector[0] = this->operator()(0, 3);
			vector[1] = this->operator()(1, 3);
			vector[2] = this->operator()(2, 3);
		}

	public: // ======= Rotations =======
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		inline void SetRotationX(const BasicAngle<T_>& angle) {
			float cos_gamma = Cos(angle.ToRadians());
			float sin_gamma = Sin(angle.ToRadians());

			// |[00]  01   02  . |
			// | 10  [11] [12] . |
			// | 20  [21] [22] . |
			// | .    .    .   . |
			this->operator()(0, 0) = T_{1};

			this->operator()(1, 1) = cos_gamma;
			this->operator()(2, 1) = sin_gamma;

			this->operator()(1, 2) = -sin_gamma;
			this->operator()(2, 2) = cos_gamma;
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		inline void SetRotationY(const BasicAngle<T_>& angle) {
			T_ cos_beta = Cos<T_>(angle.ToRadians());
			T_ sin_beta = Sin<T_>(angle.ToRadians());

			// |[00]  01  [02] . |
			// | 10  [11]  12  . |
			// |[20]  21  [22] . |
			// | .    .    .   . |
			this->operator()(0, 0) = cos_beta;
			this->operator()(0, 2) = -sin_beta;

			this->operator()(1, 1) = T_{1};

			this->operator()(2, 0) = -sin_beta;
			this->operator()(2, 2) = cos_beta;
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 2 && std::is_floating_point_v<T_>)>
		>
		inline void SetRotationZ(const BasicAngle<T_>& angle) {
			T_ cos_alpha = Cos<T_>(angle.ToRadians());
			T_ sin_alpha = Sin<T_>(angle.ToRadians());

			// |[00] [01]  .  |
			// |[10] [11]  .  |
			// | .    .    .  |
			this->operator()(0, 0) = cos_alpha;
			this->operator()(0, 1) = -sin_alpha;

			this->operator()(1, 0) = sin_alpha;
			this->operator()(1, 1) = cos_alpha;
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		inline void SetRotationXYZ(
			const BasicAngle<T_>& x,
			const BasicAngle<T_>& y,
			const BasicAngle<T_>& z
		) {
			T_ cos_x = Cos<T_>(x.ToRadians());
			T_ sin_x = Sin<T_>(x.ToRadians());

			T_ cos_y = Cos<T_>(y.ToRadians());
			T_ sin_y = Sin<T_>(y.ToRadians());

			T_ cos_z = Cos<T_>(z.ToRadians());
			T_ sin_z = Sin<T_>(z.ToRadians());

			// |[00] [01] [02] . |
			// |[10] [11] [12] . |
			// |[20] [21] [22] . |
			// | .    .    .   . |
			this->operator()(0, 0) = cos_y * cos_z;
			this->operator()(1, 0) = -cos_y * sin_z;
			this->operator()(2, 0) = sin_y;

			this->operator()(0, 1) = sin_x * sin_y * cos_z + cos_x * sin_z;
			this->operator()(1, 1) = cos_x * cos_z - sin_x * sin_y * sin_z;
			this->operator()(2, 1) = -sin_x * cos_y;

			this->operator()(0, 2) = -cos_x * sin_y * cos_z + sin_x * sin_z;
			this->operator()(1, 2) = cos_x * sin_y * sin_z + sin_x * cos_z;
			this->operator()(2, 2) = cos_x * cos_y;
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		inline void SetAxisAngle(const BasicVector<T_, 3>& axis, const BasicAngle<T_> angle) {
			auto c = Cos<T_>(angle.ToRadians());
			auto s = Sin<T_>(angle.ToRadians());
			auto omc = T_{1} - c;

			// |[00] [01] [02] . |
			// |[10] [11] [12] . |
			// |[20] [21] [22] . |
			// | .    .    .   . |
			this->operator()(0, 0) = axis[0] * axis[0] * omc + c;
			this->operator()(1, 0) = axis[1] * axis[0] * omc + axis[2] * s;
			this->operator()(2, 0) = axis[2] * axis[0] * omc - axis[1] * s;

			this->operator()(0, 1) = axis[0] * axis[1] * omc - axis[2] * s;
			this->operator()(1, 1) = axis[1] * axis[1] * omc + c;
			this->operator()(2, 1) = axis[2] * axis[1] * omc + axis[0] * s;

			this->operator()(0, 2) = axis[0] * axis[2] * omc + axis[1] * s;
			this->operator()(1, 2) = axis[1] * axis[2] * omc - axis[0] * s;
			this->operator()(2, 2) = axis[2] * axis[2] * omc + c;
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 2 && std::is_floating_point_v<T_>)>
		>
		bool ExtractXRotation(BasicAngle<T_>& angle) const {
			if (!Compare(this->Data[0], this->Data[3])
				|| !Compare(this->Data[1], -this->Data[2])) {
				return false;
			}
			angle.SetRadians(Acos(this->Data[0]));
			return true;
		}

	public: // ======= Scaling =======
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ >= 2 && Columns_ == Rows_ && std::is_floating_point_v<T_>)>
		>
		constexpr void Scale(BasicVector<T_, Rows_>& vector) const {
			// |[00] 01  02 |
			// | 10 [. ] .  |
			// | 20  .  [. ]|
			for (int i = 0; i < vector.Length(); ++i) {
				this->operator()(i, i) *= vector[i];
			}
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 2 && Columns_ == 2 && std::is_floating_point_v<T_>)>
		>
		constexpr void Scale(T_ x, T_ y) {
			// |[00]  01   . |
			// | 10  [11]  . |
			// | .    .    . |
			this->operator()(0, 0) *= x;
			this->operator()(1, 1) *= y;
		}

		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 3 && Columns_ >= 3 && std::is_floating_point_v<T_>)>
		>
		constexpr void Scale(T_ x, T_ y, T_ z) {
			// |[00]  01   02  . |
			// | 10  [11]  12  . |
			// | 20   21  [22] . |
			// | .    .    .   . |
			this->operator()(0, 0) *= x;
			this->operator()(1, 1) *= y;
			this->operator()(2, 2) *= z;
		}

	public: // ======= Transform =======
		//void TransformPosition(BasicVector<T_, 3>& vector) const;
		//void TransformDirection(BasicVector<T_, 2>& vector) const;

	public:
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 4 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		inline void SetPerspective(const BasicAngle<T_>& fovy, T_ aspect_ratio, T_ near, T_ far) {
			// RHS
			// |[00]  01   02   03  . |
			// | 10  [11]  12   13  . |
			// | 20   21  [22] [23] . |
			// | 30   31  [32] [33] . |
			// | .    .    .    .   . |

			auto cot = T_{1} / (Tan<T_>(T_{0.5} * fovy.ToRadians()));

			this->operator()(0, 0) = aspect_ratio * cot;
			this->operator()(1, 1) = cot;
			this->operator()(2, 2) = (far + near) / (near - far);

			this->operator()(3, 2) = T_{-1};
			this->operator()(2, 3) = (T_{2} * far * near) / (near - far);
			this->operator()(3, 3) = T_{0};
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 4 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		inline void SetOrthographic(T_ left, T_ right, T_ bottom, T_ top, T_ near, T_ far) {
			// RHS
			// |[00]  01   02  [03] . |
			// | 10  [11]  12  [13] . |
			// | 20   21  [22] [23] . |
			// | 30   31   32  [33] . |
			// | .    .    .    .   . |

			auto dRL_inv = T_{1} / (right - left);
			auto dTB_inv = T_{1} / (top - bottom);
			auto dFN_inv = T_{1} / (far - near);

			this->operator()(0, 0) = T_{2} * dRL_inv;
			this->operator()(1, 1) = T_{2} * dTB_inv;
			this->operator()(2, 2) = T_{-2} * dFN_inv;

			this->operator()(0, 3) = -(right + left) * dRL_inv;
			this->operator()(1, 3) = -(top + bottom) * dTB_inv;
			this->operator()(2, 3) = -(far + near) * dFN_inv;
			this->operator()(3, 3) = T_{1};
		}
		template<
			typename T_ = UnderlyingType,
			SizeType Rows_ = Rows,
			SizeType Columns_ = Columns,
			typename = EIF<(Rows_ >= 4 && Columns_ >= 4 && std::is_floating_point_v<T_>)>
		>
		inline void SetLookat(
			const BasicVector<T_, 3>& eye,
			const BasicVector<T_, 3>& center,
			const BasicVector<T_, 3>& up) {
			// RHS
			// |[00] [01] [02] [03] . |
			// |[10] [11] [12] [13] . |
			// |[20] [21] [22] [23] . |
			// |[30] [31] [32] [33] . |
			// | .    .    .    .   . |

			// Compute direction from position to lookAt
			auto dir = eye - center;
			dir.Normalize();

			// left = up x direction cross product
			BasicVector<T_, 3> left = BasicVector<T_, 3>::Cross(up, dir);
			left.Normalize();

			// up = direction x left
			BasicVector<T_, 3> upn = BasicVector<T_, 3>::Cross(dir, left);

			this->operator()(0, 0) = left.x;
			this->operator()(1, 0) = upn.x;
			this->operator()(2, 0) = dir.x;
			this->operator()(3, 0) = 0.0f;

			this->operator()(0, 1) = left.y;
			this->operator()(1, 1) = upn.y;
			this->operator()(2, 1) = dir.y;
			this->operator()(3, 1) = 0.0f;

			this->operator()(0, 2) = left.z;
			this->operator()(1, 2) = upn.z;
			this->operator()(2, 2) = dir.z;
			this->operator()(3, 2) = 0.0f;

			this->operator()(0, 3) = -(left.x * eye.x + left.y * eye.y + left.z * eye.z);
			this->operator()(1, 3) = -(upn.x * eye.x + upn.y * eye.y + upn.z * eye.z);
			this->operator()(2, 3) = -(dir.x * eye.x + dir.y * eye.y + dir.z * eye.z);
			this->operator()(3, 3) = 1.0f;
		}

		float Determinant() const;
		bool Invert();
	};
}

template<typename T, Vortex::SizeType Rows, Vortex::SizeType Columns>
std::ostream& operator<<(
	std::ostream& stream,
	const Vortex::Math::BasicMatrix<T, Rows, Columns>& matrix
) {
	for (Vortex::SizeType i = 0; i < Rows; ++i) {
		stream << "| ";
		for (Vortex::SizeType j = 0; j < Columns; ++j) {
			stream << matrix(i, j) << " ";
		}
		stream << "|\n";
	}
	return stream;
}
