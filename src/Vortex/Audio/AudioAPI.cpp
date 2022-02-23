#include "Vortex/Memory/HeapAllocator.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Audio/AudioAPI.h"

#include "OpenALBackend.h"

namespace Vortex {
	Audio::AudioBackend* s_AudioBackend = nullptr;

	void InitAudioBackend(AudioBackendType::Enum audio) {
		VORTEX_ASSERT(Vortex::s_AudioBackend == nullptr)

		if (audio == AudioBackendType::OpenAL13) {
			Vortex::s_AudioBackend = GetHeapAllocator()->New<Vortex::Audio::OpenALBackend>();
		}
	}
	Audio::AudioBackend* GetAudioBackend() {
		VORTEX_ASSERT(Vortex::s_AudioBackend != nullptr)
		return Vortex::s_AudioBackend;
	}
	void ShutdownAudioBackend() {
		VORTEX_ASSERT(Vortex::s_AudioBackend != nullptr)

		GetHeapAllocator()->Delete(Vortex::s_AudioBackend);
	}
}