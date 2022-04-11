#include "Vortex/Graphics/GraphicsAPI.h"

#include "../Platform/OpenGL45/OpenGL45Renderer.h"

namespace Vortex {
	Graphics::Renderer* s_Renderer = nullptr;

	void InitGraphics(GraphicsType::Enum graphics) {
		VORTEX_ASSERT(Vortex::s_Renderer == nullptr)

		if (graphics == GraphicsType::OpenGL45) {
			Vortex::s_Renderer = new Vortex::Graphics::OpenGL45Renderer();
		} else if (graphics == GraphicsType::Vulkan) {
		}
	}

	Graphics::Renderer* GetRenderer() {
		VORTEX_ASSERT(Vortex::s_Renderer != nullptr)
		return Vortex::s_Renderer;
	}

	void ShutdownGraphics() {
		VORTEX_ASSERT(Vortex::s_Renderer != nullptr)

		delete Vortex::s_Renderer;
	}
}