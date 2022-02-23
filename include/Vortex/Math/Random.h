#pragma once
#include <random>
#include <type_traits>

#include "Vortex/Memory/Memory.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Math/VortexMath.h"

namespace Vortex {
	class Random {
	public:
		using SeedType = UInt32;

	public:
		Random();
		Random(SeedType seed);

		void SetSeed(SeedType seed);
		SeedType GetSeed() { return m_Seed; }

	public:
		float Range(float min, float max);
		inline float Range01() { return Range(0.0f, 1.0f); };
		Vector2 UnitCircle();
		Vector3 UnitSphere();

		template<typename T>
		T& Element(T* array, SizeType count) {
			std::uniform_int_distribution<std::size_t> distribution{0, count};
			return array[distribution(m_RandomDevice)];
		}

		template<typename T>
		const T& Element(const T* array, SizeType count) {
			std::uniform_int_distribution<std::size_t> distribution{0, count};
			return array[distribution(m_RandomDevice)];
		}

		template<typename T>
		T Number() {
			VORTEX_STATIC_ASSERT_MSG(std::is_integral<T>::value || std::is_floating_point<T>::value, "T must be integral or floating point type.")

			if constexpr(std::is_integral<T>::value) {
				std::uniform_int_distribution<T> distribution;
				return distribution(m_RandomDevice);
			} else {
				std::uniform_real_distribution<T> distribution;
				return distribution(m_RandomDevice);
			}
		}

	private:
		std::mt19937 m_RandomDevice;
		SeedType m_Seed;
	};
}
