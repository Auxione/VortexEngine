#pragma once

namespace Vortex::Math {
	template<typename T>
	using EIF_FPType = std::enable_if_t<std::is_floating_point_v<T>>;
	template<typename T>
	using EIF_IntType = std::enable_if_t<std::is_integral_v<T>>;

	template<typename T, typename = EIF_FPType<T>> inline T Sin(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Cos(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Tan(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Asin(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Acos(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Atan(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Atan2(T y, T x);
	template<typename T, typename = EIF_FPType<T>> inline T Sqrt(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Abs(T value);
	template<typename T, typename = EIF_FPType<T>> inline T CopySign(T mag, T sign);

	template<typename T, typename = EIF_FPType<T>> inline T Round(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Ceil(T value);
	template<typename T, typename = EIF_FPType<T>> inline T Floor(T value);

	template<typename IT, typename FT, typename = EIF_IntType<IT>, typename = EIF_FPType<FT>>
	inline IT RoundToInt(FT value);
	template<typename IT, typename FT, typename = EIF_IntType<IT>, typename = EIF_FPType<FT>>
	inline IT CeilToInt(FT value);
	template<typename IT, typename FT, typename = EIF_IntType<IT>, typename = EIF_FPType<FT>>
	inline IT FloorToInt(FT value);

	template<typename T> constexpr T Min(T v1, T v2) { return v1 > v2 ? v2 : v1; }
	template<typename T> constexpr T Max(T v1, T v2) { return v1 < v2 ? v2 : v1; }

	template<typename T, typename = EIF_FPType<T>>
	constexpr T Lerp(T start, T stop, T ratio) { return start + (stop - start) * ratio; }
	template<typename T, typename = EIF_FPType<T>>
	constexpr T Normalize(T value, T start, T stop) { return (value - start) / (stop - start); }
	template<typename T>
	constexpr T Map(T value, T value_min, T value_max, T output_min, T output_max) {
		return output_min + (output_max - output_min) * ((value - value_min) / (value_max - value_min));
	}
	template<typename T>
	constexpr T Clamp(T value, T min, T max) {
		if (value > max) {
			return max;
		} else if (value < min) {
			return min;
		} else {
			return value;
		}
	}
	template<typename T, typename = EIF_FPType<T>>
	inline static bool Compare(T value1, T value2, T epsilon = Math::Epsilon) {
		return Math::Abs<T>(value1) - Math::Abs<T>(value2) < epsilon;
	}

	template<typename T, typename  = std::enable_if<std::is_integral_v<T>>>
	constexpr bool IsEven(T value) { return !(value & 1); }

	template<typename T, typename  = std::enable_if<std::is_integral_v<T>>>
	constexpr bool IsOdd(T value) { return !IsEven(value); }
}

//float type specializations
#include <cmath>
namespace Vortex::Math {
	template<> inline float Sin<float>(float value) { return std::sinf(value); }
	template<> inline float Cos<float>(float value) { return std::cosf(value); }
	template<> inline float Tan<float>(float value) { return std::tanf(value); }
	template<> inline float Asin<float>(float value) { return std::asinf(value); }
	template<> inline float Acos<float>(float value) { return std::acosf(value); }
	template<> inline float Atan<float>(float value) { return std::atanf(value); }
	template<> inline float Atan2<float>(float y, float x) { return std::atan2f(y, x); }
	template<> inline float Sqrt<float>(float value) { return std::sqrtf(value); }
	template<> inline float Abs<float>(float value) { return std::fabs(value); }
	template<> inline float CopySign<float>(float mag, float sign) { return std::copysignf(mag, sign); }

	template<> inline float Round(float value) { return std::roundf(value); }
	template<> inline float Ceil(float value) { return std::ceilf(value); }
	template<> inline float Floor(float value) { return std::floorf(value); }

	template<> inline Int32 RoundToInt<Int32, float>(float value) { return static_cast<Int32>(std::roundf(value)); }
	template<> inline Int32 CeilToInt<Int32, float>(float value) { return static_cast<Int32>(std::ceilf(value)); }
	template<> inline Int32 FloorToInt<Int32, float>(float value) { return static_cast<Int32>(std::floorf(value)); }

	template<> inline float Min<float>(float v1, float v2) { return std::min(v1, v2); }
	template<> inline float Max<float>(float v1, float v2) { return std::max(v1, v2); }
}
