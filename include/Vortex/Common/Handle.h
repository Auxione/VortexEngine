#pragma once

namespace Vortex {
	using HandleType = std::uint32_t;
	constexpr static HandleType InvalidID = -1;
}

#define VORTEX_HANDLE(klass)														\
struct klass {																		\
Vortex::HandleType id{Vortex::InvalidID};											\
constexpr friend bool operator==(klass rhs, klass lhs) { return rhs.id == lhs.id;}	\
constexpr friend bool operator!=(klass rhs, klass lhs) { return rhs.id != lhs.id;}	\
}

#define VORTEX_HANDLE_HASH(klass)													\
template<>																			\
struct std::hash<klass> {															\
	std::size_t operator()(const klass& s) const noexcept {							\
		return std::hash<Vortex::HandleType>{}(s.id);								\
	}																				\
}																					\
