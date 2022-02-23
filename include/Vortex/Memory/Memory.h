#pragma once
#include <cstdint>
#include <cstddef>
#include "Vortex/Debug/Assert.h"

namespace Vortex {
	using Byte = char;
	using UByte = unsigned char;

	using Int8 = std::int8_t;
	using Int16 = std::int16_t;
	using Int32 = std::int32_t;
	using Int64 = std::int64_t;

	using UInt8 = std::uint8_t;
	using UInt16 = std::uint16_t;
	using UInt32 = std::uint32_t;
	using UInt64 = std::uint64_t;

	using SizeType = std::size_t;

	using HashType = std::size_t;
}

namespace Vortex {
	template<typename Dest_Type, typename Source_Type>
	Dest_Type UnionCast(Source_Type source) {
		VORTEX_STATIC_ASSERT((sizeof(Dest_Type) == sizeof(Source_Type)));
		union {
			Source_Type source_type;
			Dest_Type dest_type;
		} u;
		u.source_type = source;
		return u.dest_type;
	}

	template<typename T, SizeType N>
	SizeType ArrayCount(const T(& data)[N]) { return N; }

}

#include <string>
namespace Vortex {
	using String = std::string;
}