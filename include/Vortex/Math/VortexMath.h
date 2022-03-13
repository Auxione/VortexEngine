#pragma once
#include <cmath>

#include "Vortex/Memory/Memory.h"
#include "Vortex/Math/VortexMathConstants.h"

namespace Vortex::Math {
	static float Sin(float value) { return std::sinf(value); }
	static float Cos(float value) { return std::cosf(value); }
	static float Tan(float value) { return std::tanf(value); }
	static float Asin(float value) { return std::asinf(value); }
	static float Acos(float value) { return std::acosf(value); }
	static float Atan(float value) { return std::atanf(value); }
	static float Atan2(float y, float x) { return std::atan2f(y, x); }
	static float Sqrt(float value) { return std::sqrtf(value); }
	static float Abs(float value) { return std::fabs(value); }
	static float CopySign(float mag, float sign) {
		return std::copysignf(mag, sign);
	}

	static bool Compare(float value1, float value2, float epsilon = Math::Epsilon) {
		return Math::Abs(value1) - Math::Abs(value2) < epsilon;
	}

	inline static float Round(float value) { return std::roundf(value); }
	inline static float Ceil(float value) { return std::ceilf(value); }
	inline static float Floor(float value) { return std::floorf(value); }

	inline static Int32 RoundToInt(float value) { return static_cast<Int32>(std::round(value)); }
	inline static Int32 CeilToInt(float value) { return static_cast<Int32>(std::ceil(value)); }
	inline static Int32 FloorToInt(float value) { return static_cast<Int32>(std::floor(value)); }

	constexpr float Min(float v1, float v2) { return v1 > v2 ? v2 : v1; }
	constexpr float Max(float v1, float v2) { return v1 < v2 ? v2 : v1; }
	constexpr float Lerp(float start, float stop, float ratio) { return start + (stop - start) * ratio; }
	constexpr float Normalize(float value, float start, float stop) { return (value - start) / (stop - start); }
	constexpr float Map(float value, float value_min, float value_max, float output_min, float output_max) {
		return output_min + (output_max - output_min) * ((value - value_min) / (value_max - value_min));
	}
	constexpr float Clamp(float value, float min, float max) {
		if (value > max) {
			return max;
		} else if (value < min) {
			return min;
		} else {
			return value;
		}
	}

	template<typename T>
	constexpr bool IsEven(T value) {
		VORTEX_STATIC_ASSERT(std::is_integral<T>::value)
		return !(value & 1);
	}

	template<typename T>
	constexpr bool IsOdd(T value) { return IsEven(value); }
}

#include "Vortex/Math/BasicCircle.h"
#include "Vortex/Math/BasicRectangle.h"
#include "Vortex/Math/BasicTriangle.h"
#include "Vortex/Math/BasicLineSegment2.h"

#include "Vortex/Math/BasicAngle.h"
#include "Vortex/Math/BasicGUID.h"

namespace Vortex {
	using Circle = Vortex::BasicCircle<float>;
	using Rectangle = Vortex::BasicRectangle<float>;
	using RectangleInt = Vortex::BasicRectangle<Int32>;
	using Triangle = Vortex::BasicTriangle<float>;
	using LineSegment2 = Vortex::BasicLineSegment2<float>;
	using LineSegment2Int = Vortex::BasicLineSegment2<Int32>;

	using Angle = Vortex::BasicAngle<float>;
	using GUID = Vortex::BasicGUID<UInt64>;
}

#include "Vortex/Math/BasicVector.h"

namespace Vortex {
	///////////////////////////////////////////////////////////////////////////
	// Vector2
	///////////////////////////////////////////////////////////////////////////
	struct Vector2: public BasicVector<2, float> {
		using Vortex::BasicVector<2, float>::BasicVector;
		using Vortex::BasicVector<2, float>::operator=;

		using Vortex::BasicVector<2, float>::operator+;
		using Vortex::BasicVector<2, float>::operator-;
		using Vortex::BasicVector<2, float>::operator*;
		using Vortex::BasicVector<2, float>::operator/;

		using Vortex::BasicVector<2, float>::operator+=;
		using Vortex::BasicVector<2, float>::operator-=;
		using Vortex::BasicVector<2, float>::operator*=;
		using Vortex::BasicVector<2, float>::operator/=;

		using Vortex::BasicVector<2, float>::Zero;
		using Vortex::BasicVector<2, float>::Magnitude;
		using Vortex::BasicVector<2, float>::SqrMagnitude;
		using Vortex::BasicVector<2, float>::Normalize;
		using Vortex::BasicVector<2, float>::Dot;
		using Vortex::BasicVector<2, float>::Reflect;
		using Vortex::BasicVector<2, float>::Distance;

		constexpr Vector2(float x, float y): Vortex::BasicVector<2, float>() {
			m_Data[0] = x;
			m_Data[1] = y;
		}

		constexpr void Set(float x, float y) {
			m_Data[0] = x;
			m_Data[1] = y;
		}

		constexpr float& x() { return m_Data[0]; }
		constexpr float& y() { return m_Data[1]; }
		constexpr const float& x() const { return m_Data[0]; }
		constexpr const float& y() const { return m_Data[1]; }

		constexpr static Vector2 PerpendicularCW(const Vector2& vector) { return Vector2{vector.y(), -vector.x()}; }
		constexpr static Vector2 PerpendicularCCW(const Vector2& vector) { return Vector2{-vector.y(), vector.x()}; }

		constexpr static Vector2 Up() { return Vector2{0.0f, 1.0f}; };
		constexpr static Vector2 Down() { return Vector2{0.0f, -1.0f}; };
		constexpr static Vector2 Left() { return Vector2{-1.0f, 0.0f}; };
		constexpr static Vector2 Right() { return Vector2{1.0f, 0.0f}; };
	};

	///////////////////////////////////////////////////////////////////////////
	// Vector2Int
	///////////////////////////////////////////////////////////////////////////
	struct Vector2Int: public BasicVector<2, Int32> {
		using Vortex::BasicVector<2, Int32>::BasicVector;
		using Vortex::BasicVector<2, Int32>::operator=;

		using Vortex::BasicVector<2, Int32>::operator+;
		using Vortex::BasicVector<2, Int32>::operator-;
		using Vortex::BasicVector<2, Int32>::operator*;
		using Vortex::BasicVector<2, Int32>::operator/;

		using Vortex::BasicVector<2, Int32>::operator+=;
		using Vortex::BasicVector<2, Int32>::operator-=;
		using Vortex::BasicVector<2, Int32>::operator*=;
		using Vortex::BasicVector<2, Int32>::operator/=;

		using Vortex::BasicVector<2, Int32>::Zero;
		using Vortex::BasicVector<2, Int32>::Magnitude;
		using Vortex::BasicVector<2, Int32>::SqrMagnitude;
		using Vortex::BasicVector<2, Int32>::Normalize;
		using Vortex::BasicVector<2, Int32>::Dot;
		using Vortex::BasicVector<2, Int32>::Reflect;
		using Vortex::BasicVector<2, Int32>::Distance;

		constexpr Vector2Int(Int32 x, Int32 y): Vortex::BasicVector<2, Int32>() {
			m_Data[0] = x;
			m_Data[1] = y;
		}

		constexpr void Set(Int32 x, Int32 y) {
			m_Data[0] = x;
			m_Data[1] = y;
		}

		constexpr Int32& x() { return m_Data[0]; }
		constexpr Int32& y() { return m_Data[1]; }
		constexpr const Int32& x() const { return m_Data[0]; }
		constexpr const Int32& y() const { return m_Data[1]; }

		constexpr static Vector2Int PerpendicularCW(const Vector2Int& vector) { return {vector.y(), -vector.x()}; }
		constexpr static Vector2Int PerpendicularCCW(const Vector2Int& vector) { return {-vector.y(), vector.x()}; }

		constexpr static Vector2Int Up() { return Vector2Int{0, 1}; };
		constexpr static Vector2Int Down() { return Vector2Int{0, -1}; };
		constexpr static Vector2Int Left() { return Vector2Int{-1, 0}; };
		constexpr static Vector2Int Right() { return Vector2Int{1, 0}; };
	};

	///////////////////////////////////////////////////////////////////////////
	// Vector3
	///////////////////////////////////////////////////////////////////////////
	struct Vector3: public BasicVector<3, float> {
		using Vortex::BasicVector<3, float>::BasicVector;
		using Vortex::BasicVector<3, float>::operator=;

		using Vortex::BasicVector<3, float>::operator+;
		using Vortex::BasicVector<3, float>::operator-;
		using Vortex::BasicVector<3, float>::operator*;
		using Vortex::BasicVector<3, float>::operator/;

		using Vortex::BasicVector<3, float>::operator+=;
		using Vortex::BasicVector<3, float>::operator-=;
		using Vortex::BasicVector<3, float>::operator*=;
		using Vortex::BasicVector<3, float>::operator/=;

		using Vortex::BasicVector<3, float>::Zero;
		using Vortex::BasicVector<3, float>::Magnitude;
		using Vortex::BasicVector<3, float>::SqrMagnitude;
		using Vortex::BasicVector<3, float>::Normalize;
		using Vortex::BasicVector<3, float>::Dot;
		using Vortex::BasicVector<3, float>::Reflect;
		using Vortex::BasicVector<3, float>::Distance;

		constexpr Vector3(float x, float y, float z): Vortex::BasicVector<3, float>() {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
		}

		constexpr void Set(float x, float y, float z) {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
		}

		constexpr float& x() { return m_Data[0]; }
		constexpr float& y() { return m_Data[1]; }
		constexpr float& z() { return m_Data[2]; }
		constexpr const float& x() const { return m_Data[0]; }
		constexpr const float& y() const { return m_Data[1]; }
		constexpr const float& z() const { return m_Data[2]; }

		constexpr Vector2 xy() const { return {x(), y()}; }

		constexpr static Vector3 Forward() { return Vector3{0.0f, 0.0f, 1.0f}; };
		constexpr static Vector3 Back() { return Vector3{0.0f, 0, -1.0f}; };
		constexpr static Vector3 Up() { return Vector3{0.0f, 1.0f, 0.0f}; };
		constexpr static Vector3 Down() { return Vector3{0.0f, -1.0f, 0.0f}; };
		constexpr static Vector3 Left() { return Vector3{-1.0f, 0.0f, 0.0f}; };
		constexpr static Vector3 Right() { return Vector3{1.0f, 0.0f, 0.0f}; };
		constexpr static Vector3 Zero() { return Vector3{0.0f, 0.0f, 0.0f}; };

		constexpr static Vector3 Cross(const Vector3& lhs, const Vector3& rhs) {
			return {
				lhs.y() * rhs.z() - lhs.z() * rhs.y()
				, lhs.z() * rhs.x() - lhs.x() * rhs.z()
				, lhs.x() * rhs.y() - lhs.y() * rhs.x()
			};
		}
	};

	///////////////////////////////////////////////////////////////////////////
	// Vector3Int
	///////////////////////////////////////////////////////////////////////////
	struct Vector3Int: public BasicVector<3, Int32> {
		using Vortex::BasicVector<3, Int32>::BasicVector;
		using Vortex::BasicVector<3, Int32>::operator=;

		using Vortex::BasicVector<3, Int32>::operator+;
		using Vortex::BasicVector<3, Int32>::operator-;
		using Vortex::BasicVector<3, Int32>::operator*;
		using Vortex::BasicVector<3, Int32>::operator/;

		using Vortex::BasicVector<3, Int32>::operator+=;
		using Vortex::BasicVector<3, Int32>::operator-=;
		using Vortex::BasicVector<3, Int32>::operator*=;
		using Vortex::BasicVector<3, Int32>::operator/=;

		using Vortex::BasicVector<3, Int32>::Zero;
		using Vortex::BasicVector<3, Int32>::Magnitude;
		using Vortex::BasicVector<3, Int32>::SqrMagnitude;
		using Vortex::BasicVector<3, Int32>::Normalize;
		using Vortex::BasicVector<3, Int32>::Dot;
		using Vortex::BasicVector<3, Int32>::Reflect;
		using Vortex::BasicVector<3, Int32>::Distance;

		constexpr Vector3Int(Int32 x, Int32 y, Int32 z): Vortex::BasicVector<3, Int32>() {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
		}

		constexpr void Set(Int32 x, Int32 y, Int32 z) {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
		}

		constexpr Int32& x() { return m_Data[0]; }
		constexpr Int32& y() { return m_Data[1]; }
		constexpr Int32& z() { return m_Data[2]; }
		constexpr const Int32& x() const { return m_Data[0]; }
		constexpr const Int32& y() const { return m_Data[1]; }
		constexpr const Int32& z() const { return m_Data[2]; }

		constexpr Vector2Int xy() const { return {x(), y()}; }

		constexpr static Vector3Int Forward() { return Vector3Int{0, 0, 1}; };
		constexpr static Vector3Int Back() { return Vector3Int{0, 0, -1}; };
		constexpr static Vector3Int Up() { return Vector3Int{0, 1, 0}; };
		constexpr static Vector3Int Down() { return Vector3Int{0, -1, 0}; };
		constexpr static Vector3Int Left() { return Vector3Int{-1, 0, 0}; };
		constexpr static Vector3Int Right() { return Vector3Int{1, 0, 0}; };

		constexpr static Vector3Int Cross(const Vector3Int& lhs, const Vector3Int& rhs) {
			return {
				lhs.y() * rhs.z() - lhs.z() * rhs.y()
				, lhs.z() * rhs.x() - lhs.x() * rhs.z()
				, lhs.x() * rhs.y() - lhs.y() * rhs.x()
			};
		}
	};

	///////////////////////////////////////////////////////////////////////////
	// Vector4
	///////////////////////////////////////////////////////////////////////////
	struct Vector4: public BasicVector<4, float> {
		using Vortex::BasicVector<4, float>::BasicVector;
		using Vortex::BasicVector<4, float>::operator=;

		using Vortex::BasicVector<4, float>::operator+;
		using Vortex::BasicVector<4, float>::operator-;
		using Vortex::BasicVector<4, float>::operator*;
		using Vortex::BasicVector<4, float>::operator/;

		using Vortex::BasicVector<4, float>::operator+=;
		using Vortex::BasicVector<4, float>::operator-=;
		using Vortex::BasicVector<4, float>::operator*=;
		using Vortex::BasicVector<4, float>::operator/=;

		using Vortex::BasicVector<4, float>::Zero;
		using Vortex::BasicVector<4, float>::Magnitude;
		using Vortex::BasicVector<4, float>::SqrMagnitude;
		using Vortex::BasicVector<4, float>::Normalize;
		using Vortex::BasicVector<4, float>::Dot;
		using Vortex::BasicVector<4, float>::Reflect;
		using Vortex::BasicVector<4, float>::Distance;

		constexpr Vector4(float x, float y, float z, float w): Vortex::BasicVector<4, float>() {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = w;
		}

		constexpr void Set(float x, float y, float z, float w) {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = w;
		}

		constexpr float& x() { return m_Data[0]; }
		constexpr float& y() { return m_Data[1]; }
		constexpr float& z() { return m_Data[2]; }
		constexpr float& w() { return m_Data[3]; }
		constexpr const float& x() const { return m_Data[0]; }
		constexpr const float& y() const { return m_Data[1]; }
		constexpr const float& z() const { return m_Data[2]; }
		constexpr const float& w() const { return m_Data[3]; }

		constexpr Vector2 xy() const { return {x(), y()}; }
		constexpr Vector3 xyz() const { return {x(), y(), z()}; }

		constexpr static Vector4 Forward() { return Vector4{0.0f, 0.0f, 1.0f, 0.0f}; };
		constexpr static Vector4 Back() { return Vector4{0.0f, 0, -1.0f, 0.0f}; };
		constexpr static Vector4 Up() { return Vector4{0.0f, 1.0f, 0.0f, 0.0f}; };
		constexpr static Vector4 Down() { return Vector4{0.0f, -1.0f, 0.0f, 0.0f}; };
		constexpr static Vector4 Left() { return Vector4{-1.0f, 0.0f, 0.0f, 0.0f}; };
		constexpr static Vector4 Right() { return Vector4{1.0f, 0.0f, 0.0f, 0.0f}; };

	};

	///////////////////////////////////////////////////////////////////////////
	// Vector4Int
	///////////////////////////////////////////////////////////////////////////
	struct Vector4Int: public BasicVector<4, Int32> {
		using Vortex::BasicVector<4, Int32>::BasicVector;
		using Vortex::BasicVector<4, Int32>::operator=;

		using Vortex::BasicVector<4, Int32>::operator+;
		using Vortex::BasicVector<4, Int32>::operator-;
		using Vortex::BasicVector<4, Int32>::operator*;
		using Vortex::BasicVector<4, Int32>::operator/;

		using Vortex::BasicVector<4, Int32>::operator+=;
		using Vortex::BasicVector<4, Int32>::operator-=;
		using Vortex::BasicVector<4, Int32>::operator*=;
		using Vortex::BasicVector<4, Int32>::operator/=;

		using Vortex::BasicVector<4, Int32>::Zero;
		using Vortex::BasicVector<4, Int32>::Magnitude;
		using Vortex::BasicVector<4, Int32>::SqrMagnitude;
		using Vortex::BasicVector<4, Int32>::Normalize;
		using Vortex::BasicVector<4, Int32>::Dot;
		using Vortex::BasicVector<4, Int32>::Reflect;
		using Vortex::BasicVector<4, Int32>::Distance;

		constexpr Vector4Int(Int32 x, Int32 y, Int32 z, Int32 w): Vortex::BasicVector<4, Int32>() {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = w;
		}

		constexpr void Set(Int32 x, Int32 y, Int32 z, Int32 w) {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = w;
		}

		constexpr Int32& x() { return m_Data[0]; }
		constexpr Int32& y() { return m_Data[1]; }
		constexpr Int32& z() { return m_Data[2]; }
		constexpr Int32& w() { return m_Data[3]; }
		constexpr const Int32& x() const { return m_Data[0]; }
		constexpr const Int32& y() const { return m_Data[1]; }
		constexpr const Int32& z() const { return m_Data[2]; }
		constexpr const Int32& w() const { return m_Data[3]; }

		constexpr Vector2Int xy() const { return {x(), y()}; }
		constexpr Vector3Int xyz() const { return {x(), y(), z()}; }

		constexpr static Vector4Int Forward() { return Vector4Int{0, 0, 1, 0}; };
		constexpr static Vector4Int Back() { return Vector4Int{0, 0, -1, 0}; };
		constexpr static Vector4Int Up() { return Vector4Int{0, 1, 0, 0}; };
		constexpr static Vector4Int Down() { return Vector4Int{0, -1, 0, 0}; };
		constexpr static Vector4Int Left() { return Vector4Int{-1, 0, 0, 0}; };
		constexpr static Vector4Int Right() { return Vector4Int{1, 0, 0, 0}; };
	};

	struct Color: public BasicVector<4, float> {
		using Vortex::BasicVector<4, float>::BasicVector;
		using Vortex::BasicVector<4, float>::operator=;

		using Vortex::BasicVector<4, float>::operator+;
		using Vortex::BasicVector<4, float>::operator-;
		using Vortex::BasicVector<4, float>::operator*;
		using Vortex::BasicVector<4, float>::operator/;

		using Vortex::BasicVector<4, float>::operator+=;
		using Vortex::BasicVector<4, float>::operator-=;
		using Vortex::BasicVector<4, float>::operator*=;
		using Vortex::BasicVector<4, float>::operator/=;

		constexpr float& Red() { return m_Data[0]; }
		constexpr float& Green() { return m_Data[1]; }
		constexpr float& Blue() { return m_Data[2]; }
		constexpr float& Alpha() { return m_Data[3]; }
		constexpr const float& Red() const { return m_Data[0]; }
		constexpr const float& Green() const { return m_Data[1]; }
		constexpr const float& Blue() const { return m_Data[2]; }
		constexpr const float& Alpha() const { return m_Data[3]; }

	public:
		constexpr Color(float x, float y, float z, float w): Vortex::BasicVector<4, float>() {
			m_Data[0] = x;
			m_Data[1] = y;
			m_Data[2] = z;
			m_Data[3] = w;
		}
		constexpr void Set(float r, float g, float b, float a) {
			m_Data[0] = r;
			m_Data[1] = g;
			m_Data[2] = b;
			m_Data[3] = a;
		}
		constexpr float Grayscale() const { return (Red() + Green() + Blue()) / 3.0f; }

	public:
		constexpr static Color FromHexColor(UInt32 hex_color) {
			return Color{
				static_cast<float>((hex_color >> 24) & 0xFF) / 255.0f
				, static_cast<float>((hex_color >> 16) & 0xFF) / 255.0f
				, static_cast<float>((hex_color >> 8) & 0xFF) / 255.0f
				, static_cast<float>(hex_color & 0xFF) / 255.0f
			};
		}

		constexpr static UInt32 ToHexColor(const Color& color) {
			UInt32 attribute = static_cast<UInt32>(color.Red() * 255.0f) << 24;
			attribute += static_cast<UInt32>(color.Green() * 255.0f) << 16;
			attribute += static_cast<UInt32>(color.Blue() * 255.0f) << 8;
			attribute += static_cast<UInt32>(color.Alpha() * 255.0f);
			return attribute;
		}
	};

	struct StandardColors {
		enum Enum {
			Black = 0,
			Gray,
			White,

			Blue,
			Cyan,
			Green,
			Yellow,
			Red,
			Magenta,

			VortexCustom,
			Clear,

			Count
		};
		constexpr static Color Values[]{
			Color{0.0f, 0.0f, 0.0f, 1.0f}          // Black
			, Color{0.5f, 0.5f, 0.5f, 1.0f}        // Gray
			, Color{1.0f, 1.0f, 1.0f, 1.0f}        // White

			, Color{0.0f, 0.0f, 1.0f, 1.0f}        // Blue
			, Color{0.0f, 1.0f, 1.0f, 1.0f}        // Cyan
			, Color{0.0f, 1.0f, 0.0f, 1.0f}        // Green
			, Color{1.0f, 1.0f, 0.0f, 1.0f}        // Yellow
			, Color{1.0f, 0.0f, 0.0f, 1.0f}        // Red
			, Color{1.0f, 0.0f, 1.0f, 1.0f}        // Magenta

			, Color{
				0.58431372549019607843137254901961f
				, 0.68627450980392156862745098039216f
				, 0.75294117647058823529411764705882f
				, 1.0f
			}    // VortexCustom
			, Color{0.0f, 0.0f, 0.0f, 0.0f}        // Clear
		};
	};
}

#include "Vortex/Math/BasicMatrix.h"

namespace Vortex {
	// Column-Major ordered 2x2 matrix class.
	// |0  2|
	// |1  3|
	struct Matrix2: public BasicMatrix<2, 2, float> {
		using Vortex::BasicMatrix<2, 2, float>::BasicMatrix;
		using Vortex::BasicMatrix<2, 2, float>::operator=;

	public:
		constexpr Matrix2()
			: Vortex::BasicMatrix<2, 2, float>() {
			m_Data[0] = 1.0f;
			m_Data[1] = 0.0f;
			m_Data[2] = 1.0f;
			m_Data[3] = 0.0f;
		}
		constexpr Matrix2(float m0, float m1, float m2, float m3)
			: Vortex::BasicMatrix<2, 2, float>() {
			m_Data[0] = m0;
			m_Data[1] = m1;
			m_Data[2] = m2;
			m_Data[3] = m3;
		}
		constexpr void Set(float m0, float m1, float m2, float m3) {
			m_Data[0] = m0;
			m_Data[1] = m1;
			m_Data[2] = m2;
			m_Data[3] = m3;
		}
		inline void Set(const float* other) {
			std::memcpy(m_Data, other, 4 * sizeof(float));
		}

	public:
		Vector2 operator*(const Vector2& rhs) const;
		inline friend Vector2 operator*(const Vector2& lhs, const Matrix2& rhs) { return rhs * lhs; }
		Matrix2 operator*(const Matrix2& rhs) const;
		Matrix2& operator*=(const Matrix2& rhs);

	public:
		void Identity();
		float Determinant() const;
		bool Invert();
		void Transpose();

		void SetRotation(const Angle& angle);
		bool ExtractRotation(Angle& angle) const;
	};

	// Column-Major ordered 3x3 matrix class.
	// |0  3  6|
	// |1  4  7|
	// |2  5  8|
	struct Matrix3: public BasicMatrix<3, 3, float> {
		using Vortex::BasicMatrix<3, 3, float>::BasicMatrix;
		using Vortex::BasicMatrix<3, 3, float>::operator=;

		constexpr Matrix3()
			: Vortex::BasicMatrix<3, 3, float>() {
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
		constexpr Matrix3(float m0, float m1, float m2,
						  float m3, float m4, float m5,
						  float m6, float m7, float m8)
			: Vortex::BasicMatrix<3, 3, float>() {
			m_Data[0] = m0;
			m_Data[1] = m1;
			m_Data[2] = m2;
			m_Data[3] = m3;
			m_Data[4] = m4;
			m_Data[5] = m5;
			m_Data[6] = m6;
			m_Data[7] = m7;
			m_Data[8] = m8;
		}
		constexpr void Set(float m0, float m1, float m2,
						   float m3, float m4, float m5,
						   float m6, float m7, float m8) {
			m_Data[0] = m0;
			m_Data[1] = m1;
			m_Data[2] = m2;
			m_Data[3] = m3;
			m_Data[4] = m4;
			m_Data[5] = m5;
			m_Data[6] = m6;
			m_Data[7] = m7;
			m_Data[8] = m8;
		}
		inline void Set(const float* other) {
			std::memcpy(m_Data, other, 9 * sizeof(float));
		}

	public:
		Vector3 operator*(const Vector3& rhs) const;
		inline friend Vector3 operator*(const Vector3& lhs, const Matrix3& rhs) { return rhs * lhs; }
		Matrix3 operator*(const Matrix3& rhs) const;
		Matrix3& operator*=(const Matrix3& rhs);

	public:
		void Identity();
		float Determinant() const;
		bool Invert();
		void Transpose();

		void SetAngleAxis(const Angle& angle, const float* axis);
		void SetRotationXYZ(const Angle& x, const Angle& y, const Angle& z);
		void SetRotationX(const Angle& angle);
		void SetRotationY(const Angle& angle);
		void SetRotationZ(const Angle& angle);
		void SetLookAt(const Vector3& position, const Vector3& target, const Vector3& upwards);

		void SetTranslation(const float* vector);
		void ExtractTranslation(float* vector) const;
		bool ExtractRotation(Angle& angle) const;
		void TransformPosition(float* vector) const;
		void TransformDirection(float* vector) const;

		//void SetRotation(const Quaternion& rotation);
	};

	struct Matrix4: public BasicMatrix<4, 4, float> {
		using Vortex::BasicMatrix<4, 4, float>::BasicMatrix;
		using Vortex::BasicMatrix<4, 4, float>::operator=;

		constexpr Matrix4()
			: Vortex::BasicMatrix<4, 4, float>() {
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
		constexpr Matrix4(float m00, float m01, float m02, float m03,
						  float m04, float m05, float m06, float m07,
						  float m08, float m09, float m10, float m11,
						  float m12, float m13, float m14, float m15)
			: Vortex::BasicMatrix<4, 4, float>() {
			m_Data[0] = m00;
			m_Data[1] = m01;
			m_Data[2] = m02;
			m_Data[3] = m03;
			m_Data[4] = m04;
			m_Data[5] = m05;
			m_Data[6] = m06;
			m_Data[7] = m07;
			m_Data[8] = m08;
			m_Data[9] = m09;
			m_Data[10] = m10;
			m_Data[11] = m11;
			m_Data[12] = m12;
			m_Data[13] = m13;
			m_Data[14] = m14;
			m_Data[15] = m15;
		}
		constexpr void Set(float m00, float m01, float m02, float m03,
						   float m04, float m05, float m06, float m07,
						   float m08, float m09, float m10, float m11,
						   float m12, float m13, float m14, float m15) {
			m_Data[0] = m00;
			m_Data[1] = m01;
			m_Data[2] = m02;
			m_Data[3] = m03;
			m_Data[4] = m04;
			m_Data[5] = m05;
			m_Data[6] = m06;
			m_Data[7] = m07;
			m_Data[8] = m08;
			m_Data[9] = m09;
			m_Data[10] = m10;
			m_Data[11] = m11;
			m_Data[12] = m12;
			m_Data[13] = m13;
			m_Data[14] = m14;
			m_Data[15] = m15;
		}
		inline void Set(const float* other) {
			std::memcpy(m_Data, other, 16 * sizeof(float));
		}

	public:
		Vector4 operator*(const Vector4& rhs) const;
		inline friend Vector4 operator*(const Vector4& lhs, const Matrix4& rhs) { return rhs * lhs; }
		Matrix4 operator*(const Matrix4& rhs) const;
		Matrix4& operator*=(const Matrix4& rhs);

	public:
		void Identity();
		float Determinant() const;
		bool Invert();
		void Transpose();

		void SetPerspective(const Angle& angle, float aspect_ratio, float near, float far);
		void SetOrthographic(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);

		void SetAngleAxis(const Angle& angle, const Vector3& axis);
		//void SetRotation(const Quaternion& quaternion);
		void SetRotationXYZ(const Angle& x, const Angle& y, const Angle& z);
		void SetRotationX(const Angle& angle);
		void SetRotationY(const Angle& angle);
		void SetRotationZ(const Angle& angle);
		void SetLookAt(const Vector3& position, const Vector3& target, const Vector3& upwards);

		void SetTranslation(const float* vector);
		void ExtractTranslation(float* vector) const;
		//bool ExtractRotation(Quaternion& angle) const;
		void TransformPosition(float* vector) const;
		void TransformDirection(float* vector) const;
	};
}