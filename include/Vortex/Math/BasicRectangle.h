#pragma once
#include "Vortex/Math/BasicVector.h"

namespace Vortex::Math {
	template<typename T>
	struct BasicRectangle {
		union {
			T Data[4];
			struct {
				T x;
				T y;
				T width;
				T height;
			};
			struct {
				BasicVector<T, 2> Position;
				BasicVector<T, 2> Size;
			};
		};

	public:
		constexpr bool Contains(const BasicVector<T, 2>& point) const {
			return point.x >= Position.x
				&& point.x <= Position.x + Size.x
				&& point.y >= Position.y
				&& point.y <= Position.y + Size.y;
		}
		constexpr BasicVector<T, 2> GetCenter() const { return Position + Size / T{2}; }

		constexpr static bool Intersects(const BasicRectangle<T>& rectangle1, const BasicRectangle<T>& rectangle2) {
			return rectangle1.Position.x + rectangle1.Size.x > rectangle2.Position.x &&
				rectangle1.Position.y + rectangle1.Size.y > rectangle2.Position.y &&
				rectangle1.Position.x < rectangle2.Position.x + rectangle2.Size.x &&
				rectangle1.Position.y < rectangle2.Position.y + rectangle2.Size.y;
		}
	};
}