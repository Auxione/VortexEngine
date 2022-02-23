#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Common/Handle.h"
#include "Vortex/Core/Event.h"

namespace Vortex::Graphics {
	VORTEX_HANDLE(MonitorHandle);

	struct WindowData {
		String Title;
		Int32 Resolution[3];
		MonitorHandle Monitor;
		EventCallbackFn PushEvent;
	};

	struct MonitorData {
		String Title;
		UInt32 Size[2];
	};

	struct WindowBackend {
		virtual ~WindowBackend() = default;

	public: // Window
		virtual void Update() = 0;

		virtual void CreateWindow(const Int32* resolution, const char* title) = 0;
		virtual void Present() = 0;
		virtual void Shutdown() = 0;

		virtual void SetResolution(const Int32* resolution) = 0;
		virtual void SetTitle(const char* title) = 0;
		virtual void SetIcon(UInt16 width, UInt16 height, void* pixels) = 0;
		virtual void SetFullscreen(MonitorHandle monitor_handle) = 0;
		virtual void SetEventCallback(EventCallbackFn event_fn) = 0;
		virtual void SetCursorVisibility(bool value) = 0;

		virtual void SwapBuffers() = 0;

		virtual const WindowData& GetData() const = 0;

		//public: // Monitor
		//	virtual SizeType GetMonitorCount() const = 0;
		//	virtual const float* GetContentScale(MonitorHandle handle) const = 0;
		//	//virtual const std::vector<VideoMode>& GetVideoModes(MonitorHandle monitor_handle) const = 0;
		//	virtual float GetGamma(MonitorHandle handle) const = 0;
		//	virtual void SetGamma(MonitorHandle handle, float gamma) = 0;
		//	virtual const char* GetMonitorName(MonitorHandle handle) const = 0;
	};
}