#pragma once

namespace Vortex {
	template<typename T, typename U = UInt32>
	struct StrongHandle {
		using Type = T;
		using UnderlyingType = U;

		UnderlyingType id;

		constexpr friend bool operator==(StrongHandle<T> rhs, StrongHandle<T> lhs) { return rhs.id == lhs.id; }
		constexpr friend bool operator!=(StrongHandle<T> rhs, StrongHandle<T> lhs) { return rhs.id != lhs.id; }
	};
}

template<typename T, typename U>
struct std::hash<Vortex::StrongHandle<T, U>> {
	std::size_t operator()(const Vortex::StrongHandle<T, U>& s) const noexcept {
		return std::hash<U>(s.id);
	}
};