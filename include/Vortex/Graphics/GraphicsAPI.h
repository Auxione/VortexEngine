#pragma once
#include "Vortex/Memory/Memory.h"

#include "Vortex/Graphics/RenderBackend.h"
#include "Vortex/Graphics/WindowBackend.h"
#include "Vortex/Graphics/Renderer.h"

namespace Vortex {
	namespace GraphicsBackendType {
		enum Enum {
			None = 0,
			OpenGL45,

			Count
		};
		constexpr static const char* ToString[]{
			"None"
			, "OpenGL45"
		};
	}

	void InitGraphicsBackend(GraphicsBackendType::Enum graphics, const Int32* resolution, const char* title);
	Graphics::WindowBackend* GetWindowBackend();
	Graphics::RenderBackend* GetRenderBackend();
	Graphics::Renderer* GetRenderer();
	void ShutdownGraphicsBackend();
}