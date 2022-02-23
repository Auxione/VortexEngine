#include "Vortex/Memory/HeapAllocator.h"
#include "Vortex/Debug/Assert.h"
#include "Vortex/Graphics/GraphicsAPI.h"

#include "OpenGL45Backend.h"
#include "GLFWBackend.h"

namespace Vortex {
	Graphics::RenderBackend* s_RenderBackend = nullptr;
	Graphics::WindowBackend* s_WindowBackend = nullptr;
	Graphics::Renderer* s_Renderer = nullptr;

	void InitGraphicsBackend(GraphicsBackendType::Enum graphics, const Int32* resolution, const char* title) {
		VORTEX_ASSERT(Vortex::s_WindowBackend == nullptr)

		if (graphics == GraphicsBackendType::OpenGL45) {
			Vortex::s_WindowBackend = GetHeapAllocator()->New<Vortex::Graphics::GLFWBackend>(graphics, resolution, title);
			//glfwGetProcAddress must be in GLFWbackend
			Vortex::s_RenderBackend = GetHeapAllocator()->New<Vortex::Graphics::OpenGL45Backend>((GLADloadproc) glfwGetProcAddress);
		}
		VORTEX_ASSERT(Vortex::s_WindowBackend != nullptr)
		VORTEX_ASSERT(Vortex::s_RenderBackend != nullptr)

		Vortex::s_Renderer = GetHeapAllocator()->New<Vortex::Graphics::Renderer>(Vortex::s_RenderBackend, resolution);
	}

	Graphics::WindowBackend* GetWindowBackend() {
		VORTEX_ASSERT(Vortex::s_WindowBackend != nullptr)
		return Vortex::s_WindowBackend;
	}
	Graphics::RenderBackend* GetRenderBackend() {
		VORTEX_ASSERT(Vortex::s_RenderBackend != nullptr)
		return Vortex::s_RenderBackend;
	}
	Graphics::Renderer* GetRenderer() {
		VORTEX_ASSERT(Vortex::s_Renderer != nullptr)
		return Vortex::s_Renderer;
	}

	void ShutdownGraphicsBackend() {
		VORTEX_ASSERT(Vortex::s_WindowBackend != nullptr)
		VORTEX_ASSERT(Vortex::s_RenderBackend != nullptr)

		GetHeapAllocator()->Delete(Vortex::s_Renderer);
		GetHeapAllocator()->Delete(Vortex::s_RenderBackend);
		GetHeapAllocator()->Delete(Vortex::s_WindowBackend);
	}
}