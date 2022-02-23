#pragma once
#include "Vortex/Common/Handle.h"

#include <unordered_map>

namespace Vortex::Audio {
	VORTEX_HANDLE(BufferHandle);
	VORTEX_HANDLE(SourceHandle);
}

VORTEX_HANDLE_HASH(Vortex::Audio::BufferHandle);
VORTEX_HANDLE_HASH(Vortex::Audio::SourceHandle);

namespace Vortex::Audio {
	namespace AudioFormat {
		enum Enum {
			Mono8 = 0,
			Mono16,
			Stereo8,
			Stereo16,
		};

		constexpr static const char* ToString[]{
			"Mono8"
			, "Mono16"
			, "Stereo8"
			, "Stereo16"
		};
	};

	namespace AudioSourceState {
		enum Enum {
			Stopped = 0,
			Playing,
			Paused,
		};
		constexpr static const char* ToString[]{
			"Stopped"
			, "Playing"
			, "Paused"
		};
	};

	struct BufferData {
		UInt32 Frequency;
		AudioFormat::Enum Format;
	};

	struct SourceData {
		float Gain;
		float Pitch;

		float Direction[3];
		float Position[3];
		float Velocity[3];

		bool Looping;
		AudioSourceState::Enum State;
	};

	struct ListenerData {
		float Gain;
		float Orientation[6];
		float Position[3];
		float Velocity[3];
	};

	struct AudioBackend {
		virtual ~AudioBackend() = default;

	public:
		virtual BufferHandle CreateBuffer(
			AudioFormat::Enum format,
			UInt32 frequency,
			UInt32 size,
			const void* data
		) = 0;
		virtual void Destroy(BufferHandle handle) = 0;

	public:
		virtual SourceHandle CreateSource(BufferHandle buffer, float distance, const float* position, const float* direction) = 0;

		virtual void SetOrientation(SourceHandle handle, const float* forward_up) = 0;
		virtual void SetPosition(SourceHandle handle, const float* position) = 0;
		virtual void SetVelocity(SourceHandle handle, const float* velocity) = 0;

		virtual void SetPitch(SourceHandle handle, float pitch) = 0;
		virtual void SetGain(SourceHandle handle, float gain) = 0;
		virtual void SetDistance(SourceHandle handle, float distance) = 0;
		virtual void SetLooping(SourceHandle handle, bool looping) = 0;

		virtual void Play(SourceHandle handle) = 0;
		virtual void Pause(SourceHandle handle) = 0;
		virtual void Stop(SourceHandle handle) = 0;
		virtual void Rewind(SourceHandle handle) = 0;

		virtual void Destroy(SourceHandle handle) = 0;

	public:
		virtual void Update() = 0;

		virtual void Mute() = 0;
		virtual void Unmute() = 0;

		virtual void SetListenerGain(float gain) = 0;
		virtual void SetListenerOrientation(const float* forward_up) = 0;
		virtual void SetListenerPosition(const float* position) = 0;
		virtual void SetListenerVelocity(const float* position) = 0;

	public:
		inline const BufferData& GetData(BufferHandle handle) const { return m_BufferDatas.at(handle); };
		inline const SourceData& GetData(SourceHandle handle) const { return m_SourceDatas.at(handle); };
		inline const ListenerData& GetListenerData() const { return m_ListenerData; };

	public:
		inline bool IsValid(BufferHandle handle) const { return handle.id != InvalidID && m_BufferDatas.find(handle) != m_BufferDatas.end(); };
		inline bool IsValid(SourceHandle handle) const { return handle.id != InvalidID && m_SourceDatas.find(handle) != m_SourceDatas.end(); };

	protected:
		std::unordered_map<BufferHandle, BufferData> m_BufferDatas;
		std::unordered_map<SourceHandle, SourceData> m_SourceDatas;
		ListenerData m_ListenerData;
	};
}