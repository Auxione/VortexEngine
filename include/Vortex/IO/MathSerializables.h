#pragma once
#include "Vortex/IO/Serializable.h"
#include "Vortex/IO/Serializer.h"

#include "Vortex/Math/VortexMath.h"

namespace Vortex {
	template<typename T>
	struct Serializable<Math::BasicVector<T, 2>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicVector<T, 2>& obj) {
			bool out{true};
			out &= serializer.Set<T>("x", obj.x);
			out &= serializer.Set<T>("y", obj.y);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicVector<T, 2>& obj) {
			bool out{true};
			out &= serializer.Get<T>("x", obj.x);
			out &= serializer.Get<T>("y", obj.y);
			return out;
		}
	};
	template<typename T>
	struct Serializable<Math::BasicVector<T, 3>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicVector<T, 3>& obj) {
			bool out{true};
			out &= serializer.Set<T>("x", obj.x);
			out &= serializer.Set<T>("y", obj.y);
			out &= serializer.Set<T>("z", obj.y);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicVector<T, 3>& obj) {
			bool out{true};
			out &= serializer.Get<T>("x", obj.x);
			out &= serializer.Get<T>("y", obj.y);
			out &= serializer.Get<T>("z", obj.y);
			return out;
		}
	};
	template<typename T>
	struct Serializable<Math::BasicVector<T, 4>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicVector<T, 4>& obj) {
			bool out{true};
			out &= serializer.Set<T>("x", obj.x);
			out &= serializer.Set<T>("y", obj.y);
			out &= serializer.Set<T>("z", obj.y);
			out &= serializer.Set<T>("w", obj.y);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicVector<T, 4>& obj) {
			bool out{true};
			out &= serializer.Get<T>("x", obj.x);
			out &= serializer.Get<T>("y", obj.y);
			out &= serializer.Get<T>("z", obj.y);
			out &= serializer.Get<T>("w", obj.y);
			return out;
		}
	};

	template<typename T>
	struct Serializable<Math::BasicMatrix<T, 2, 2>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicMatrix<T, 2, 2>& obj) {
			bool out{true};
			out &= serializer.Set<T>("m00", obj.m00);
			out &= serializer.Set<T>("m10", obj.m10);

			out &= serializer.Set<T>("m01", obj.m01);
			out &= serializer.Set<T>("m11", obj.m11);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicMatrix<T, 2, 2>& obj) {
			bool out{true};
			out &= serializer.Get<T>("m00", obj.m00);
			out &= serializer.Get<T>("m10", obj.m10);

			out &= serializer.Get<T>("m01", obj.m01);
			out &= serializer.Get<T>("m11", obj.m11);
			return out;
		}
	};

	template<typename T>
	struct Serializable<Math::BasicMatrix<T, 3, 3>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicMatrix<T, 3, 3>& obj) {
			bool out{true};
			out &= serializer.Set<T>("m00", obj.m00);
			out &= serializer.Set<T>("m10", obj.m10);
			out &= serializer.Set<T>("m20", obj.m20);

			out &= serializer.Set<T>("m01", obj.m01);
			out &= serializer.Set<T>("m11", obj.m11);
			out &= serializer.Set<T>("m21", obj.m21);

			out &= serializer.Set<T>("m02", obj.m02);
			out &= serializer.Set<T>("m12", obj.m12);
			out &= serializer.Set<T>("m22", obj.m22);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicMatrix<T, 3, 3>& obj) {
			bool out{true};
			out &= serializer.Get<T>("m00", obj.m00);
			out &= serializer.Get<T>("m10", obj.m10);
			out &= serializer.Get<T>("m20", obj.m20);

			out &= serializer.Get<T>("m01", obj.m01);
			out &= serializer.Get<T>("m11", obj.m11);
			out &= serializer.Get<T>("m21", obj.m21);

			out &= serializer.Get<T>("m02", obj.m02);
			out &= serializer.Get<T>("m12", obj.m12);
			out &= serializer.Get<T>("m22", obj.m22);
			return out;
		}
	};

	template<typename T>
	struct Serializable<Math::BasicMatrix<T, 4, 4>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicMatrix<T, 4, 4>& obj) {
			bool out{true};
			out &= serializer.Set<T>("m00", obj.m00);
			out &= serializer.Set<T>("m10", obj.m10);
			out &= serializer.Set<T>("m20", obj.m20);
			out &= serializer.Set<T>("m30", obj.m30);

			out &= serializer.Set<T>("m01", obj.m01);
			out &= serializer.Set<T>("m11", obj.m11);
			out &= serializer.Set<T>("m21", obj.m21);
			out &= serializer.Set<T>("m31", obj.m31);

			out &= serializer.Set<T>("m02", obj.m02);
			out &= serializer.Set<T>("m12", obj.m12);
			out &= serializer.Set<T>("m22", obj.m22);
			out &= serializer.Set<T>("m32", obj.m32);

			out &= serializer.Set<T>("m03", obj.m03);
			out &= serializer.Set<T>("m13", obj.m13);
			out &= serializer.Set<T>("m23", obj.m23);
			out &= serializer.Set<T>("m33", obj.m33);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicMatrix<T, 3, 3>& obj) {
			bool out{true};
			out &= serializer.Get<T>("m00", obj.m00);
			out &= serializer.Get<T>("m10", obj.m10);
			out &= serializer.Get<T>("m20", obj.m20);
			out &= serializer.Get<T>("m30", obj.m30);

			out &= serializer.Get<T>("m01", obj.m01);
			out &= serializer.Get<T>("m11", obj.m11);
			out &= serializer.Get<T>("m21", obj.m21);
			out &= serializer.Get<T>("m31", obj.m31);

			out &= serializer.Get<T>("m02", obj.m02);
			out &= serializer.Get<T>("m12", obj.m12);
			out &= serializer.Get<T>("m22", obj.m22);
			out &= serializer.Get<T>("m32", obj.m32);

			out &= serializer.Get<T>("m03", obj.m03);
			out &= serializer.Get<T>("m13", obj.m13);
			out &= serializer.Get<T>("m23", obj.m23);
			out &= serializer.Get<T>("m33", obj.m33);
			return out;
		}
	};

	template<typename T>
	struct Serializable<Math::BasicAngle<T>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicAngle<T>& obj) {
			bool out{true};
			out &= serializer.Set<T>("degrees", obj.ToDegrees());
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicAngle<T>& obj) {
			bool out{true};
			T degrees;
			out &= serializer.Get<T>("degrees", degrees);
			obj = Math::Angle::FromDegrees(degrees);
			return out;
		}
	};

	template<typename T>
	struct Serializable<Math::BasicRectangle<T>> {
		inline static bool Serialize(Serializer& serializer, const Math::BasicRectangle<T>& obj) {
			bool out{true};
			out &= serializer.Set<T>("x", obj.x);
			out &= serializer.Set<T>("y", obj.y);
			out &= serializer.Set<T>("width", obj.width);
			out &= serializer.Set<T>("height", obj.height);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::BasicRectangle<T>& obj) {
			bool out{true};
			out &= serializer.Get<T>("x", obj.x);
			out &= serializer.Get<T>("y", obj.y);
			out &= serializer.Get<T>("width", obj.width);
			out &= serializer.Get<T>("height", obj.height);
			return out;
		}
	};

	template<>
	struct Serializable<Math::Color> {
		inline static bool Serialize(Serializer& serializer, const Math::Color& obj) {
			bool out{true};
			out &= serializer.Set<float>("red", obj.r);
			out &= serializer.Set<float>("green", obj.g);
			out &= serializer.Set<float>("blue", obj.b);
			out &= serializer.Set<float>("alpha", obj.a);
			return out;
		}
		inline static bool Deserialize(Serializer& serializer, Math::Color& obj) {
			bool out{true};
			out &= serializer.Get<float>("red", obj.r);
			out &= serializer.Get<float>("green", obj.g);
			out &= serializer.Get<float>("blue", obj.b);
			out &= serializer.Get<float>("alpha", obj.a);
			return out;
		}
	};
}