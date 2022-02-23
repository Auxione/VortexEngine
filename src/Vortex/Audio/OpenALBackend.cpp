#include "Vortex/Audio/OpenALBackend.h"
#include "Vortex/Common/Logger.h"

#ifdef VORTEX_DEBUG
  #define VORTEX_WRAP_AL_CALLS
#endif

#ifdef VORTEX_WRAP_AL_CALLS
constexpr static const char* TranslateErrorCode(ALenum error) {
	switch (error) {
		default: return "Unknown openal error";
		case AL_INVALID_ENUM: return "AL_INVALID_ENUM";
		case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
		case AL_INVALID_OPERATION: return "AL_INVALID_OPERATION";
		case AL_INVALID_NAME: return "AL_INVALID_NAME";
		case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
		case AL_NO_ERROR: return "AL_NO_ERROR";
	}
}

void ClearALError() {
	while (alGetError() != AL_NO_ERROR) {}
}

void LogALError(const char* file, int line) {
	ALenum error = alGetError();
	while (error != AL_NO_ERROR) {
		VORTEX_LOG_ERROR("[OpenGL] %s at %s:%i", TranslateErrorCode(error), std::filesystem::path{file}.string().c_str(), line);
		error = alGetError();
	}
}

#define VORTEX_AL_CALL(x) ClearALError();    \
x;                                        \
LogALError(__FILE__,__LINE__)
#else
#define VORTEX_AL_CALL(x) x//TODO check only out of memory errors
#endif

namespace Vortex::OpenAL {
	ALenum AudioFormat[]{
		AL_FORMAT_MONO8
		, AL_FORMAT_MONO16
		, AL_FORMAT_STEREO8
		, AL_FORMAT_STEREO16
	};

	ALenum SourceState[]{
		AL_STOPPED
		, AL_PLAYING
		, AL_PAUSED
	};

}

namespace Vortex::Audio {
	OpenALBackend::OpenALBackend() {

		m_Device = alcOpenDevice(NULL); // select the "preferred device"

		if (!m_Device) {
			VORTEX_LOG_ERROR("Failed to open default sound device: %s", alcGetString(m_Device, ALC_DEVICE_SPECIFIER));
			return;
		}
		VORTEX_AL_CALL(m_Context = alcCreateContext(m_Device, NULL));
		VORTEX_AL_CALL(alcMakeContextCurrent(m_Context));

		VORTEX_LOG_INFO("[OpenAL] Version    : %s", alGetString(AL_VERSION));
		VORTEX_LOG_INFO("[OpenAL] Vendor     : %s", alGetString(AL_VENDOR));
		VORTEX_LOG_INFO("[OpenAL] Renderer   : %s", alGetString(AL_RENDERER));
		VORTEX_LOG_DEBUG("[OpenAL] Extensions : %s", alGetString(AL_EXTENSIONS))

		ALfloat listenerOri[]{0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
		VORTEX_AL_CALL(alListener3f(AL_POSITION, 0, 0, 1.0f));
		VORTEX_AL_CALL(alListener3f(AL_VELOCITY, 0, 0, 0));
		VORTEX_AL_CALL(alListenerfv(AL_ORIENTATION, listenerOri));

		VORTEX_AL_CALL(alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED));
	}
	OpenALBackend::~OpenALBackend() {
		VORTEX_ASSERT(m_Context != nullptr)
		VORTEX_ASSERT(m_Device != nullptr)

		VORTEX_AL_CALL(alcDestroyContext(m_Context));
		VORTEX_AL_CALL(alcCloseDevice(m_Device));
	}

	BufferHandle OpenALBackend::CreateBuffer(
		AudioFormat::Enum format,
		UInt32 frequency,
		UInt32 size,
		const void* data) {
		VORTEX_ASSERT(data != nullptr)

		ALuint id;
		VORTEX_AL_CALL(alGenBuffers(1, &id));

		VORTEX_AL_CALL(alBufferData(
			id,
			OpenAL::AudioFormat[format],
			data,
			size,
			frequency
		));

		BufferHandle handle{static_cast<HandleType>(id)};
		BufferData buffer_data{frequency, format};
		m_BufferDatas[handle] = buffer_data;
		return handle;
	}
	void OpenALBackend::Destroy(BufferHandle handle) {
		if (!IsValid(handle)) {
			VORTEX_LOG_WARNING("[OpenAL] Tried to destroy invalid buffer handle.");
			return;
		}
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alDeleteBuffers(1, &id));
		m_BufferDatas.erase(handle);
	}

	SourceHandle OpenALBackend::CreateSource(
		BufferHandle buffer,
		float distance,
		const float* position,
		const float* direction) {

		ALuint id{0};
		VORTEX_AL_CALL(alGenSources(1, &id));

		float gain{1.0f};
		float pitch{1.0f};
		float velocity[3]{0.0f, 0.0f, 0.0f};
		bool looping{false};
		AudioSourceState::Enum state{AudioSourceState::Stopped};

		VORTEX_AL_CALL(alSourcef(id, AL_GAIN, gain));
		VORTEX_AL_CALL(alSourcef(id, AL_PITCH, pitch));
		VORTEX_AL_CALL(alSourcefv(id, AL_DIRECTION, direction));
		VORTEX_AL_CALL(alSourcefv(id, AL_POSITION, position));
		VORTEX_AL_CALL(alSourcefv(id, AL_VELOCITY, velocity));
		//VORTEX_AL_CALL(alSourcef(id, AL_ROLLOFF_FACTOR, m_RolloffFactor));
		VORTEX_AL_CALL(alSourcei(id, AL_LOOPING, looping));

		VORTEX_AL_CALL(alSourcef(id, AL_MAX_DISTANCE, distance));
		VORTEX_AL_CALL(alSourcef(id, AL_REFERENCE_DISTANCE, distance * 0.5f));

		auto buffer_id = static_cast<ALuint>(buffer.id);

		VORTEX_AL_CALL(alSourcei(id, AL_BUFFER, buffer_id));
		SourceHandle handle{static_cast<HandleType>(id)};


		SourceData source_data;

		source_data.Gain = gain;
		source_data.Pitch = pitch;
		std::memcpy(source_data.Direction, direction, 3 * sizeof(float));
		std::memcpy(source_data.Position, position, 3 * sizeof(float));
		std::memcpy(source_data.Velocity, velocity, 3 * sizeof(float));
		source_data.Looping = looping;
		source_data.State = state;

		m_SourceDatas[handle] = source_data;

		return handle;
	}
	void OpenALBackend::SetOrientation(SourceHandle handle, const float* forward_up) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcefv(id, AL_ORIENTATION, forward_up));
	}
	void OpenALBackend::SetPosition(SourceHandle handle, const float* position) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcefv(id, AL_POSITION, position));
	}
	void OpenALBackend::SetVelocity(SourceHandle handle, const float* velocity) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcefv(id, AL_VELOCITY, velocity));
	}
	void OpenALBackend::SetPitch(SourceHandle handle, float pitch) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcef(id, AL_PITCH, pitch));
	}
	void OpenALBackend::SetGain(SourceHandle handle, float gain) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcef(id, AL_GAIN, gain));
	}
	void OpenALBackend::SetDistance(SourceHandle handle, float distance) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcef(id, AL_MAX_DISTANCE, distance));
	}
	void OpenALBackend::SetLooping(SourceHandle handle, bool looping) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcei(id, AL_LOOPING, looping));
	}
	void OpenALBackend::Play(SourceHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcePlay(id));
	}
	void OpenALBackend::Pause(SourceHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourcePause(id));
	}
	void OpenALBackend::Stop(SourceHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourceStop(id));
	}
	void OpenALBackend::Rewind(SourceHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alSourceRewind(id));
	}
	void OpenALBackend::Destroy(SourceHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<ALuint>(handle.id);
		VORTEX_AL_CALL(alDeleteSources(1, &id));
	}

	void OpenALBackend::Update() {


	}

	void OpenALBackend::Mute() {
		VORTEX_AL_CALL(alListenerf(AL_GAIN, 0.0f));
	}
	void OpenALBackend::Unmute() {
		VORTEX_AL_CALL(alListenerf(AL_GAIN, m_ListenerGain));
	}

	void OpenALBackend::SetListenerGain(float gain) {
		VORTEX_AL_CALL(alListenerf(AL_GAIN, gain));
		m_ListenerGain = gain;
	}
	void OpenALBackend::SetListenerOrientation(const float* forward_up) {
		VORTEX_AL_CALL(alListenerfv(AL_ORIENTATION, forward_up));
	}
	void OpenALBackend::SetListenerPosition(const float* position) {
		VORTEX_AL_CALL(alListenerfv(AL_POSITION, position));
	}
	void OpenALBackend::SetListenerVelocity(const float* velocity) {
		VORTEX_AL_CALL(alListenerfv(AL_VELOCITY, velocity));
	}
}