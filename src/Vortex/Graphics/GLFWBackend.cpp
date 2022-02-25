#include "Vortex/Debug/Assert.h"
#include "Vortex/Common/Logger.h"

#include "GLFWBackend.h"

namespace Vortex::Graphics {
	void GLFWBackend::RegisterCallbacks(GLFWwindow* ptr) {
		glfwSetWindowUserPointer(ptr, this);

		glfwSetKeyCallback(ptr, [](GLFWwindow* nwin, int key, int scancode, int action, int mods) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			auto& data = backend->m_WindowData;
			if (data.PushEvent != nullptr) {
				if (action == GLFW_PRESS) {
					data.PushEvent(Event::CreateKeyPress(static_cast<KeyCode::Enum>(key)));
				} else if (action == GLFW_RELEASE) {
					data.PushEvent(Event::CreateKeyRelease(static_cast<KeyCode::Enum>(key)));
				} else if (action == GLFW_REPEAT) {
					data.PushEvent(Event::CreateKeyRepeat(static_cast<KeyCode::Enum>(key)));
				}
			}
		});
		glfwSetCharCallback(ptr, [](GLFWwindow* nwin, unsigned int c) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			auto& data = backend->m_WindowData;
			if (data.PushEvent != nullptr) {
				data.PushEvent(Event::CreateCharInput(c));
			}
		});

		glfwSetMouseButtonCallback(ptr, [](GLFWwindow* nwin, int button, int action, int mods) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			auto& data = backend->m_WindowData;

			if (data.PushEvent != nullptr) {
				if (action == GLFW_PRESS) {
					data.PushEvent(Event::CreateMousePress(static_cast<MouseButton::Enum>(button)));
				} else if (action == GLFW_RELEASE) {
					data.PushEvent(Event::CreateMouseRelease(static_cast<MouseButton::Enum>(button)));
				}
			}
		});
		glfwSetCursorPosCallback(ptr, [](GLFWwindow* nwin, double x, double y) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			auto& data = backend->m_WindowData;

			if (data.PushEvent != nullptr) {
				data.PushEvent(Event::CreateMouseMove(static_cast<float>(x), static_cast<float>(y)));
			}
		});
		glfwSetScrollCallback(ptr, [](GLFWwindow* nwin, double x, double y) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			auto& data = backend->m_WindowData;

			if (data.PushEvent != nullptr) {
				data.PushEvent(Event::CreateScrollChange(static_cast<float>(x), static_cast<float>(y)));
			}
		});

		glfwSetWindowSizeCallback(ptr, [](GLFWwindow* nwin, int width, int height) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			auto& data = backend->m_WindowData;
			data.Resolution[0] = width;
			data.Resolution[1] = height;

			if (data.PushEvent != nullptr) {
				data.PushEvent(Event::CreateWindowResize(width, height));
			}
		});

		glfwSetWindowFocusCallback(ptr, [](GLFWwindow* nwin, int focused) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			if (backend->m_WindowData.PushEvent != nullptr) {
				if (focused) {
					backend->m_WindowData.PushEvent(Event::CreateApplicationFocusGain());
				} else {
					backend->m_WindowData.PushEvent(Event::CreateApplicationFocusLost());
				}
			}
		});
		glfwSetWindowCloseCallback(ptr, [](GLFWwindow* nwin) {
			auto* backend = static_cast<GLFWBackend*>(glfwGetWindowUserPointer(nwin));
			if (backend->m_WindowData.PushEvent != nullptr) {
				backend->m_WindowData.PushEvent(Event::CreateApplicationClose());
			}
		});
	}

	GLFWwindow* GLFWBackend::CreateWindowImpl(GraphicsBackendType::Enum type, const Int32* resolution, const char* title) {
		if (type == GraphicsBackendType::OpenGL45) {
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

#ifdef VORTEX_DEBUG
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		} else {
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		glfwWindowHint(GLFW_VISIBLE, false);
		glfwWindowHint(GLFW_RESIZABLE, false);
		glfwWindowHint(GLFW_REFRESH_RATE, resolution[2]);

		GLFWwindow* window = glfwCreateWindow(resolution[0], resolution[1], title, nullptr, nullptr);
		if (window == nullptr) {
			VORTEX_LOG_ERROR("[GLFW] Error creating window %s.", title);
			VORTEX_ASSERT(false)
			return nullptr;
		}
		glfwMakeContextCurrent(window);

		VORTEX_LOG_INFO("[GLFW] Window \"%s\" created. Resolution %ix%i %iHz.", title, resolution[0], resolution[1], resolution[2]);
		return window;
	}

	GLFWBackend::GLFWBackend(GraphicsBackendType::Enum graphics, const Int32* resolution, const char* title)
		: m_NativePtr{nullptr},
		  m_GraphicsBackendType{graphics} {

		if (!glfwInit()) {
			VORTEX_LOG_ERROR("[GLFW] Error initializing backend.");
			VORTEX_ASSERT(false)
			return;
		}

		VORTEX_LOG_INFO("[GLFW] Initialized.");
		VORTEX_LOG_DEBUG("[GLFW] Version: %s", glfwGetVersionString())

		glfwSetErrorCallback([](int code, const char* description) {
			VORTEX_LOG_ERROR("[GLFW] %i : %s", code, description);
		});

		m_NativePtr = CreateWindowImpl(graphics, resolution, title);

		RegisterCallbacks(m_NativePtr);

		m_WindowData.Resolution[0] = resolution[0]; // width
		m_WindowData.Resolution[1] = resolution[1]; // height
		m_WindowData.Resolution[2] = resolution[2]; // refreshrate
		m_WindowData.Title.assign(title);
	}

	GLFWBackend::~GLFWBackend() {
		glfwTerminate();
	}

	void GLFWBackend::Update() {
		glfwPollEvents();
	}

	void GLFWBackend::CreateWindow(const Int32* resolution, const char* title) {
		if (m_NativePtr == nullptr) {
			m_NativePtr = CreateWindowImpl(m_GraphicsBackendType, resolution, title);
			m_WindowData.Resolution[0] = resolution[0]; // width
			m_WindowData.Resolution[1] = resolution[1]; // height
			m_WindowData.Resolution[2] = resolution[2]; // refreshrate
			m_WindowData.Title.assign(title);
		}
	}

	void GLFWBackend::Present() {
		glfwShowWindow(m_NativePtr);
	}

	void GLFWBackend::Shutdown() {
		if (m_NativePtr != nullptr) {
			glfwDestroyWindow(m_NativePtr);
			m_NativePtr = nullptr;
		}
	}

	void GLFWBackend::SetResolution(const Int32* resolution) {
		glfwSetWindowSize(m_NativePtr, resolution[0], resolution[1]);
		glfwSetWindowAttrib(m_NativePtr, GLFW_REFRESH_RATE, resolution[2]);

		for (int i = 0; i < 3; ++i) {
			m_WindowData.Resolution[i] = resolution[i];
		}
	}
	void GLFWBackend::SetTitle(const char* title) {
		glfwSetWindowTitle(m_NativePtr, title);
		m_WindowData.Title.assign(title);
	}
	void GLFWBackend::SetIcon(UInt16 width, UInt16 height, void* pixels) {
		GLFWimage img;
		img.width = width;
		img.height = height;
		img.pixels = static_cast<unsigned char*>(pixels);

		glfwSetWindowIcon(m_NativePtr, 1, &img);
	}

	void GLFWBackend::SetFullscreen(MonitorHandle monitor_handle) {

	}
	void GLFWBackend::SetEventCallback(EventCallbackFn event_fn) {
		m_WindowData.PushEvent = event_fn;
	}
	void GLFWBackend::SetCursorVisibility(bool value){
		if(value){
			glfwSetInputMode(m_NativePtr,GLFW_CURSOR,GLFW_CURSOR_NORMAL);
		}else{
			glfwSetInputMode(m_NativePtr,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
		}
	}

	void GLFWBackend::SwapBuffers() {
		glfwSwapBuffers(m_NativePtr);
	}
}