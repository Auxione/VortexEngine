#pragma once

namespace Vortex {
	using HandleType = std::uint32_t;
	constexpr static HandleType InvalidID = -1;

	template<typename T>
	struct Handle {
		using Type = T;
		Vortex::HandleType id{Vortex::InvalidID};

		Handle(): id{InvalidID} {}
		Handle(Vortex::HandleType id_): id{id_} {}

		constexpr friend bool operator==(Handle<T> rhs, Handle<T> lhs) { return rhs.id == lhs.id; }
		constexpr friend bool operator!=(Handle<T> rhs, Handle<T> lhs) { return rhs.id != lhs.id; }
	};
}

template<typename T>
struct std::hash<Vortex::Handle<T>> {
	std::size_t operator()(const Vortex::Handle<T>& s) const noexcept {
		return std::hash<Vortex::HandleType>{}(s.id);
	}
};