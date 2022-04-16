#pragma once
#include <random>
#include <chrono>
#include "Vortex/Memory/Memory.h"

namespace Vortex::Math {
	struct Random {
	public:
		explicit Random(UInt64 seed
		= static_cast<long unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()))
			: m_Seed{seed}, m_RandomDevice(m_Seed) {}
		virtual ~Random() = default;

	public:
		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
		inline T Range(T min, T max) {
			if constexpr(std::is_integral<T>::value) {
				std::uniform_int_distribution<T> distribution(min, max);
				return distribution(m_RandomDevice);
			} else {
				std::uniform_real_distribution<T> distribution(min, max);
				return distribution(m_RandomDevice);
			}
		}
		inline float Range01() { return Range<float>(0.0f, 1.0f); };

		template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
		inline T Number() {
			if constexpr(std::is_integral<T>::value) {
				std::uniform_int_distribution<T> distribution;
				return distribution(m_RandomDevice);
			} else {
				std::uniform_real_distribution<T> distribution;
				return distribution(m_RandomDevice);
			}
		}
		inline void SetSeed(UInt64 seed) {
			m_RandomDevice.seed(seed);
			m_Seed = seed;
		}
		inline UInt64 GetSeed() const { return m_Seed; }

	private:
		UInt64 m_Seed;
		std::mt19937 m_RandomDevice;
	};
}