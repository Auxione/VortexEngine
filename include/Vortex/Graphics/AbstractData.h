#pragma once

namespace Vortex::Graphics {
	template<Vortex::SizeType Size_>
	struct AbstractData {
		Byte Data[Size_];

		template<typename T>
		inline T& Layout() {
			VORTEX_STATIC_ASSERT(sizeof(T) < Size_)
			VORTEX_STATIC_ASSERT(std::is_trivial_v<T>)

			return *reinterpret_cast<T*>(Data);
		}
		template<typename T>
		inline const T& Layout() const {
			VORTEX_STATIC_ASSERT(sizeof(T) < Size_)
			VORTEX_STATIC_ASSERT(std::is_trivial_v<T>)
			return *reinterpret_cast<const T*>(Data);
		}
	};
}