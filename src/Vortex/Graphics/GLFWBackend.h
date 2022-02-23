#pragma once
#include "Vortex/Graphics/WindowBackend.h"
#include "Vortex/Graphics/GraphicsAPI.h"

#include <GLFW/glfw3.h>

namespace Vortex::Graphics {
	class GLFWBackend: public WindowBackend {
	public:
		GLFWBackend(GraphicsBackendType::Enum graphics, const Int32* resolution, const char* title);
		~GLFWBackend() override;

	public: // Window
		void Update() override;

		void CreateWindow(const Int32* resolution, const char* title) override;
		void Present() override;
		void Shutdown() override;

		void SetResolution(const Int32* resolution) override;
		void SetTitle(const char* title) override;
		void SetIcon(UInt16 width, UInt16 height, void* pixels) override;
		void SetFullscreen(MonitorHandle monitor_handle) override;
		void SetEventCallback(EventCallbackFn event_fn) override;
		void SetCursorVisibility(bool value) override;

		void SwapBuffers() override;

		inline const WindowData& GetData() const override { return m_WindowData; }

	protected:
		void RegisterCallbacks(GLFWwindow* ptr);
		static GLFWwindow* CreateWindowImpl(GraphicsBackendType::Enum type, const Int32* resolution, const char* title);

	private:
		GLFWwindow* m_NativePtr;
		WindowData m_WindowData;
		GraphicsBackendType::Enum m_GraphicsBackendType;
	};
}
