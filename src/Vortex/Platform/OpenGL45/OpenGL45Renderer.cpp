#include "OpenGL45Renderer.h"

#include "Vortex/Common/Console.h"

constexpr static const char* TranslateErrorCode(GLenum error) {
	switch (error) {
		default: return "Unknown opengl error";
		case GL_INVALID_ENUM: return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE: return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		case GL_OUT_OF_MEMORY: return "GL_OUT_OF_MEMORY";
		case GL_NO_ERROR: return "GL_NO_ERROR";
	}
}

constexpr static const char* GetFrameBufferStatus(GLenum error) {
	switch (error) {
		default: return "Unknown opengl error";
		case GL_FRAMEBUFFER_COMPLETE  : return "GL_FRAMEBUFFER_COMPLETE";
		case GL_FRAMEBUFFER_UNDEFINED : return "GL_FRAMEBUFFER_UNDEFINED";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT : return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT : return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER : return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER : return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
		case GL_FRAMEBUFFER_UNSUPPORTED  : return "GL_FRAMEBUFFER_UNSUPPORTED";
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE   : return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS    : return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
	}
}

namespace Vortex::Graphics {
	OpenGL45Renderer::OpenGL45Renderer()
		: m_FirstTime{true},
		  m_CurentWindowContext{nullptr} {
		if (!glfwInit()) {
			Console::WriteError("[GLFW] Error initializing backend.");
			VORTEX_ASSERT(false)
			return;
		}
		//Vortex::Console::WriteError("[OpenGL] %s at %s:%i", TranslateErrorCode(error), std::filesystem::path{file}.string().c_str(), line);

		Console::WriteInfo("[GLFW] Initialized.");
		Console::WriteDebug("[GLFW] Version: %s", glfwGetVersionString());

		glfwSetErrorCallback([](int code, const char* description) {
			Console::WriteError("[GLFW] %i : %s", code, description);
		});

	}
	OpenGL45Renderer::~OpenGL45Renderer() {
#ifdef VORTEX_DEBUG
		if (!m_DataMap.d_ActiveIDs.empty()) {
			Console::WriteDebug("[Renderer] following handles was active:");
			for (auto handle : m_DataMap.d_ActiveIDs) {
				if (m_DataMap.Is<Window>(handle)) {
					Console::WriteDebug("[Renderer] Window %u", handle);
				} else if (m_DataMap.Is<Texture>(handle)) {
					Console::WriteDebug("[Renderer] Texture %u", handle);
				} else if (m_DataMap.Is<Buffer>(handle)) {
					Console::WriteDebug("[Renderer] Buffer %u", handle);
				} else if (m_DataMap.Is<Shader>(handle)) {
					Console::WriteDebug("[Renderer] Shader %u", handle);
				} else if (m_DataMap.Is<FrameBuffer>(handle)) {
					Console::WriteDebug("[Renderer] FrameBuffer %u", handle);
				} else if (m_DataMap.Is<ComputeShader>(handle)) {
					Console::WriteDebug("[Renderer] ComputeShader %u", handle);
				} else if (m_DataMap.Is<Mesh>(handle)) {
					Console::WriteDebug("[Renderer] Mesh %u", handle);
				} else if (m_DataMap.Is<Material>(handle)) {
					Console::WriteDebug("[Renderer] Material %u", handle);
				} else if (m_DataMap.Is<DrawSurface>(handle)) {
					Console::WriteDebug("[Renderer] DrawSurface %u", handle);
				} else if (m_DataMap.Is<View>(handle)) {
					Console::WriteDebug("[Renderer] View %u", handle);
				}
			}
		}
#endif
		glfwTerminate();
	}

	Handle OpenGL45Renderer::CreateWindow(const Resolution& resolution, const char* title) {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);

#ifdef VORTEX_DEBUG
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif
		glfwWindowHint(GLFW_VISIBLE, true);
		glfwWindowHint(GLFW_RESIZABLE, false);
		glfwWindowHint(GLFW_REFRESH_RATE, resolution.RefreshRate);

		GLFWwindow* window_ptr = glfwCreateWindow(resolution.Width, resolution.Height, title, nullptr, m_CurentWindowContext);
		if (window_ptr == nullptr) {
			Console::WriteError("[GLFW] Error creating window %s.", title);
			return Map::NullHandle;
		}

		if (m_FirstTime) {
			m_FirstTime = false;
			glfwMakeContextCurrent(window_ptr);
			m_CurentWindowContext = window_ptr;

			if (gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
				Console::WriteInfo("[OpenGL] Version    : %s", glGetString(GL_VERSION));
				Console::WriteInfo("[OpenGL] Vendor     : %s", glGetString(GL_VENDOR));
				Console::WriteInfo("[OpenGL] API        : %s", glGetString(GL_RENDERER));
			} else {
				Console::WriteError("[OpenGL] Failed to initialize OpenGL 4.5");
			}

#ifdef VORTEX_DEBUG
			glEnable(GL_DEBUG_OUTPUT);
			glDebugMessageCallback(
				[](GLenum source,
				   GLenum type,
				   GLuint id,
				   GLenum severity,
				   GLsizei length,
				   const GLchar* message,
				   const void* userParam
				) {

					Console::WriteError("[OpenGL] %s", message);

				}, nullptr
			);
#endif
			glCullFace(GL_BACK);
		}

		Window window{};
		window.Title.assign(title);
		window.Resolution = resolution;
		window.AdditionalData.Layout<GLFWwindow*>() = window_ptr;

		View default_view{};
		default_view.FramebufferHandle = Map::NullHandle;
		default_view.Viewport.x = 0;
		default_view.Viewport.y = 0;
		default_view.Viewport.width = resolution.Width;
		default_view.Viewport.height = resolution.Height;
		default_view.ProjectionMatrix.SetOrthographic(
			0.0f,
			static_cast<float>(resolution.Width),
			static_cast<float>(resolution.Height),
			0.0f,
			-1000.0f,
			1000.0f
		);
		default_view.ViewMatrix = Math::Matrix4::Identity();
		default_view.DepthTest = DepthTesting::Disabled;
		window.DefaultViewHandle = m_DataMap.Insert<View>(default_view);

		auto handle = m_DataMap.Insert<Window>(window);

		Console::WriteInfo("[Renderer] Window \"%s\" created. Resolution %ix%ix%iHz.", title, resolution.Width, resolution.Height, resolution.RefreshRate);

		return handle;
	}
	void OpenGL45Renderer::DestroyWindow(Handle window_handle) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))

		const auto& window = m_DataMap.Get<Window>(window_handle);
		auto* glfw_win_ptr = window.AdditionalData.Layout<GLFWwindow*>();
		delete UnionCast<WindowDataStruct*>(glfwGetWindowUserPointer(glfw_win_ptr));
		glfwDestroyWindow(glfw_win_ptr);

		m_DataMap.Destroy(window.DefaultViewHandle);
		m_DataMap.Destroy(window_handle);
	}
	void OpenGL45Renderer::SetWindowResolution(Handle window_handle, const Resolution& resolution) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))

		auto& window = m_DataMap.Get<Window>(window_handle);
		auto* glfw_window_ptr = window.AdditionalData.Layout<GLFWwindow*>();
		glfwSetWindowSize(glfw_window_ptr, resolution.Width, resolution.Height);
		if (resolution.RefreshRate != 0) {
			glfwSetWindowAttrib(glfw_window_ptr, GLFW_REFRESH_RATE, resolution.RefreshRate);
		}
		window.Resolution = resolution;
	}
	void OpenGL45Renderer::SetWindowTitle(Handle window_handle, const char* title) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))

		auto& window = m_DataMap.Get<Window>(window_handle);
		glfwSetWindowTitle(window.AdditionalData.Layout<GLFWwindow*>(), title);
		window.Title.assign(title);
	}
	void OpenGL45Renderer::SetWindowIcon(Handle window_handle, UInt16 width, UInt16 height, void* pixels) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))

		GLFWimage glfw_image;
		glfw_image.width = width;
		glfw_image.height = height;
		glfw_image.pixels = static_cast<unsigned char*>(pixels);

		auto& window = m_DataMap.Get<Window>(window_handle);
		glfwSetWindowIcon(window.AdditionalData.Layout<GLFWwindow*>(), 1, &glfw_image);
	}
	void OpenGL45Renderer::SetWindowFullscreen(Handle window_handle, Handle monitor_handle) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))

	}
	void OpenGL45Renderer::SetWindowEventCallback(Handle window_handle, EventCallbackFn event_fn) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))
		auto& window = m_DataMap.Get<Window>(window_handle);
		auto* glfw_window_ptr = window.AdditionalData.Layout<GLFWwindow*>();

		auto* window_data_str = UnionCast<WindowDataStruct*>(glfwGetWindowUserPointer(glfw_window_ptr));
		if (window_data_str == nullptr) {
			window_data_str = new WindowDataStruct();
			glfwSetWindowUserPointer(glfw_window_ptr, window_data_str);
		}

		window_data_str->event_callback_fn = event_fn;
		window_data_str->window_handle = window_handle;

		glfwSetKeyCallback(
			glfw_window_ptr, [](GLFWwindow* nwin, int key, int scancode, int action, int mods) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));

				if (action == GLFW_PRESS) {
					window_user_struct->event_callback_fn(Event::CreateKeyPress(static_cast<KeyCode::Enum>(key)));
				} else if (action == GLFW_RELEASE) {
					window_user_struct->event_callback_fn(Event::CreateKeyRelease(static_cast<KeyCode::Enum>(key)));
				} else if (action == GLFW_REPEAT) {
					window_user_struct->event_callback_fn(Event::CreateKeyRepeat(static_cast<KeyCode::Enum>(key)));
				}
			}
		);
		glfwSetCharCallback(
			glfw_window_ptr, [](GLFWwindow* nwin, unsigned int c) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));
				window_user_struct->event_callback_fn(Event::CreateCharInput(c));
			}
		);

		glfwSetMouseButtonCallback(
			glfw_window_ptr, [](GLFWwindow* nwin, int button, int action, int mods) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));

				if (action == GLFW_PRESS) {
					window_user_struct->event_callback_fn(Event::CreateMousePress(static_cast<MouseButton::Enum>(button)));
				} else if (action == GLFW_RELEASE) {
					window_user_struct->event_callback_fn(Event::CreateMouseRelease(static_cast<MouseButton::Enum>(button)));
				}
			}
		);
		glfwSetCursorPosCallback(
			glfw_window_ptr, [](GLFWwindow* nwin, double x, double y) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));
				window_user_struct->event_callback_fn(Event::CreateMouseMove(static_cast<float>(x), static_cast<float>(y)));
			}
		);
		glfwSetScrollCallback(
			glfw_window_ptr, [](GLFWwindow* nwin, double x, double y) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));
				window_user_struct->event_callback_fn(Event::CreateScrollChange(static_cast<float>(x), static_cast<float>(y)));
			}
		);

		glfwSetWindowSizeCallback(
			glfw_window_ptr, [](GLFWwindow* nwin, int width, int height) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));
				window_user_struct->event_callback_fn(Event::CreateWindowResize(window_user_struct->window_handle, width, height));
			}
		);

		glfwSetWindowFocusCallback(
			glfw_window_ptr,
			[](GLFWwindow* nwin, int focused) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));
				if (focused) {
					window_user_struct->event_callback_fn(Event::CreateWindowFocusGain(window_user_struct->window_handle));
				} else {
					window_user_struct->event_callback_fn(Event::CreateWindowFocusLost(window_user_struct->window_handle));
				}
			}
		);
		glfwSetWindowCloseCallback(
			glfw_window_ptr,
			[](GLFWwindow* nwin) {
				auto* window_user_struct = static_cast<WindowDataStruct*>(glfwGetWindowUserPointer(nwin));
				window_user_struct->event_callback_fn(Event::CreateWindowClose(window_user_struct->window_handle));
			}
		);
	}
	void OpenGL45Renderer::SetCursorVisibility(Handle window_handle, bool value) {
		VORTEX_ASSERT(d_WindowChecks(window_handle))
		auto& window = m_DataMap.Get<Window>(window_handle);
		auto* glfw_window_ptr = window.AdditionalData.Layout<GLFWwindow*>();
		if (value) {
			glfwSetInputMode(glfw_window_ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		} else {
			glfwSetInputMode(glfw_window_ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	Handle OpenGL45Renderer::CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) {
		GLuint id;
		glCreateBuffers(1, &id);

		auto gl_size = static_cast<GLsizei>(count * buffer_layout.Stride);
		glNamedBufferData(id, gl_size, data, BufferUsage::ToGLType[buffer_usage]);

		Buffer buffer{};
		buffer.BufferUsage = buffer_usage;
		buffer.Layout = buffer_layout;
		buffer.Mutable = true;
		buffer.Size = count * buffer_layout.Stride;
		buffer.AdditionalData.Layout<GLuint>() = id;

		return m_DataMap.Insert<Buffer>(buffer);
	}
	void OpenGL45Renderer::UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) {
		VORTEX_ASSERT(d_BufferChecks(buffer_handle))

		const auto& buffer = m_DataMap.Get<Buffer>(buffer_handle);
		VORTEX_ASSERT(buffer.Mutable)
		VORTEX_ASSERT(offset + data_size <= buffer.Size)

		auto id = buffer.AdditionalData.Layout<GLuint>();
		auto gl_offs = static_cast<GLsizei>(offset);
		auto gl_size = static_cast<GLsizei>(data_size);
		glNamedBufferSubData(id, gl_offs, gl_size, data);
	}
	void OpenGL45Renderer::GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) {
		VORTEX_ASSERT(d_BufferChecks(buffer_handle))

		const auto& buffer = m_DataMap.Get<Buffer>(buffer_handle);
		VORTEX_ASSERT(buffer.Mutable)
		VORTEX_ASSERT(offset + data_size <= buffer.Size)

		auto id = buffer.AdditionalData.Layout<GLuint>();
		auto gl_offs = static_cast<GLsizei>(offset);
		auto gl_size = static_cast<GLsizei>(data_size);
		glGetNamedBufferSubData(id, gl_offs, gl_size, data);
	}
	void OpenGL45Renderer::DestroyBuffer(Handle buffer_handle) {
		VORTEX_ASSERT(d_BufferChecks(buffer_handle))
		const auto& buffer = m_DataMap.Get<Buffer>(buffer_handle);

		auto id = buffer.AdditionalData.Layout<GLuint>();
		glDeleteBuffers(1, &id);
		m_DataMap.Destroy(buffer_handle);
	}

	Handle OpenGL45Renderer::CreateTexture2D(
		const Vector2HalfInt& size,
		PixelFormat::Enum format,
		const void* pixels,
		TextureLODFilter::Enum min_filter,
		TextureLODFilter::Enum mag_filter,
		TextureWrap::Enum wrap_s,
		TextureWrap::Enum wrap_t,
		bool create_mipmap
	) {
		//VORTEX_ASSERT(size[0] <= m_HardwareLimits[HardwareLimit::MaxTextureSize])
		//VORTEX_ASSERT(size[1] <= m_HardwareLimits[HardwareLimit::MaxTextureSize])

		GLuint id;
		glCreateTextures(GL_TEXTURE_2D, 1, &id);
		glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, TextureLODFilter::ToGLType[min_filter]);
		glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, TextureLODFilter::ToGLType[mag_filter]);
		glTextureParameteri(id, GL_TEXTURE_WRAP_S, TextureWrap::ToGLType[wrap_s]);
		glTextureParameteri(id, GL_TEXTURE_WRAP_T, TextureWrap::ToGLType[wrap_t]);

		if (Math::IsOdd(size[0] * size[1])) {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glPixelStorei(GL_PACK_ALIGNMENT, 1);
		} else {
			glPixelStorei(GL_UNPACK_ALIGNMENT, 2);
			glPixelStorei(GL_PACK_ALIGNMENT, 2);
		}

		glTextureStorage2D(
			id,
			1, //num levels
			PixelFormat::ToGLInternalType[format],
			size[0],
			size[1]
		);

		//swizzle data if it is alpha texture
		if (format == PixelFormat::Alpha_UI8 ||
			format == PixelFormat::Alpha_UI32 ||
			format == PixelFormat::Alpha_F32) {
			GLint swizzleMask[]{GL_ONE, GL_ONE, GL_ONE, GL_RED};
			glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
		}

		if (pixels != nullptr) {
			glTextureSubImage2D(
				id,
				0,            //level
				0,            //x_off
				0,            //y_off
				size[0],       //width
				size[1],       //height
				PixelFormat::ToGLFormatType[format],
				PixelFormat::ToGLType[format],
				pixels
			);
			if (create_mipmap) {
				glGenerateTextureMipmap(id);
			}
		}

		Texture texture{};
		texture.PixelFormat = format;
		texture.MinificationFilter = min_filter;
		texture.MagnificationFilter = mag_filter;
		texture.WrapS = wrap_s;
		texture.WrapT = wrap_t;
		texture.WrapR = TextureWrap::Count;
		texture.Size.x = size.x;
		texture.Size.y = size.y;
		texture.Size.z = 1;
		texture.LayerCount = 1;
		texture.BitsPerPixel = PixelFormat::BitsPerPixel[format];
		texture.IsCubemap = false;
		texture.Mipmapped = create_mipmap;
		texture.DataSize = size.x * size.y * texture.BitsPerPixel;
		texture.AdditionalData.Layout<GLuint>() = id;

		return m_DataMap.Insert<Texture>(texture);
	}
	void OpenGL45Renderer::UpdateTexture2D(Handle texture_handle, const void* pixels) {
		VORTEX_ASSERT(d_TextureChecks(texture_handle))
		VORTEX_ASSERT(pixels != nullptr)

		const auto& texture = m_DataMap.Get<Texture>(texture_handle);
		auto id = texture.AdditionalData.Layout<GLuint>();

		glTextureSubImage2D(
			id,
			0,            //level
			0,            //x_off
			0,            //y_off
			texture.Size[0],        //width
			texture.Size[1],       //height
			PixelFormat::ToGLFormatType[texture.PixelFormat],
			PixelFormat::ToGLType[texture.PixelFormat],
			pixels
		);

		if (texture.Mipmapped) {
			glGenerateTextureMipmap(id);
		}
	}
	void OpenGL45Renderer::GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) {
		VORTEX_ASSERT(d_TextureChecks(texture_handle))
		VORTEX_ASSERT(pixels != nullptr)

		const auto& texture = m_DataMap.Get<Texture>(texture_handle);
		auto id = texture.AdditionalData.Layout<GLuint>();

		glGetTextureImage(
			id,
			0,
			PixelFormat::ToGLInternalType[texture.PixelFormat],
			PixelFormat::ToGLType[texture.PixelFormat],
			buffer_size,
			pixels
		);
	}
	void OpenGL45Renderer::DestroyTexture(Handle texture_handle) {
		VORTEX_ASSERT(d_TextureChecks(texture_handle))
		const auto& texture = m_DataMap.Get<Texture>(texture_handle);
		auto id = texture.AdditionalData.Layout<GLuint>();
		glDeleteTextures(1, &id);

		m_DataMap.Destroy(texture_handle);
	}

	Handle OpenGL45Renderer::CreateShader(const char** sources, ShaderType::Enum* types, SizeType count, ShaderTags::Enum tags) {
		std::vector<GLuint> shader_ids;

		for (SizeType i = 0; i < count; ++i) {
			auto type = types[i];
			const char* source = sources[i];
			auto id = CompileShaderSource(source, type);
			if (id == 0) {
				return Map::NullHandle;
			}
			shader_ids.emplace_back(id);
		}

		auto gl_id = LinkShaderProgram(shader_ids.data(), shader_ids.size());
		if (gl_id == 0) {
			return Map::NullHandle;
		}

		Shader shader{};
		QueryShaderTypes(shader, gl_id);
		shader.AdditionalData.Layout<GLuint>() = gl_id;
		shader.Tags = tags;

		return m_DataMap.Insert<Shader>(shader);
	}
	bool OpenGL45Renderer::ReloadShader(Handle shader_handle, const char** sources, ShaderType::Enum* types, SizeType count) {
		VORTEX_ASSERT(d_ShaderChecks(shader_handle))

		std::vector<GLuint> shader_ids;
		for (SizeType i = 0; i < count; ++i) {
			auto type = types[i];
			const char* source = sources[i];
			auto id = CompileShaderSource(source, type);
			if (id == 0) {
				return false;
			}
			shader_ids.emplace_back(id);
		}

		auto gl_new_id = LinkShaderProgram(shader_ids.data(), shader_ids.size());
		if (gl_new_id == 0) {
			return false;
		}

		auto& shader = m_DataMap.Get<Shader>(shader_handle);
		shader.BindingPositions.clear();
		shader.Locations.clear();
		shader.Types.clear();
		QueryShaderTypes(shader, gl_new_id);
		auto gl_shader_id = shader.AdditionalData.Layout<GLuint>();
		glDeleteProgram(gl_shader_id);
		shader.AdditionalData.Layout<GLuint>() = gl_new_id;

		return true;
	}
	void OpenGL45Renderer::SetUniform(Handle shader_handle, HashedString name, const void* data, SizeType count) const {
		VORTEX_ASSERT(d_ShaderChecks(shader_handle))
		const auto& shader = m_DataMap.Get<Shader>(shader_handle);

		auto gl_program_id = shader.AdditionalData.Layout<GLuint>();

		GLint gl_location;
		auto it = shader.Locations.find(name);

		if (it == shader.Locations.end()) {
			return;
		}

		gl_location = it->second;
		ElementType::Enum type = shader.Types.at(name);

		if (type == ElementType::Sampler1D ||
			type == ElementType::Sampler2D ||
			type == ElementType::Sampler3D) {
			auto binding = shader.BindingPositions.at(name);
			auto texture_handle = *static_cast<const Handle*>(data);
			VORTEX_ASSERT(d_TextureChecks(texture_handle))

			const auto& texture = m_DataMap.Get<Texture>(texture_handle);

			auto gl_texture_id = texture.AdditionalData.Layout<GLuint>();
			glBindTextureUnit(binding, gl_texture_id);

		} else if (type == ElementType::Image1D ||
			type == ElementType::Image2D ||
			type == ElementType::Image3D) {
			auto binding = shader.BindingPositions.at(name);
			auto texture_handle = *static_cast<const Handle*>(data);
			VORTEX_ASSERT(d_TextureChecks(texture_handle))

			const auto& texture = m_DataMap.Get<Texture>(texture_handle);

			auto gl_texture_id = texture.AdditionalData.Layout<GLuint>();
			glBindImageTexture(
				binding,
				gl_texture_id,
				0,
				GL_FALSE,
				0,
				GL_READ_WRITE,
				PixelFormat::ToGLImageTexture[texture.PixelFormat]
			);
		} else if (type == ElementType::UniformBlock) {
			auto binding = shader.BindingPositions.at(name);
			auto buffer_handle = *static_cast<const Handle*>(data);
			VORTEX_ASSERT(d_BufferChecks(buffer_handle))

			const auto& buffer = m_DataMap.Get<Buffer>(buffer_handle);

			auto gl_buffer_id = buffer.AdditionalData.Layout<GLuint>();

			glUniformBlockBinding(gl_program_id, gl_location, binding);
			glBindBufferBase(GL_UNIFORM_BUFFER, binding, gl_buffer_id);
		} else if (type == ElementType::StorageBlock) {
			auto binding = shader.BindingPositions.at(name);
			auto buffer_handle = *static_cast<const Handle*>(data);
			VORTEX_ASSERT(d_BufferChecks(buffer_handle))

			const auto& buffer = m_DataMap.Get<Buffer>(buffer_handle);

			auto gl_buffer_id = buffer.AdditionalData.Layout<GLuint>();

			glShaderStorageBlockBinding(gl_program_id, gl_location, binding);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, gl_buffer_id);
		} else {
			auto gl_count = static_cast<GLsizei>(count);
			switch (type) {
				case ElementType::Float1: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniform1fv(gl_program_id, gl_location, gl_count, float_ptr);
					break;
				}
				case ElementType::Float2: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniform2fv(gl_program_id, gl_location, gl_count, float_ptr);
					break;
				}
				case ElementType::Float3: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniform3fv(gl_program_id, gl_location, gl_count, float_ptr);
					break;
				}
				case ElementType::Float4: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniform4fv(gl_program_id, gl_location, gl_count, float_ptr);
					break;
				}

				case ElementType::Matrix2: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniformMatrix2fv(gl_program_id, gl_location, gl_count, false, float_ptr);
					break;
				}
				case ElementType::Matrix3: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniformMatrix3fv(gl_program_id, gl_location, gl_count, false, float_ptr);
					break;
				}
				case ElementType::Matrix4: {
					const auto* float_ptr = static_cast<const float*>(data);
					glProgramUniformMatrix4fv(gl_program_id, gl_location, gl_count, false, float_ptr);
					break;
				}

				case ElementType::Int1: {
					const auto* int_ptr = static_cast<const Int32*>(data);
					glProgramUniform1iv(gl_program_id, gl_location, gl_count, int_ptr);
					break;
				}
				case ElementType::Int2: {
					const auto* int_ptr = static_cast<const Int32*>(data);
					glProgramUniform2iv(gl_program_id, gl_location, gl_count, int_ptr);
					break;
				}
				case ElementType::Int3: {
					const auto* int_ptr = static_cast<const Int32*>(data);
					glProgramUniform3iv(gl_program_id, gl_location, gl_count, int_ptr);
					break;
				}
				case ElementType::Int4: {
					const auto* int_ptr = static_cast<const Int32*>(data);
					glProgramUniform4iv(gl_program_id, gl_location, gl_count, int_ptr);
					break;
				}

				case ElementType::UInt1: {
					const auto* uint_ptr = static_cast<const UInt32*>(data);
					glProgramUniform1uiv(gl_program_id, gl_location, gl_count, uint_ptr);
					break;
				}
				case ElementType::UInt2: {
					const auto* uint_ptr = static_cast<const UInt32*>(data);
					glProgramUniform2uiv(gl_program_id, gl_location, gl_count, uint_ptr);
					break;
				}
				case ElementType::UInt3: {
					const auto* uint_ptr = static_cast<const UInt32*>(data);
					glProgramUniform3uiv(gl_program_id, gl_location, gl_count, uint_ptr);
					break;
				}
				case ElementType::UInt4: {
					const auto* uint_ptr = static_cast<const UInt32*>(data);
					glProgramUniform4uiv(gl_program_id, gl_location, gl_count, uint_ptr);
					break;
				}

				case ElementType::FloatingTypeCount:

				case ElementType::Byte1:
				case ElementType::Byte2:
				case ElementType::Byte3:
				case ElementType::Byte4:
				case ElementType::UByte1:
				case ElementType::UByte2:
				case ElementType::UByte3:
				case ElementType::UByte4:
				case ElementType::Short1:
				case ElementType::Short2:
				case ElementType::Short3:
				case ElementType::Short4:
				case ElementType::UShort1:
				case ElementType::UShort2:
				case ElementType::UShort3:
				case ElementType::UShort4:

				case ElementType::IntegralTypeCount:

				case ElementType::Sampler1D:
				case ElementType::Sampler2D:
				case ElementType::Sampler3D:

				case ElementType::SamplerTypeCount:

				case ElementType::Image1D:
				case ElementType::Image2D:
				case ElementType::Image3D:

				case ElementType::ImageTypeCount:

				case ElementType::UniformBlock:
				case ElementType::StorageBlock:

				case ElementType::Count:break;
			}
		}
	}
	void OpenGL45Renderer::DestroyShader(Handle shader_handle) {
		VORTEX_ASSERT(d_ShaderChecks(shader_handle))
		const auto& shader = m_DataMap.Get<Shader>(shader_handle);

		auto id = shader.AdditionalData.Layout<GLuint>();
		glDeleteProgram(id);
		m_DataMap.Destroy(shader_handle);
	}
	GLuint OpenGL45Renderer::CompileShaderSource(const char* source, ShaderType::Enum type) {
		GLuint gl_id{0};
		gl_id = glCreateShader(ShaderType::ToGLType[type]);
		glShaderSource(gl_id, 1, &source, 0);
		glCompileShader(gl_id);

		GLint status = 0;
		glGetShaderiv(gl_id, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint max_length = 0;
			glGetShaderiv(gl_id, GL_INFO_LOG_LENGTH, &max_length);
			String err_log;
			err_log.resize(max_length);
			glGetShaderInfoLog(gl_id, max_length, &max_length, &err_log[0]);

			Console::WriteError("[OpenGL] Failed to compile %s shader:\n%s", ShaderType::ToString[type], err_log.data());

			glDeleteShader(gl_id);
			gl_id = 0;
		} else {
			Console::WriteInfo("[OpenGL] %s Shader compiled successfully.", ShaderType::ToString[type]);
		}

		return gl_id;
	}
	GLuint OpenGL45Renderer::LinkShaderProgram(GLuint* shader_ids, SizeType count) {
		GLuint gl_id;
		gl_id = glCreateProgram();
		for (SizeType i = 0; i < count; ++i) {
			glAttachShader(gl_id, shader_ids[i]);
		}

		glLinkProgram(gl_id);
		glValidateProgram(gl_id);

		GLint program_status = 0;
		glGetProgramiv(gl_id, GL_LINK_STATUS, &program_status);

		if (program_status == GL_FALSE) {
			GLint max_length = 0;
			glGetProgramiv(gl_id, GL_INFO_LOG_LENGTH, &max_length);

			String err_log;
			err_log.resize(max_length);
			glGetProgramInfoLog(gl_id, max_length, &max_length, &err_log[0]);
			Console::WriteError("[OpenGL] Program linking failed.\n%s", err_log.data());

			glDeleteProgram(gl_id);
			gl_id = 0;
		} else {
			Console::WriteDebug("[OpenGL] Program linking successful.");
		}
		for (SizeType i = 0; i < count; ++i) {
			glDeleteShader(shader_ids[i]);
		}
		return gl_id;
	}
	ElementType::Enum OpenGL45Renderer::FindType(GLenum gl_type) {
		for (int j = 0; j < ElementType::Count; ++j) {
			auto type = static_cast<ElementType::Enum>(j);
			if (ElementType::ToGLUniformType[type] == gl_type) {
				return type;
			}
		}
		return ElementType::Count;
	}
	void OpenGL45Renderer::QueryShaderTypes(Shader& data, GLuint program_id) {
		{
			//retrieve shader uniform count
			GLsizei uniform_count;
			glGetProgramiv(program_id, GL_ACTIVE_UNIFORMS, &uniform_count);

			GLint sampler_position{0};
			//Create cache for names and locations of every uniform and attribute in shader.
			for (GLsizei i = 0; i < uniform_count; i++) {
				GLsizei length;
				GLint size;
				GLenum gl_type;
				char name[128];
				glGetActiveUniform(program_id, i, 128, &length, &size, &gl_type, name);
				GLint location = glGetUniformLocation(program_id, name);

				HashedString hashed_str{name, static_cast<SizeType>(length) + 1};
				ElementType::Enum type = FindType(gl_type);

				data.Types.insert(std::pair(hashed_str, type));
				data.Locations.insert(std::pair(hashed_str, location));

				if (gl_type == GL_SAMPLER_1D || gl_type == GL_SAMPLER_2D || gl_type == GL_SAMPLER_3D
					|| gl_type == GL_IMAGE_1D || gl_type == GL_IMAGE_2D || gl_type == GL_IMAGE_3D) {
					data.BindingPositions.insert(std::pair(hashed_str, sampler_position));
					glProgramUniform1i(program_id, location, sampler_position);
					++sampler_position;
				}

				Console::WriteDebug("[OpenGL] Cached shader uniform. Location: %i Type: %s Name: %s", location, ElementType::ToString[type], name);
			}
		}
		{
			GLsizei uniform_block_count;
			int binding_position{0};
			glGetProgramiv(program_id, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_block_count);
			for (GLsizei i = 0; i < uniform_block_count; ++i) {
				GLsizei length;
				char name[128];

				glGetActiveUniformBlockName(
					program_id,
					i,
					128,
					&length,
					name
				);

				HashedString hashed_str{name, static_cast<SizeType>(length) + 1};

				ElementType::Enum type = ElementType::UniformBlock;
				GLint location = glGetUniformBlockIndex(program_id, name);

				data.Types.insert(std::pair(hashed_str, type));
				data.Locations.insert(std::pair(hashed_str, location));
				data.BindingPositions.insert(std::pair(hashed_str, binding_position));
				++binding_position;
				Console::WriteDebug("[OpenGL] Cached shader uniform block. Location: %i Type: %s Name: %s", location, ElementType::ToString[type], name);
			}
		}
		{
			GLsizei attribute_count;
			glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, &attribute_count);

			for (GLsizei i = 0; i < attribute_count; i++) {
				GLsizei length;
				GLint size;
				GLenum gl_type;
				char name[128];
				glGetActiveAttrib(program_id, i, 128, &length, &size, &gl_type, name);
				GLint location = glGetAttribLocation(program_id, name);

				HashedString hashed_str{name, static_cast<SizeType>(length) + 1};

				ElementType::Enum type = FindType(gl_type);

				data.Types.insert(std::pair(hashed_str, type));
				data.Locations.insert(std::pair(hashed_str, location));
				Console::WriteDebug("[OpenGL] Cached shader attribute: %i - %s", location, name);
			}
		}
	}

	Handle OpenGL45Renderer::CreateFrameBuffer(
		const Vector2HalfInt& size,
		const PixelFormat::Enum* attachment_formats,
		SizeType count
	) {
		VORTEX_ASSERT(attachment_formats != nullptr)
		VORTEX_ASSERT(count > 0)

		GLuint gl_framebuffer_id;
		glCreateFramebuffers(1, &gl_framebuffer_id);

		FrameBuffer frame_buffer{};
		frame_buffer.Size = size;

		std::vector<GLenum> draw_buffers;
		for (SizeType i = 0; i < count; ++i) {
			auto attachment_pixel_format = attachment_formats[i];

			auto attachment_texture_handle = CreateTexture2D(
				size,
				attachment_pixel_format,
				nullptr,
				TextureLODFilter::Linear,
				TextureLODFilter::Linear,
				TextureWrap::ClampEdge,
				TextureWrap::ClampEdge,
				true
			);
			const auto& attachment_texture = m_DataMap.Get<Texture>(attachment_texture_handle);
			auto gl_attachment_texture_id = attachment_texture.AdditionalData.Layout<GLuint>();

			GLenum current_attachment_enum = PixelFormat::ToGLFrameBufferAttachment[attachment_pixel_format];
			if (attachment_pixel_format != PixelFormat::DepthStencil
				&& attachment_pixel_format != PixelFormat::Stencil_UI8
				&& attachment_pixel_format != PixelFormat::Depth_F32) {

				current_attachment_enum += draw_buffers.size();
				draw_buffers.emplace_back(current_attachment_enum);
			}
			glNamedFramebufferTexture(gl_framebuffer_id, current_attachment_enum, gl_attachment_texture_id, 0);

			frame_buffer.TextureHandles.emplace_back(attachment_texture_handle);

			Console::WriteDebug("[OpenGL] Framebuffer: %zu Attachment: %s", gl_framebuffer_id, PixelFormat::ToString[attachment_pixel_format]);
		}

		glNamedFramebufferDrawBuffers(gl_framebuffer_id, draw_buffers.size(), draw_buffers.data());
		auto result = glCheckNamedFramebufferStatus(gl_framebuffer_id, GL_FRAMEBUFFER);

		if (result != GL_FRAMEBUFFER_COMPLETE) {
			Console::WriteDebug("[OpenGL] Incomplete Framebuffer:%s", GetFrameBufferStatus(result));
			VORTEX_ASSERT(false)
		}

		frame_buffer.AdditionalData.Layout<GLuint>() = gl_framebuffer_id;

		auto handle = m_DataMap.Insert<FrameBuffer>(frame_buffer);
		return handle;
	}
	void OpenGL45Renderer::DestroyFrameBuffer(Handle framebuffer_handle) {
		const auto& framebuffer = m_DataMap.Get<FrameBuffer>(framebuffer_handle);
		for (auto texture_id : framebuffer.TextureHandles) {
			DestroyTexture(texture_id);
		}

		auto gl_framebuffer_id = framebuffer.AdditionalData.Layout<GLuint>();
		glDeleteFramebuffers(1, &gl_framebuffer_id);
		m_DataMap.Destroy(framebuffer_handle);
	}

	Handle OpenGL45Renderer::CreateComputeShader(const char* source, OnComputeShaderBindFn on_compute_shader_bind) {
		VORTEX_ASSERT(on_compute_shader_bind != nullptr);
		ShaderType::Enum type{ShaderType::Compute};
		auto shader_handle = CreateShader(&source, &type, 1, ShaderTags::Undefined);
		if (shader_handle == Map::NullHandle) {
			return Map::NullHandle;
		}

		ComputeShader compute_shader{};
		compute_shader.ShaderHandle = shader_handle;
		compute_shader.OnBind = on_compute_shader_bind;

		return m_DataMap.Insert<ComputeShader>(compute_shader);
	}
	void OpenGL45Renderer::DestroyComputeShader(Handle compute_shader_handle) {
		VORTEX_ASSERT(d_ComputeShaderChecks(compute_shader_handle))
		const auto& compute_shader = m_DataMap.Get<ComputeShader>(compute_shader_handle);

		DestroyShader(compute_shader.ShaderHandle);
		m_DataMap.Destroy(compute_shader_handle);
	}

	Handle OpenGL45Renderer::CreateMesh(
		Topology::Enum topology,
		BufferUsage::Enum usage,
		const MeshLayout& layout,
		SizeType vertex_capacity,
		SizeType index_capacity
	) {

		GLuint gl_mesh_id;
		glCreateVertexArrays(1, &gl_mesh_id);

		auto index_buffer_handle = CreateBuffer(usage, CreateBufferLayout(ElementType::UInt1), index_capacity, nullptr);
		const auto& index_buffer = m_DataMap.Get<Buffer>(index_buffer_handle);
		auto gl_index_buffer_id = index_buffer.AdditionalData.Layout<GLuint>();
		glVertexArrayElementBuffer(gl_mesh_id, gl_index_buffer_id);

		Mesh mesh{};
		mesh.IndexBufferHandle = index_buffer_handle;
		mesh.IndexCount = 0;
		mesh.IndexCapacity = index_capacity;
		mesh.Topology = topology;
		mesh.AdditionalData.Layout<GLuint>() = gl_mesh_id;

		SizeType buffer_binding_index = 0;
		for (SizeType i = 0; i < layout.Count; ++i) {
			const auto& buffer_layout = layout.BufferLayouts[i];
			auto vertex_buffer_handle = CreateBuffer(
				usage,
				buffer_layout,
				vertex_capacity,
				nullptr
			);

			const auto& buffer = m_DataMap.Get<Buffer>(vertex_buffer_handle);
			auto gl_buffer_id = buffer.AdditionalData.Layout<GLuint>();

			AddVertexBuffer(
				gl_mesh_id,
				gl_buffer_id,
				buffer.Layout,
				buffer_binding_index
			);
			mesh.BufferHandles.emplace_back(vertex_buffer_handle);

#ifdef VORTEX_DEBUG
			mesh.d_BufferSizes.emplace_back(buffer_layout.Stride * vertex_capacity);
#endif
		}

		auto handle = m_DataMap.Insert<Mesh>(mesh);

		return handle;
	}
	void OpenGL45Renderer::SetMeshIndexCount(Handle mesh_handle, SizeType count) {
		VORTEX_ASSERT(d_MeshChecks(mesh_handle))
		auto& mesh = m_DataMap.Get<Mesh>(mesh_handle);

		VORTEX_ASSERT(count <= mesh.IndexCapacity)
		mesh.IndexCount = count;
	}
	void OpenGL45Renderer::SetMeshIndices(Handle mesh_handle, const UInt32* data, SizeType count) {
		VORTEX_ASSERT(d_MeshChecks(mesh_handle))
		VORTEX_ASSERT(data != nullptr)
		auto& mesh = m_DataMap.Get<Mesh>(mesh_handle);
		VORTEX_ASSERT(count <= mesh.IndexCapacity)

		UpdateBuffer(mesh.IndexBufferHandle, 0, count * sizeof(UInt32), data);
		mesh.IndexCount = count;
	}
	void OpenGL45Renderer::SetMeshData(Handle mesh_handle, SizeType index, const void* data, SizeType size) {
		VORTEX_ASSERT(d_MeshChecks(mesh_handle))
		VORTEX_ASSERT(data != nullptr)
		auto& mesh = m_DataMap.Get<Mesh>(mesh_handle);

		VORTEX_ASSERT(index <= mesh.BufferHandles.size())
		VORTEX_ASSERT(size <= mesh.d_BufferSizes[index])

		auto attribute_handle = mesh.BufferHandles[index];

		UpdateBuffer(attribute_handle, 0, size, data);
	}
	void OpenGL45Renderer::DestroyMesh(Handle mesh_handle) {
		VORTEX_ASSERT(d_MeshChecks(mesh_handle))
		const auto& mesh = m_DataMap.Get<Mesh>(mesh_handle);

		for (auto buffer_handle : mesh.BufferHandles) {
			VORTEX_ASSERT(m_DataMap.Contains(buffer_handle))
			DestroyBuffer(buffer_handle);
		}

		auto gl_mesh_id = mesh.AdditionalData.Layout<GLuint>();
		glDeleteVertexArrays(1, &gl_mesh_id);

		DestroyBuffer(mesh.IndexBufferHandle);
		m_DataMap.Destroy(mesh_handle);
	}
	void OpenGL45Renderer::AddVertexBuffer(
		GLuint gl_mesh_id,
		GLuint gl_buffer_id,
		const BufferLayout& buffer_layout,
		SizeType& buffer_binding_index
	) {
		SizeType attribute_index = buffer_binding_index;
		for (SizeType j = 0; j < buffer_layout.Count; ++j) {
			auto element_type = buffer_layout.Types[j];
			auto element_offset = buffer_layout.Offsets[j];
			auto element_normalized = buffer_layout.Normalized[j];

			glEnableVertexArrayAttrib(gl_mesh_id, attribute_index);
			glVertexArrayAttribBinding(gl_mesh_id, attribute_index, buffer_binding_index);

			if (element_type < ElementType::FloatingTypeCount) {
				if (element_type == ElementType::Matrix2) {// matrix2 is equals 2 float2s
					for (int k = 0; k < 2; ++k) {
						glVertexArrayAttribFormat(
							gl_mesh_id,
							attribute_index,
							ElementType::ComponentCount[ElementType::Float2],
							ElementType::ToGLBufferType[ElementType::Float2],
							element_normalized,
							element_offset
						);
						++attribute_index;
					}
				} else if (element_type == ElementType::Matrix3) {// matrix3 is equals 3 float3s
					for (int k = 0; k < 3; ++k) {
						glVertexArrayAttribFormat(
							gl_mesh_id,
							attribute_index,
							ElementType::ComponentCount[ElementType::Float3],
							ElementType::ToGLBufferType[ElementType::Float3],
							element_normalized,
							element_offset
						);
						++attribute_index;
					}
				} else if (element_type == ElementType::Matrix4) {// matrix4 is equals 4 float4s
					for (int k = 0; k < 4; ++k) {
						glVertexArrayAttribFormat(
							gl_mesh_id,
							attribute_index,
							ElementType::ComponentCount[ElementType::Float4],
							ElementType::ToGLBufferType[ElementType::Float4],
							element_normalized,
							element_offset
						);
						++attribute_index;
					}
				} else {
					glVertexArrayAttribFormat(
						gl_mesh_id,
						attribute_index,
						ElementType::ComponentCount[element_type],
						ElementType::ToGLBufferType[element_type],
						element_normalized,
						element_offset
					);
					++attribute_index;
				}
			} else {
				glVertexArrayAttribIFormat(
					gl_mesh_id,
					attribute_index,
					ElementType::ComponentCount[element_type],
					ElementType::ToGLBufferType[element_type],
					element_offset
				);
				++attribute_index;
			}
		}

		auto gl_stride = static_cast<GLsizei>(buffer_layout.Stride);
		glVertexArrayVertexBuffer(
			gl_mesh_id,
			buffer_binding_index,
			gl_buffer_id,
			0,
			gl_stride
		);
		glVertexArrayBindingDivisor(
			gl_mesh_id,
			buffer_binding_index,
			buffer_layout.ElementPerInstance
		);
		buffer_binding_index = attribute_index;
	}
	void OpenGL45Renderer::DrawMesh(Handle mesh_handle) const {
		VORTEX_ASSERT(d_MeshChecks(mesh_handle))
		const auto& mesh = m_DataMap.Get<Mesh>(mesh_handle);
		auto gl_mesh_id = mesh.AdditionalData.Layout<GLuint>();
		auto gl_mesh_topology = Topology::ToGLType[mesh.Topology];

		glBindVertexArray(gl_mesh_id);
		glDrawElements(
			gl_mesh_topology,
			mesh.IndexCount,
			ElementType::ToGLBufferType[ElementType::UInt1],
			nullptr
		);
	}

	Handle OpenGL45Renderer::CreateMaterial(Handle shader_handle, Vortex::Graphics::Blending::Enum blending, OnMaterialBindFn on_material_bind) {
		Material material{};
		material.ShaderHandle = shader_handle;
		material.Blending = blending;
		material.OnBind = on_material_bind;
		auto handle = m_DataMap.Insert<Material>(material);
		return handle;
	}
	void OpenGL45Renderer::DestroyMaterial(Handle material_handle) {
		VORTEX_ASSERT(d_MaterialChecks(material_handle))

		const auto& material = m_DataMap.Get<Material>(material_handle);
		m_DataMap.Destroy(material_handle);
	}

	Handle OpenGL45Renderer::CreateView(
		const Math::RectangleInt& viewport,
		const Math::Matrix4& projection_matrix,
		const Math::Matrix4& view_matrix,
		DepthTesting::Enum depth_test,
		Handle framebuffer_handle
	) {
		VORTEX_ASSERT(d_FrameBufferChecks(framebuffer_handle))
		View view{};
		view.ProjectionMatrix = projection_matrix;
		view.ViewMatrix = view_matrix;
		view.FramebufferHandle = framebuffer_handle;
		view.DepthTest = depth_test;
		view.Viewport = viewport;

		auto handle = m_DataMap.Insert<View>(view);
		return handle;
	}
	void OpenGL45Renderer::DestroyView(Handle view_handle) {
		VORTEX_ASSERT(d_ViewChecks(view_handle))
		const auto& view = m_DataMap.Get<View>(view_handle);

		DestroyFrameBuffer(view.FramebufferHandle);
		m_DataMap.Destroy(view_handle);
	}

	Handle OpenGL45Renderer::CreateDrawSurface(
		const Math::RectangleInt& area,
		const Math::Color& clear_color,
		float clear_depth,
		Int32 clear_stencil
	) {
		DrawSurface surface{};
		surface.Area = area;
		surface.ClearColor = clear_color;
		surface.ClearDepth = clear_depth;
		surface.ClearStencil = clear_stencil;
		auto handle = m_DataMap.Insert<DrawSurface>(surface);
		return handle;
	}
	void OpenGL45Renderer::DestroyDrawSurface(Handle draw_surface_handle) {
		VORTEX_ASSERT(d_DrawSurfaceChecks(draw_surface_handle))
		m_DataMap.Destroy(draw_surface_handle);
	}

	void OpenGL45Renderer::NextFrame() {
		glfwPollEvents();
		if (!m_ComputeCommands.empty()) {
			ProcessComputeCommands();
			m_ComputeCommands.clear();
		}
		if (!m_DrawCommands.empty()) {
			SortDrawCommands();
			VORTEX_ASSERT(d_DrawCommandChecks())
			ProcessDrawCommands();
			m_DrawCommands.clear();
		}
	}
	void OpenGL45Renderer::OnEvent(const Event& event) {
		if (!EventType::IsWindow(event.Type)) {
			if (event.Type == EventType::WindowResize) {
				auto& view = m_DataMap.Get<View>(event.WindowResize.Handle);
				view.ProjectionMatrix.SetOrthographic(
					0.0f,
					static_cast<float>(event.WindowResize.Size.x),
					static_cast<float>(event.WindowResize.Size.y),
					0.0f,
					-1000.0f,
					1000.0f
				);
				view.Viewport.width = event.WindowResize.Size.x;
				view.Viewport.height = event.WindowResize.Size.y;
			}
		}
	}
	void OpenGL45Renderer::ProcessComputeCommands() {
		for (const auto& cmd : m_ComputeCommands) {
			const auto& compute_shader = m_DataMap.Get<ComputeShader>(cmd.ComputeShaderHandle);
			auto gl_shader_id = static_cast<GLuint>(0);// gl shader id;
			glUseProgram(gl_shader_id);

			if (compute_shader.OnBind != nullptr) {
				compute_shader.OnBind(*this, compute_shader);
			}
			glDispatchCompute(cmd.GroupCount.x, cmd.GroupCount.y, cmd.GroupCount.z);
		}
		glMemoryBarrier(GL_ALL_BARRIER_BITS);
	}
	void OpenGL45Renderer::SortDrawCommands() {
		for (auto& cmd :  m_DrawCommands) {
			ViewLayer::Enum cmd_view_layer{GetSortingKeyViewLayer(cmd.Key)};

			Handle cmd_draw_surface_handle{Map::NullHandle};
			Blending::Enum cmd_blending{Blending::Count};
			Handle cmd_material_handle{Map::NullHandle};
			GetDrawKeyData(cmd.Key, cmd_draw_surface_handle, cmd_blending, cmd_material_handle);

			//calculate depths
			if (cmd_view_layer != ViewLayer::PostProcess && m_DataMap.Is<View>(cmd.DrawHandle)) {
				Handle view_handle = cmd.DrawHandle;

				Math::Vector3 view_position;
				m_DataMap.Get<View>(view_handle).ViewMatrix.Extract3DTranslation(view_position);

				Math::Vector3 mesh_position;
				cmd.TransformMatrix.Extract3DTranslation(mesh_position);

				float distance{Math::Vector3::Distance(mesh_position, view_position)};

				SetDrawKeyDepth(cmd.Key, static_cast<UInt16>(distance));

#ifdef VORTEX_DEBUG
				cmd.d_Depth = static_cast<UInt16>(distance);
#endif
			}
		}

		std::sort(
			m_DrawCommands.begin(), m_DrawCommands.end(),
			[](const DrawCommand& lhs, const DrawCommand& rhs) {
				if (lhs.DrawHandle < rhs.DrawHandle) {
					return false;
				} else {
					return lhs.Key < rhs.Key;
				}
			}
		);
	}

	void OpenGL45Renderer::ProcessDrawCommands() {
		Handle current_draw_handle{Map::NullHandle};

		Handle current_view_handle{Map::NullHandle};
		Math::Matrix4 current_view_matrix = Math::Matrix4::Identity();
		Math::Matrix4 current_projection_matrix = Math::Matrix4::Identity();
		Math::RectangleInt current_viewport{0, 0, 0, 0};

		Handle current_draw_surface_handle{Map::NullHandle};
		ViewLayer::Enum current_view_layer{ViewLayer::Count};
		Blending::Enum current_blending{Blending::Count};
		Handle current_material_handle{Map::NullHandle};
		Handle current_material_shader_handle{Map::NullHandle};

		GLFWwindow* glfw_window_ptr;

		for (const auto& cmd :  m_DrawCommands) {
			//draw handle is either window or view
			if (current_draw_handle != cmd.DrawHandle) {
				current_draw_handle = cmd.DrawHandle;

				// get view handle from window or command
				if (m_DataMap.Is<Window>(cmd.DrawHandle)) {
					// - Window
					//		Set render context to window

					const auto& window = m_DataMap.Get<Window>(cmd.DrawHandle);
					glfw_window_ptr = window.AdditionalData.Layout<GLFWwindow*>();
					if (m_CurentWindowContext != glfw_window_ptr) {
						glfwSwapBuffers(m_CurentWindowContext);
						m_CurentWindowContext = glfw_window_ptr;
					}

					current_view_handle = window.DefaultViewHandle;
				} else {
					current_view_handle = cmd.DrawHandle;
				}

				// - View
				//		Bind Framebuffer
				//		Store ProjectionMatrix
				//		Store Viewport

				const auto& view = m_DataMap.Get<View>(current_view_handle);
				current_viewport = view.Viewport;
				current_projection_matrix = view.ProjectionMatrix;

				if (view.FramebufferHandle != Map::NullHandle) {
					const auto& framebuffer = m_DataMap.Get<FrameBuffer>(view.FramebufferHandle);
					auto gl_framebuffer_id = framebuffer.AdditionalData.Layout<GLuint>();
					glBindFramebuffer(GL_FRAMEBUFFER, gl_framebuffer_id);
					glDisable(GL_SCISSOR_TEST);
				} else {
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					glEnable(GL_SCISSOR_TEST);
				}

				glViewport(
					current_viewport.x,
					current_viewport.y,
					current_viewport.width,
					current_viewport.height
				);

				//Set DepthTesting
				if (view.DepthTest == DepthTesting::Disabled) {
					glDisable(GL_DEPTH_TEST);
				} else {
					glEnable(GL_DEPTH_TEST);
					glDepthMask(GL_TRUE);
					glDepthFunc(DepthTesting::ToGLType[view.DepthTest]);
				}
			}

			ViewLayer::Enum cmd_view_layer{GetSortingKeyViewLayer(cmd.Key)};

			// - ViewLayer
			//		Set View Matrix
			//		Apply PostProcess
			if (current_view_layer != cmd_view_layer) {
				current_view_layer = cmd_view_layer;

				if (current_view_layer == ViewLayer::HUD) {
					current_view_matrix = Vortex::Math::Matrix4::Identity();

				} else if (current_view_layer == ViewLayer::World) {
					const auto& view = m_DataMap.Get<View>(current_view_handle);
					current_view_matrix = view.ViewMatrix;
					current_view_matrix.Invert();

				} else if (current_view_layer == ViewLayer::PostProcess) {
					Handle cmd_post_process_handle{Map::NullHandle};
					GetPostProcessKeyData(cmd.Key, cmd_post_process_handle);
					/*
					const auto& post_process = m_DataMap.Get<PostProcess>(cmd_post_process_handle);

					auto gl_shader_id = *UnionCast<GLuint*>(post_process_shader.AdditionalData);
					VORTEX_GL_CALL(glUseProgram(gl_shader_id));

					PostProcessBinder post_process_binder{this, post_process};
					post_process.OnBind(post_process_binder);*/

					continue;
				}
			}

			Handle cmd_draw_surface_handle{Map::NullHandle};
			Blending::Enum cmd_blending{Blending::Count};
			Handle cmd_material_handle{Map::NullHandle};
			GetDrawKeyData(cmd.Key, cmd_draw_surface_handle, cmd_blending, cmd_material_handle);

			// - DrawingSurface
			//		Clear Color, Depth, Stencil
			//		Set Draw area
			if (current_draw_surface_handle != cmd_draw_surface_handle) {
				current_draw_surface_handle = cmd_draw_surface_handle;

				const auto& draw_surface = m_DataMap.Get<DrawSurface>(cmd_draw_surface_handle);
				glColorMask(
					GL_TRUE,
					GL_TRUE,
					GL_TRUE,
					GL_TRUE
				);
				glClearColor(
					draw_surface.ClearColor.r,
					draw_surface.ClearColor.g,
					draw_surface.ClearColor.b,
					draw_surface.ClearColor.a
				);

				glScissor(
					draw_surface.Area.x,
					//translate scissor coordinates to top left origin
					current_viewport.height - draw_surface.Area.height - draw_surface.Area.y,
					draw_surface.Area.width,
					draw_surface.Area.height
				);

				glDepthMask(GL_TRUE);
				glClearDepthf(draw_surface.ClearDepth);

				glStencilMask(GL_TRUE);
				glClearStencil(draw_surface.ClearStencil);

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			}

			// - Material
			//		Set Blending
			//		SetUniform: ViewMatrix
			//		SetUniform: ProjectionMatrix
			if (current_material_handle != cmd_material_handle) {
				current_material_handle = cmd_material_handle;
				const auto& material = m_DataMap.Get<Material>(cmd_material_handle);

				//Set Blending
				if (material.Blending == Blending::Opaque) {
					glDisable(GL_BLEND);
					glDepthMask(GL_TRUE);
				} else {
					glEnable(GL_BLEND);
					glDepthMask(GL_FALSE);

					if (material.Blending == Blending::Additive) {
						glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					} else if (material.Blending == Blending::Subtractive) {
						glBlendFunc(GL_DST_COLOR, GL_ZERO);
					}
				}

				current_material_shader_handle = material.ShaderHandle;
				const auto& material_shader = m_DataMap.Get<Shader>(current_material_shader_handle);
				auto gl_shader_id = material_shader.AdditionalData.Layout<GLuint>();
				glUseProgram(gl_shader_id);

				SetUniform(material.ShaderHandle, ShaderConstants::ToHashedString[ShaderConstants::View], current_view_matrix.Data, 1);
				SetUniform(material.ShaderHandle, ShaderConstants::ToHashedString[ShaderConstants::Projection], current_projection_matrix.Data, 1);
				//SetUniform(material.ShaderHandle, ShaderConstants::ToHashedString[ShaderConstants::Resolution], res, 1);

				if (material.OnBind != nullptr) {
					material.OnBind(*this, material);
				}
			}

			// - Draw Mesh
			//		SetUniform: ModelMatrix
			SetUniform(current_material_shader_handle, ShaderConstants::ToHashedString[ShaderConstants::Transform], cmd.TransformMatrix.Data, 1);
			DrawMesh(cmd.MeshHandle);
		}
		glfwSwapBuffers(m_CurentWindowContext);
	}
}