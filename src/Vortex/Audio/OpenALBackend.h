#pragma once
#include <AL/al.h>
#include <AL/alc.h>

#include "Vortex/Memory/Memory.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Audio/AudioBackend.h"

namespace Vortex::Audio {
	struct OpenALBackend: public AudioBackend {

	public:
		OpenALBackend();
		~OpenALBackend() override;

	public:
		BufferHandle CreateBuffer(
			AudioFormat::Enum format,
			UInt32 frequency,
			UInt32 size,
			const void* data) override;
		void Destroy(BufferHandle handle) override;

	public:
		SourceHandle CreateSource(BufferHandle buffer, float distance, const float* position, const float* direction) override;
		void SetOrientation(SourceHandle handle, const float* forward_up) override;
		void SetPosition(SourceHandle handle, const float* position) override;
		void SetVelocity(SourceHandle handle, const float* velocity) override;
		void SetPitch(SourceHandle handle, float pitch) override;
		void SetGain(SourceHandle handle, float gain) override;
		void SetDistance(SourceHandle handle, float distance) override;
		void SetLooping(SourceHandle handle, bool looping) override;
		void Play(SourceHandle handle) override;
		void Pause(SourceHandle handle) override;
		void Stop(SourceHandle handle) override;
		void Rewind(SourceHandle handle) override;
		void Destroy(SourceHandle handle) override;

	public:
		void Update() override;

		void Mute() override;
		void Unmute() override;

		void SetListenerGain(float gain) override;
		void SetListenerOrientation(const float* forward_up) override;
		void SetListenerPosition(const float* position) override;
		void SetListenerVelocity(const float* velocity) override;

	private:
		ALCdevice* m_Device;
		ALCcontext* m_Context;
		float m_ListenerGain;
	};
}