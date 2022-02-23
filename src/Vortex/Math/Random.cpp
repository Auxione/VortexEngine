#include "Vortex/Math/Random.h"

#include <chrono>
namespace Vortex {
	Random::Random(): Random(std::chrono::system_clock::now().time_since_epoch().count()) {}
	Random::Random(SeedType seed)
		: m_Seed{seed}, m_RandomDevice{seed} {
	}
	void Random::SetSeed(SeedType seed) {
		m_Seed = seed;
		m_RandomDevice.seed(m_Seed);
	}

	float Random::Range(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(m_RandomDevice);
	}

	Vector2 Random::UnitCircle() {
		std::uniform_real_distribution<float> distribution{-1, 1};
		return Vector2{distribution(m_RandomDevice), distribution(m_RandomDevice)};
	}
	Vector3 Random::UnitSphere() {
		std::uniform_real_distribution<float> distribution{-1, 1};
		return Vector3{distribution(m_RandomDevice), distribution(m_RandomDevice), distribution(m_RandomDevice)};
	}
}