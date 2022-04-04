#pragma once
#include <type_traits>
#include <tuple>

#include "Vortex/Memory/Memory.h"

namespace Vortex {
	template<typename T, SizeType ... BitSizes_>
	class IntegerPacker {
		VORTEX_STATIC_ASSERT(std::is_integral<T>::value)
		VORTEX_STATIC_ASSERT(std::is_unsigned<T>::value)

		using UnderlyingType = T;

		constexpr static SizeType ElementCount{sizeof...(BitSizes_)};
		constexpr static SizeType BitSizes[ElementCount]{BitSizes_...};

	public:
		template<SizeType I, SizeType Start = 0, SizeType U = 0>
		constexpr static SizeType GetBitStart_r() {
			if constexpr(U < I) {
				return GetBitStart_r<I, Start + BitSizes[U], U + 1>();
			}
			return Start;
		}
		template<SizeType I = 0, SizeType Total = 0>
		constexpr static SizeType GetTotalSize_r() {
			if constexpr(I < ElementCount) {
				return GetTotalSize_r<I + 1, Total + BitSizes[I]>();
			}
			return 0;
		}

		VORTEX_STATIC_ASSERT(GetTotalSize_r() <= sizeof(UnderlyingType) * CHAR_BIT)

	public:
		template<SizeType I>
		constexpr static UnderlyingType GetMaxValue() {
			return (UnderlyingType{1} << BitSizes[I]) - UnderlyingType{1};
		}

	public:
		template<SizeType I, typename = std::enable_if<(I < ElementCount)>>
		constexpr static void Pack(UnderlyingType& integer, UnderlyingType value) {
			VORTEX_ASSERT(value <= GetMaxValue<I>())
			integer |= (value & GetMaxValue<I>()) << GetBitStart_r<I>();
		}

		template<SizeType I, typename = std::enable_if<(I < ElementCount)>>
		constexpr static UnderlyingType Unpack(UnderlyingType integer) {
			return (integer >> GetBitStart_r<I>()) & GetMaxValue<I>();
		}
	};
}