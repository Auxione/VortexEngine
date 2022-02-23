#pragma once
#include "Vortex/Memory/Memory.h"

#include "Vortex/Audio/AudioBackend.h"

namespace Vortex {
	namespace AudioBackendType {
		enum Enum {
			None = 0,
			OpenAL13,

			Count
		};
		constexpr static const char* ToString[]{
			"None"
			, "OpenAL13"
		};
	};

	void InitAudioBackend(AudioBackendType::Enum audio);
	Audio::AudioBackend* GetAudioBackend();
	void ShutdownAudioBackend();
}