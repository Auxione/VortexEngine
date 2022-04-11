#pragma once
#include "Vortex/Memory/Memory.h"

#include "Vortex/Graphics/Renderer.h"

namespace Vortex {
	namespace GraphicsType {
		enum Enum {
			None = 0,
			OpenGL45,
			Vulkan,

			Count
		};
		constexpr static const char* ToString[]{
			"None"
			, "OpenGL45"
			, "Vulkan"
		};
	}

	void InitGraphics(GraphicsType::Enum graphics);
	Graphics::Renderer* GetRenderer();
	void ShutdownGraphics();
}