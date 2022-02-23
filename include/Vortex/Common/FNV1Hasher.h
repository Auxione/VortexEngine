#pragma once
#include "Vortex/Memory/Memory.h"

namespace Vortex {
	namespace FNV1Hasher {
		constexpr static HashType Prime = 16777619u;
		constexpr static HashType Seed = 2166136261u;

		template<SizeType N, SizeType I>
		constexpr static void Hash_R(const Byte (& str)[N], HashType& hash) {
			if constexpr(I < N) {
				hash *= Prime;
				hash ^= str[I];

				Hash_R<N, I + 1>(str, hash);
			}
		}

		template<SizeType N>
		constexpr static HashType Hash(const Byte (& str)[N]) {
			HashType hash{Seed};
			Hash_R<N, 0>(str, hash);
			return hash;
		}

		static HashType HashRuntime(const Byte* str, SizeType length) {
			HashType hash{Seed};
			for (SizeType i = 0; i < length; ++i) {
				hash *= Prime;
				hash ^= str[i];
			}
			return hash;
		}
	};
}