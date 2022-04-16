#pragma once

namespace Vortex::Math {
	template<typename T>
	constexpr T Pi();
	template<typename T>
	constexpr T TwoPi();
	template<typename T>
	constexpr T DegToRad();
	template<typename T>
	constexpr T RadToDeg();
	template<typename T>
	constexpr T Epsilon();
}

//float type specializations
namespace Vortex::Math {
	template<> constexpr float Pi<float>() { return 3.14159265359f; }
	template<> constexpr float TwoPi<float>() { return 2.0f * Pi<float>(); }
	template<> constexpr float DegToRad<float>() { return TwoPi<float>() / 360.0f; }
	template<> constexpr float RadToDeg<float>() { return 360.0f / TwoPi<float>(); }
	template<> constexpr float Epsilon<float>() { return 0.00001f; }
}
