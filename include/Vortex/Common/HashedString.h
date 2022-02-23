#pragma once
#include "Vortex/Common/FNV1Hasher.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex {
	struct HashedString {
		template<SizeType N>
		constexpr HashedString(const char (& str)[N]): m_Hash{FNV1Hasher::Hash<N>(str)} {}

		HashedString(const char* str, SizeType length)
			: m_Hash{FNV1Hasher::HashRuntime(str, length)} {
			VORTEX_ASSERT(length > 0)
		}

	public:
		constexpr operator HashType() const { return m_Hash; }
		constexpr bool operator==(const HashedString& other) const { return m_Hash == other.m_Hash; }

	public:
		constexpr HashType Get() const { return m_Hash; }

	private:
		HashType m_Hash;
	};
}

template<>
struct std::hash<Vortex::HashedString> {
	std::size_t operator()(const Vortex::HashedString& s) const noexcept { return s.Get(); }
};