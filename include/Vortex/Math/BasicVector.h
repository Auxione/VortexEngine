#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex {
	// |0|
	// |1|
	// .
	// .
	// .
	// |n|
	template<SizeType Count, typename T>
	struct BasicVector {
		constexpr BasicVector() = default;
		constexpr BasicVector(const BasicVector&) = default;
		constexpr BasicVector(BasicVector&&) noexcept = default;
		constexpr BasicVector(const T* data) { std::memcpy(m_Data, data, Count * sizeof(T)); };
		inline ~BasicVector() = default;

	public:
		inline T* Data() { return m_Data; }
		inline const T* Data() const { return m_Data; }

	protected:
		T& operator[](Vortex::SizeType i) { return m_Data[i]; }
		const T& operator[](Vortex::SizeType i) const { return m_Data[i]; }
		constexpr BasicVector& operator=(const BasicVector&) = default;
		constexpr BasicVector& operator=(BasicVector&&) noexcept = default;

#define VORTEX_BASICVECTOR_EQ_OP(op)                                                    \
        constexpr friend bool operator op(const BasicVector<Count, T>& lhs, const BasicVector<Count, T>& rhs)  {        \
            bool out{true};                                                                \
            for (int i = 0; i < Count; ++i) {                                            \
                out &= lhs.m_Data[i] op rhs.m_Data[i];                                    \
            }                                                                            \
            return out;                                                                     \
        }

		VORTEX_BASICVECTOR_EQ_OP(==)
		VORTEX_BASICVECTOR_EQ_OP(!=)
		VORTEX_BASICVECTOR_EQ_OP(>=)
		VORTEX_BASICVECTOR_EQ_OP(<=)
		VORTEX_BASICVECTOR_EQ_OP(>)
		VORTEX_BASICVECTOR_EQ_OP(<)

#undef VORTEX_BASICVECTOR_EQ_OP//(op)

#define VORTEX_BASICVECTOR_OP_EQ(op)                                                    \
        constexpr BasicVector<Count, T>& operator op(T rhs) {                                        \
            for (int i = 0; i < Count; ++i) {                                            \
                m_Data[i] op rhs;                                                        \
            }                                                                            \
            return *this;                                                                \
        }                                                                                \
        constexpr BasicVector<Count, T>& operator op(const BasicVector<Count, T>& rhs) {            \
            for (int i = 0; i < Count; ++i) {                                            \
                m_Data[i] op rhs[i];                                                    \
            }                                                                            \
            return *this;                                                                \
        }

		VORTEX_BASICVECTOR_OP_EQ(+=)
		VORTEX_BASICVECTOR_OP_EQ(-=)
		VORTEX_BASICVECTOR_OP_EQ(*=)
		VORTEX_BASICVECTOR_OP_EQ(/=)

#undef VORTEX_BASICVECTOR_OP_EQ//(op)

#define VORTEX_BASICVECTOR_OP(op)                                                        \
        constexpr BasicVector<Count, T> operator op(T rhs) const{                                    \
        BasicVector<Count, T> out;                                                        \
            for (int i = 0; i < Count; ++i) {                                            \
                out.m_Data[i] = m_Data[i] op rhs;                                        \
            }                                                                            \
            return out;                                                                    \
        }                                                                                \
        constexpr BasicVector<Count, T> operator op(const BasicVector<Count, T>& rhs)const {        \
        BasicVector<Count, T> out;                                                        \
            for (int i = 0; i < Count; ++i) {                                            \
            out.m_Data[i] = m_Data[i] op rhs.m_Data[i];                                    \
            }                                                                            \
            return out;                                                                    \
        }

		VORTEX_BASICVECTOR_OP(+)
		VORTEX_BASICVECTOR_OP(-)
		VORTEX_BASICVECTOR_OP(*)
		VORTEX_BASICVECTOR_OP(/)

#undef VORTEX_BASICVECTOR_OP//(op)

		constexpr BasicVector<Count, T> operator-() const {
			BasicVector<Count, T> out;
			for (int i = 0; i < Count; ++i) {
				out.m_Data[i] = -m_Data[i];
			}
			return out;
		}

	protected:
		constexpr void Zero() {
			for (int i = 0; i < Count; ++i) {
				m_Data[i] = T{};
			}
		}
		constexpr T Magnitude() const {
			T mag{0};
			for (int i = 0; i < Count; ++i) {
				mag += m_Data[i] * m_Data[i];
			}
			return mag;
		}
		constexpr T SqrMagnitude() const {
			auto magnitude = Magnitude();
			if (magnitude > T{0}) {
				return Math::Sqrt(magnitude);
			} else {
				return T{0};
			}
		}
		constexpr void Normalize() {
			auto sqr_magnitude = SqrMagnitude();

			if (sqr_magnitude > T{0}) {
				auto rsqrt_magnitude = T{1} / sqr_magnitude;
				for (int i = 0; i < Count; ++i) {
					m_Data[i] *= rsqrt_magnitude;
				}
			} else {
				for (int i = 0; i < Count; ++i) {
					m_Data[i] = T{0};
				}
			}
		}
		constexpr T Dot(const BasicVector<Count, T>& rhs) const {
			T mag{0};
			for (int i = 0; i < Count; ++i) {
				mag += m_Data[i] * rhs.m_Data[i];
			}
			return mag;
		}
		constexpr BasicVector<Count, T> Reflect(const BasicVector<Count, T>& normal) const {
			T dot = Dot(normal);
			BasicVector<Count, T> output;
			for (int i = 0; i < Count; ++i) {
				output.m_Data[i] = m_Data[i] - normal.m_Data[i] * dot * T{2};
			}
			return output;
		}
		constexpr T Distance(const BasicVector<Count, T>& rhs) {
			T magnitude{0};

			for (int i = 0; i < Count; ++i) {
				auto d = rhs[i] - m_Data[i];
				magnitude += d * d;
			}

			if (magnitude > T{0}) {
				return Math::Sqrt(magnitude);
			} else {
				return T{0};
			}
		}

	protected:
		T m_Data[Count];
	};
}