#pragma once
#include "Vortex/Debug/Assert.h"
#include "Vortex/Math/VortexMathFunctions.h"

namespace Vortex::Math {
	// |0|
	// |1|
	// .
	// .
	// .
	// |n|
	template<typename T, SizeType Count>
	struct BasicVectorBase { T Data[Count]; };

	template<typename T>
	struct BasicVectorBase<T, 2> {
		union {
			T Data[2];
			struct { T x; T y; };
		};
	};
	template<typename T>
	struct BasicVectorBase<T, 3> {
		union {
			T Data[3];
			struct { T x; T y; T z; };
		};
	};
	template<typename T>
	struct BasicVectorBase<T, 4> {
		union {
			T Data[4];
			struct { T x; T y; T z; T w; };
		};
	};

	template<typename T, SizeType C>
	struct BasicVector: public BasicVectorBase<T, C> {
		using VectorType = BasicVector<T, C>;
		using BaseType = BasicVectorBase<T, C>;
		using UnderlyingType = T;
		constexpr static SizeType Length() { return C; }

	private:
		template<bool A>
		using EIF = typename std::enable_if_t<A>;

	public:
		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 2)>>
		constexpr void Set(UnderlyingType x, UnderlyingType y) {
			this->Data[0] = x;
			this->Data[1] = y;
		}
		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 3)>>
		constexpr void Set(UnderlyingType x, UnderlyingType y, UnderlyingType z) {
			this->Data[0] = x;
			this->Data[1] = y;
			this->Data[2] = z;
		}
		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 4)>>
		constexpr void Set(UnderlyingType x, UnderlyingType y, UnderlyingType z, UnderlyingType w) {
			this->Data[0] = x;
			this->Data[1] = y;
			this->Data[2] = z;
			this->Data[3] = w;
		}

	public:
		template<SizeType Length_ = Length(), typename = EIF<(Length_ > 2)>>
		constexpr BasicVector<UnderlyingType, 2> xy() const { return {this->Data[0], this->Data[1]}; }
		template<SizeType Length_ = Length(), typename = EIF<(Length_ > 3)>>
		constexpr BasicVector<UnderlyingType, 3> xyz() const { return {this->Data[0], this->Data[1], this->Data[2]}; }

	public:
		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 2)>>
		constexpr static VectorType CWPerpendicular(const VectorType& vector) { return {vector.y, -vector.x}; }
		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 2)>>
		constexpr static VectorType CCWPerpendicular(const VectorType& vector) { return {-vector.y, vector.x}; }

		template<SizeType Length_ = Length(), typename = EIF<(Length_ >= 2)>>
		constexpr static VectorType Up() { return VectorType{UnderlyingType{0}, UnderlyingType{1}}; };
		template<SizeType Length_ = Length(), typename = EIF<(Length_ >= 2)>>
		constexpr static VectorType Down() { return VectorType{UnderlyingType{0}, UnderlyingType{-1}}; };
		template<SizeType Length_ = Length(), typename = EIF<(Length_ >= 2)>>
		constexpr static VectorType Left() { return VectorType{UnderlyingType{-1}, UnderlyingType{0}}; };
		template<SizeType Length_ = Length(), typename = EIF<(Length_ >= 2)>>
		constexpr static VectorType Right() { return VectorType{UnderlyingType{1}, UnderlyingType{0}}; };

		template<SizeType Length_ = Length(), typename = EIF<(Length_ >= 3)>>
		constexpr static VectorType Forward() {
			return VectorType{UnderlyingType{0}, UnderlyingType{0}, UnderlyingType{1}};
		};
		template<SizeType Length_ = Length(), typename = EIF<(Length_ >= 3)>>
		constexpr static VectorType Backward() {
			return VectorType{UnderlyingType{0}, UnderlyingType{0}, UnderlyingType{-1}};
		};

	public:
		template<typename UnderlyingType_ = UnderlyingType, typename = EIF<(std::is_arithmetic_v<UnderlyingType_>)>>
		constexpr UnderlyingType_ Magnitude() const {
			UnderlyingType_ out{0};
			for (SizeType i = 0; i < Length(); ++i) {
				out += this->Data[i] * this->Data[i];
			}
			return out;
		}
		template<typename UnderlyingType_ = UnderlyingType, typename = EIF<(std::is_arithmetic_v<UnderlyingType_>)>>
		constexpr UnderlyingType_ SqrMagnitude() const {
			auto magnitude = Magnitude();
			if (magnitude > UnderlyingType_{0}) {
				return Sqrt<UnderlyingType_>(magnitude);
			} else {
				return UnderlyingType_{0};
			}
		}
		template<typename UnderlyingType_ = UnderlyingType, typename = EIF<(std::is_arithmetic_v<UnderlyingType_>)>>
		constexpr void Normalize() {
			auto sqr_magnitude = SqrMagnitude<UnderlyingType_>();

			if (sqr_magnitude > UnderlyingType_{0}) {
				auto rsqrt_magnitude = UnderlyingType_{1} / sqr_magnitude;
				for (SizeType i = 0; i < Length(); ++i) {
					this->Data[i] *= rsqrt_magnitude;
				}
			} else {
				for (SizeType i = 0; i < Length(); ++i) {
					this->Data[i] = T{0};
				}
			}
		}
		template<typename UnderlyingType_ = UnderlyingType, typename = EIF<(std::is_arithmetic_v<UnderlyingType_>)>>
		constexpr static UnderlyingType_ Dot(const VectorType& lhs, const VectorType& rhs) {
			UnderlyingType_ mag{0};
			for (SizeType i = 0; i < Length(); ++i) {
				mag += lhs.Data[i] * rhs.Data[i];
			}
			return mag;
		}
		template<typename UnderlyingType_ = UnderlyingType, typename = EIF<(std::is_arithmetic_v<UnderlyingType_>)>>
		constexpr static VectorType Reflect(const VectorType& vector, const VectorType& normal) {
			UnderlyingType_ dot = Dot(normal);
			VectorType out;
			for (SizeType i = 0; i < Length(); ++i) {
				out.Data[i] = vector.Data[i] - normal.Data[i] * dot * UnderlyingType_{2};
			}
			return out;
		}
		template<typename UnderlyingType_ = UnderlyingType, typename = EIF<(std::is_arithmetic_v<UnderlyingType_>)>>
		constexpr static UnderlyingType_ Distance(const VectorType& lhs, const VectorType& rhs) {
			UnderlyingType_ magnitude{0};

			for (SizeType i = 0; i < Length(); ++i) {
				auto d = rhs.Data[i] - lhs.Data[i];
				magnitude += d * d;
			}

			if (magnitude > UnderlyingType_{0}) {
				return Sqrt<UnderlyingType_>(magnitude);
			} else {
				return UnderlyingType_{0};
			}
		}

	public:
		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 3)>>
		constexpr static VectorType Cross(const VectorType& lhs, const VectorType& rhs) {
			return {
				lhs.y * rhs.z - lhs.z * rhs.y
				, lhs.z * rhs.x - lhs.x * rhs.z
				, lhs.x * rhs.y - lhs.y * rhs.x
			};
		}

		template<SizeType Length_ = Length(), typename = EIF<(Length_ == 2)>>
		inline static BasicAngle<UnderlyingType> AngleBetween(const VectorType& from, const VectorType& to) {
			auto d = to - from;
			auto atan2 = Atan2<UnderlyingType>(d.y, d.x);
			return BasicAngle<UnderlyingType>::FromRadians(atan2);
		}

	public:
		constexpr friend VectorType operator-(const VectorType& lhs) {
			VectorType out;
			for (SizeType i = 0; i < Length(); ++i) {
				out.Data[i] = -lhs.Data[i];
			}
			return out;
		}

		template<SizeType Other_Length_, SizeType This_Length_ = Length(), typename = EIF<(This_Length_ > Other_Length_)>>
		constexpr explicit operator BasicVector<UnderlyingType, Other_Length_>() {
			BasicVector<UnderlyingType, Other_Length_> out;
			for (SizeType i = 0; i < Other_Length_; ++i) {
				out.Data[i] = this->Data[i];
			}
			return out;
		}

		UnderlyingType& operator[](Vortex::SizeType i) {
			VORTEX_ASSERT(i < Length())
			return this->Data[i];
		}
		const UnderlyingType& operator[](Vortex::SizeType i) const {
			VORTEX_ASSERT(i < Length())
			return this->Data[i];
		}

	public:
#define VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_OPERATOR(op) \
constexpr friend VectorType operator op(const VectorType& lhs, UnderlyingType rhs) {    \
VectorType out;    \
for (SizeType i = 0; i < Length(); ++i) {    \
out.Data[i] = lhs.Data[i] op rhs;    \
}    \
return out;                                  \
}

		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_OPERATOR(+)
		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_OPERATOR(-)
		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_OPERATOR(*)
		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_OPERATOR(/)

#undef VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_OPERATOR

	public:
#define VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_EQUALS_OPERATOR(op) \
constexpr friend VectorType& operator op(VectorType& lhs, UnderlyingType rhs) {    \
for (SizeType i = 0; i < Length(); ++i) {    \
lhs.Data[i] op rhs;    \
}    \
return lhs;    \
}

		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_EQUALS_OPERATOR(+=)
		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_EQUALS_OPERATOR(-=)
		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_EQUALS_OPERATOR(*=)
		VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_EQUALS_OPERATOR(/=)

#undef VORTEX_BASIC_VECTOR_UNDERLYING_TYPE_EQUALS_OPERATOR

	public:
#define VORTEX_BASIC_VECTOR_VECTOR_OPERATOR(op)         \
constexpr friend VectorType operator op(const VectorType& lhs, const VectorType& rhs) {    \
VectorType out;    \
for (SizeType i = 0; i < Length(); ++i) {    \
out.Data[i] = lhs.Data[i] op rhs.Data[i];    \
}    \
return out;    \
}
		VORTEX_BASIC_VECTOR_VECTOR_OPERATOR(+)
		VORTEX_BASIC_VECTOR_VECTOR_OPERATOR(-)
		VORTEX_BASIC_VECTOR_VECTOR_OPERATOR(*)
		VORTEX_BASIC_VECTOR_VECTOR_OPERATOR(/)

#undef VORTEX_BASIC_VECTOR_VECTOR_OPERATOR

	public:
#define VORTEX_BASIC_VECTOR_VECTOR_EQUALS_OPERATOR(op) \
constexpr friend VectorType& operator op(VectorType& lhs, const VectorType& rhs) {    \
for (SizeType i = 0; i < Length(); ++i) {    \
lhs.Data[i] op rhs.Data[i];    \
}    \
return lhs;    \
}

		VORTEX_BASIC_VECTOR_VECTOR_EQUALS_OPERATOR(+=)
		VORTEX_BASIC_VECTOR_VECTOR_EQUALS_OPERATOR(-=)
		VORTEX_BASIC_VECTOR_VECTOR_EQUALS_OPERATOR(*=)
		VORTEX_BASIC_VECTOR_VECTOR_EQUALS_OPERATOR(/=)

#undef VORTEX_BASIC_VECTOR_VECTOR_EQUALS_OPERATOR
	};
}

template<typename T, Vortex::SizeType Rows>
std::ostream& operator<<(
	std::ostream& stream,
	const Vortex::Math::BasicVector<T, Rows>& vector
) {
	stream << "[ ";
	for (Vortex::SizeType i = 0; i < Rows; ++i) {
		stream << vector[i] << " ";
	}
	stream << "]\n";
	return stream;
}
