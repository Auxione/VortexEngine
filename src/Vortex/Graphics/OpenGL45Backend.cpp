#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vortex/Common/Logger.h"

#include "OpenGL45Backend.h"
#include "OpenGL45Enums.h"

#ifdef VORTEX_DEBUG
  #define VORTEX_WRAP_GL_CALLS
#endif

#ifdef VORTEX_WRAP_GL_CALLS

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
void ClearGLError() {
	while (glGetError() != GL_NO_ERROR) {}
}

#include <filesystem>
void LogGLError(const char* file, int line) {
	GLenum error = glGetError();
	while (error != GL_NO_ERROR) {
		VORTEX_LOG_ERROR("[OpenGL] %s at %s:%i", TranslateErrorCode(error), std::filesystem::path{file}.string().c_str(), line);
		error = glGetError();
	}
}

#define VORTEX_GL_CALL(x) ClearGLError();    \
x;                                        \
LogGLError(__FILE__,__LINE__)
#else
#define VORTEX_GL_CALL(x) x//TODO check only out of memory errors
#endif

namespace Vortex::Graphics {
	OpenGL45Backend::OpenGL45Backend()
		: m_HardwareLimits{},
		  m_ActiveStates{} {

		if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
			VORTEX_LOG_ERROR("[OpenGL] Failed to initialize OpenGL 4.5");
			return;
		}

		VORTEX_LOG_INFO("[OpenGL] Version    : %s", glGetString(GL_VERSION));
		VORTEX_LOG_INFO("[OpenGL] Vendor     : %s", glGetString(GL_VENDOR));
		VORTEX_LOG_INFO("[OpenGL] API        : %s", glGetString(GL_RENDERER));
		//VORTEX_LOG_DEBUG("[OpenGL] Extensions : %s", glGetString(GL_EXTENSIONS))

		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_SIZE, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxTextureSize])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxTextureLayers])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxTextureSamplers])));

		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxFrameBufferWidth])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxFrameBufferHeight])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxFrameBufferColorAttachments])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_DRAW_BUFFERS, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxDrawBuffers])));

		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, static_cast<GLint*>(&m_HardwareLimits[HardwareLimit::MaxVertexAttributes])));

		GLint work_group_x;
		GLint work_group_y;
		GLint work_group_z;
		VORTEX_GL_CALL(glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_group_x));
		VORTEX_GL_CALL(glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_group_y));
		VORTEX_GL_CALL(glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_group_z));

		m_HardwareLimits[HardwareLimit::MaxComputeWorkGroupX] = static_cast<int>(work_group_x);
		m_HardwareLimits[HardwareLimit::MaxComputeWorkGroupY] = static_cast<int>(work_group_y);
		m_HardwareLimits[HardwareLimit::MaxComputeWorkGroupZ] = static_cast<int>(work_group_z);

		for (int type = 0; type < HardwareLimit::Count;
			 ++type) {
			VORTEX_LOG_DEBUG("[OpenGL] %s : %i", HardwareLimit::ToString[type], m_HardwareLimits[type]);
		}

		VORTEX_LOG_INFO("[OpenGL] Initialized successfully.");
	}

	OpenGL45Backend::~OpenGL45Backend() {
		VORTEX_LOG_DEBUG("[RenderBackend] Shutdown begin.")

#ifdef VORTEX_DEBUG
		if (!m_Datas.d_ActiveIDs.empty()) {
			VORTEX_LOG_DEBUG("[RenderBackend] following handles was active:")
			for (auto handle : m_Datas.d_ActiveIDs) {
				if (m_Datas.Is<Texture>(handle)) {
					VORTEX_LOG_DEBUG("[RenderBackend] Texture %u", handle)
				} else if (m_Datas.Is<Buffer>(handle)) {
					VORTEX_LOG_DEBUG("[RenderBackend] Buffer %u", handle)
				} else if (m_Datas.Is<VertexArray>(handle)) {
					VORTEX_LOG_DEBUG("[RenderBackend] VertexArray %u", handle)
				} else if (m_Datas.Is<Shader>(handle)) {
					VORTEX_LOG_DEBUG("[RenderBackend] Shader %u", handle)
				} else if (m_Datas.Is<FrameBuffer>(handle)) {
					VORTEX_LOG_DEBUG("[RenderBackend] FrameBuffer %u", handle)
				} else if (m_Datas.Is<TimerQuery>(handle)) {
					VORTEX_LOG_DEBUG("[RenderBackend] TimerQuery %u", handle)
				}
			}
		}
#endif
		VORTEX_LOG_DEBUG("[RenderBackend] Shutdown end.")

	}

	ElementType::Enum OpenGL45Backend::FindType(GLenum gl_type) {
		for (int j = 0; j < ElementType::Count; ++j) {
			auto type = static_cast<ElementType::Enum>(j);
			if (OpenGL45::UniformElementType[type] == gl_type) {
				return type;
			}
		}
		return ElementType::Count;
	}

	Handle OpenGL45Backend::CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) {
		GLuint id;
		VORTEX_GL_CALL(glCreateBuffers(1, &id));

		auto gl_size = static_cast<GLsizei>(count * buffer_layout.GetStride());
		VORTEX_GL_CALL(glNamedBufferData(id, gl_size, data, OpenGL45::BufferUsage[buffer_usage]));

		Buffer buffer{
			buffer_usage
			, buffer_layout
			, true
			, count * buffer_layout.GetStride()
			, id
		};

		auto handle = m_Datas.Insert(buffer);

		return handle;
	}
	void OpenGL45Backend::UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) {
		VORTEX_ASSERT(d_BufferChecks(buffer_handle))
		const auto& buffer = m_Datas.Get<Buffer>(buffer_handle);
		VORTEX_ASSERT(buffer.Mutable)
		VORTEX_ASSERT(offset + data_size <= buffer.Size)

		auto id = static_cast<GLuint>(buffer.BackendID);
		auto gl_offs = static_cast<GLsizei>(offset);
		auto gl_size = static_cast<GLsizei>(data_size);
		VORTEX_GL_CALL(glNamedBufferSubData(id, gl_offs, gl_size, data));
	}
	void OpenGL45Backend::GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) {
		VORTEX_ASSERT(d_BufferChecks(buffer_handle))
		const auto& buffer = m_Datas.Get<Buffer>(buffer_handle);
		VORTEX_ASSERT(buffer.Mutable)
		VORTEX_ASSERT(offset + data_size <= buffer.Size)

		auto id = static_cast<GLuint>(buffer.BackendID);
		auto gl_offs = static_cast<GLsizei>(offset);
		auto gl_size = static_cast<GLsizei>(data_size);
		VORTEX_GL_CALL(glGetNamedBufferSubData(id, gl_offs, gl_size, data));
	}
	void OpenGL45Backend::DestroyBuffer(Handle buffer_handle) {

		VORTEX_ASSERT(d_BufferChecks(buffer_handle))
		const auto& buffer = m_Datas.Get<Buffer>(buffer_handle);

		auto id = static_cast<GLuint>(buffer.BackendID);
		VORTEX_GL_CALL(glDeleteBuffers(1, &id));
		m_Datas.Destroy(buffer_handle);
	}

	Handle OpenGL45Backend::CreateVertexArray(Topology::Enum topology) {
		GLuint id;
		VORTEX_GL_CALL(glCreateVertexArrays(1, &id));

		VertexArray vertex_array{
			RenderBackendMap::NullHandle
			, std::vector<Handle>{}
			, false
			, 0
			, topology
			, id
		};

		auto handle = m_Datas.Insert(vertex_array);

		return handle;
	}
	void OpenGL45Backend::SetIndexBuffer(Handle vertex_array_handle, Handle index_buffer_handle) {
		VORTEX_ASSERT(d_VertexArrayChecks(vertex_array_handle))
		VORTEX_ASSERT(d_BufferChecks(index_buffer_handle))

		auto& vertex_Array = m_Datas.Get<VertexArray>(vertex_array_handle);
		const auto& buffer = m_Datas.Get<Buffer>(index_buffer_handle);

		vertex_Array.Indexed = true;
		vertex_Array.IndexBufferHandle = index_buffer_handle;

		auto vao_id = static_cast<GLuint>(vertex_Array.BackendID);
		auto ibo_id = static_cast<GLuint>(buffer.BackendID);

		VORTEX_GL_CALL(glVertexArrayElementBuffer(vao_id, ibo_id));
	}
	void OpenGL45Backend::AddVertexBuffer(Handle vertex_array_handle, Handle vertex_buffer_handle) {
		VORTEX_ASSERT(d_VertexArrayChecks(vertex_array_handle))
		VORTEX_ASSERT(d_BufferChecks(vertex_buffer_handle))

		auto& vertex_Array = m_Datas.Get<VertexArray>(vertex_array_handle);
		const auto& buffer = m_Datas.Get<Buffer>(vertex_buffer_handle);

		const auto& layout = buffer.Layout;

		auto vao_id = static_cast<GLuint>(vertex_Array.BackendID);
		auto vbo_id = static_cast<GLuint>(buffer.BackendID);

		SizeType attribute_index = vertex_Array.BufferBindIndex;

		for (SizeType i = 0; i < layout.Count(); ++i) {
			auto element_type = layout[i].Type;
			auto element_offset = layout[i].Offset;

			VORTEX_GL_CALL(glEnableVertexArrayAttrib(vao_id, attribute_index));
			VORTEX_GL_CALL(glVertexArrayAttribBinding(vao_id, attribute_index, vertex_Array.BufferBindIndex));

			if (element_type < ElementType::FloatingTypeCount) {
				if (element_type == ElementType::Matrix2) {// matrix2 is equals 2 float2s
					for (int j = 0; j < 2; ++j) {
						auto element_norm = layout[i].Normalized;
						VORTEX_GL_CALL(glVertexArrayAttribFormat(
							vao_id,
							attribute_index,
							ElementType::ComponentCount[ElementType::Float2],
							OpenGL45::BufferElementType[ElementType::Float2],
							element_norm,
							element_offset
						));
						++attribute_index;
					}
				} else if (element_type == ElementType::Matrix3) {// matrix3 is equals 3 float3s
					for (int j = 0; j < 3; ++j) {
						auto element_norm = layout[i].Normalized;
						VORTEX_GL_CALL(glVertexArrayAttribFormat(
							vao_id,
							attribute_index,
							ElementType::ComponentCount[ElementType::Float3],
							OpenGL45::BufferElementType[ElementType::Float3],
							element_norm,
							element_offset
						));
						++attribute_index;
					}
				} else if (element_type == ElementType::Matrix4) {// matrix4 is equals 4 float4s
					for (int j = 0; j < 4; ++j) {
						auto element_norm = layout[i].Normalized;
						VORTEX_GL_CALL(glVertexArrayAttribFormat(
							vao_id,
							attribute_index,
							ElementType::ComponentCount[ElementType::Float4],
							OpenGL45::BufferElementType[ElementType::Float4],
							element_norm,
							element_offset
						));
						++attribute_index;
					}
				} else {
					auto element_norm = layout[i].Normalized;
					VORTEX_GL_CALL(glVertexArrayAttribFormat(
						vao_id,
						attribute_index,
						ElementType::ComponentCount[element_type],
						OpenGL45::BufferElementType[element_type],
						element_norm,
						element_offset
					));
					++attribute_index;
				}
			} else {
				VORTEX_GL_CALL(glVertexArrayAttribIFormat(
					vao_id,
					attribute_index,
					ElementType::ComponentCount[element_type],
					OpenGL45::BufferElementType[element_type],
					element_offset
				));
				++attribute_index;
			}
		}

		auto gl_stride = static_cast<GLsizei>(layout.GetStride());
		VORTEX_GL_CALL(glVertexArrayVertexBuffer(vao_id, vertex_Array.BufferBindIndex, vbo_id, 0, gl_stride));
		VORTEX_GL_CALL(glVertexArrayBindingDivisor(vao_id, vertex_Array.BufferBindIndex, layout.ElementPerInstance));
		vertex_Array.VertexBufferHandles.emplace_back(vertex_buffer_handle);
		vertex_Array.BufferBindIndex = attribute_index;
	}
	void OpenGL45Backend::Draw(Handle vertex_array_handle, SizeType count) const {
		VORTEX_ASSERT(d_VertexArrayChecks(vertex_array_handle))
		const auto& vertex_array = m_Datas.Get<VertexArray>(vertex_array_handle);
		auto gl_id = static_cast<GLuint>(vertex_array.BackendID);

		VORTEX_GL_CALL(glBindVertexArray(gl_id));
		if (vertex_array.Indexed) {
			auto index_buffer_handle = vertex_array.IndexBufferHandle;
			VORTEX_ASSERT(Contains(index_buffer_handle))

			const auto& index_buffer = m_Datas.Get<Buffer>(index_buffer_handle);
			auto type = index_buffer.Layout[0].Type;

			VORTEX_ASSERT(type == ElementType::UInt1 || type == ElementType::UShort1 || type == ElementType::UByte1)

			VORTEX_GL_CALL(glDrawElements(
				OpenGL45::Topology[vertex_array.Topology],
				count,
				OpenGL45::BufferElementType[type],
				nullptr
			));
		} else {
			VORTEX_GL_CALL(glDrawArrays(
				OpenGL45::Topology[vertex_array.Topology],
				0,
				count
			));
		}
	}
	void OpenGL45Backend::DrawInstanced(Handle vertex_array_handle, SizeType count, SizeType instance_count) const {
		VORTEX_ASSERT(d_VertexArrayChecks(vertex_array_handle))
		const auto& vertex_array = m_Datas.Get<VertexArray>(vertex_array_handle);
		auto gl_id = static_cast<GLuint>(vertex_array.BackendID);

		VORTEX_GL_CALL(glBindVertexArray(gl_id));
		if (vertex_array.Indexed) {
			auto index_buffer_handle = vertex_array.IndexBufferHandle;
			VORTEX_ASSERT(Contains(index_buffer_handle))

			const auto& index_buffer = m_Datas.Get<Buffer>(index_buffer_handle);
			auto type = index_buffer.Layout[0].Type;

			VORTEX_ASSERT(type == ElementType::UInt1 || type == ElementType::UShort1 || type == ElementType::UByte1)

			VORTEX_GL_CALL(glDrawElementsInstanced(
				OpenGL45::Topology[vertex_array.Topology],
				count,
				OpenGL45::BufferElementType[type],
				nullptr,
				instance_count
			));
		} else {
			VORTEX_GL_CALL(glDrawArraysInstanced(
				OpenGL45::Topology[vertex_array.Topology],
				0,
				count,
				instance_count
			));
		}
	}
	void OpenGL45Backend::DestroyVertexArray(Handle vertex_array_handle) {
		VORTEX_ASSERT(d_VertexArrayChecks(vertex_array_handle))
		const auto& vertex_array = m_Datas.Get<VertexArray>(vertex_array_handle);

		auto id = static_cast<GLuint>(vertex_array.BackendID);
		VORTEX_GL_CALL(glDeleteVertexArrays(1, &id));
		m_Datas.Destroy(vertex_array_handle);
	}

	Handle OpenGL45Backend::CreateTexture2D(
		const UInt16* size,
		PixelFormat::Enum format,
		const void* pixels,
		TextureLODFilter::Enum min_filter,
		TextureLODFilter::Enum mag_filter,
		TextureWrap::Enum wrap_s,
		TextureWrap::Enum wrap_t,
		bool create_mipmap
	) {

		VORTEX_ASSERT(size[0] <= m_HardwareLimits[HardwareLimit::MaxTextureSize])
		VORTEX_ASSERT(size[1] <= m_HardwareLimits[HardwareLimit::MaxTextureSize])

		GLuint id;
		VORTEX_GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &id));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, OpenGL45::TextureLODFilter[min_filter]));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, OpenGL45::TextureLODFilter[mag_filter]));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_WRAP_S, OpenGL45::TextureWrapType[wrap_s]));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_WRAP_T, OpenGL45::TextureWrapType[wrap_t]));

		VORTEX_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		VORTEX_GL_CALL(glPixelStorei(GL_PACK_ALIGNMENT, 1));

		VORTEX_GL_CALL(glTextureStorage2D(id,
										  1, //num levels
										  OpenGL45::PixelFormat::Internal[format],
										  size[0],
										  size[1]
		));

		//swizzle data if it is alpha texture
		if (format == PixelFormat::Alpha_UI8 ||
			format == PixelFormat::Alpha_UI32 ||
			format == PixelFormat::Alpha_F32) {
			GLint swizzleMask[]{GL_ONE, GL_ONE, GL_ONE, GL_RED};
			VORTEX_GL_CALL(glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
		}

		if (pixels != nullptr) {
			VORTEX_GL_CALL(glTextureSubImage2D(id,
											   0,            //level
											   0,            //x_off
											   0,            //y_off
											   size[0],       //width
											   size[1],       //height
											   OpenGL45::PixelFormat::Format[format],
											   OpenGL45::PixelFormat::Type[format],
											   pixels
			));
			if (create_mipmap) {
				VORTEX_GL_CALL(glGenerateTextureMipmap(id));
			}
		}

		Texture texture{
			format
			, size[0]
			, size[1]
			, 1
			, 1
			, PixelFormat::BitsPerPixel[format]
			, false
			, id
		};

		auto handle = m_Datas.Insert<Texture>(texture);
		return handle;
	}
	void OpenGL45Backend::UpdateTexture2D(Handle texture_handle, const void* pixels, bool create_mipmap) {
		VORTEX_ASSERT(d_TextureChecks(texture_handle))
		VORTEX_ASSERT(pixels != nullptr)

		const auto& texture = m_Datas.Get<Texture>(texture_handle);
		auto id = static_cast<GLuint>(texture.BackendID);

		VORTEX_GL_CALL(glTextureSubImage2D(id,
										   0,            //level
										   0,            //x_off
										   0,            //y_off
										   texture.Size[0],        //width
										   texture.Size[1],       //height
										   OpenGL45::PixelFormat::Format[texture.Format],
										   OpenGL45::PixelFormat::Type[texture.Format],
										   pixels
		));

		if (create_mipmap) {
			VORTEX_GL_CALL(glGenerateTextureMipmap(id));
		}
	}

	void OpenGL45Backend::GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) {
		VORTEX_ASSERT(d_TextureChecks(texture_handle))
		VORTEX_ASSERT(pixels != nullptr)

		const auto& texture = m_Datas.Get<Texture>(texture_handle);
		auto id = static_cast<GLuint>(texture.BackendID);

		VORTEX_GL_CALL(
			glGetTextureImage(
				id,
				0,
				OpenGL45::PixelFormat::Internal[texture.Format],
				OpenGL45::PixelFormat::Type[texture.Format],
				buffer_size,
				pixels
			)
		);
	}
	void OpenGL45Backend::DestroyTexture(Handle texture_handle) {
		VORTEX_ASSERT(d_TextureChecks(texture_handle))
		const auto& texture = m_Datas.Get<Texture>(texture_handle);

		auto id = static_cast<GLuint>(texture.BackendID);
		VORTEX_GL_CALL(glDeleteTextures(1, &id));
		m_Datas.Destroy(texture_handle);
	}

	Handle OpenGL45Backend::CreateShader(const char** sources, ShaderType::Enum* types, SizeType count) {
		std::vector<GLuint> shader_ids;
		shader_ids.reserve(count);
		shader_ids.resize(count);

		for (SizeType i = 0; i < count; ++i) {
			GLuint id;
			auto type = types[i];
			const char* source = sources[i];
			VORTEX_GL_CALL(id = glCreateShader(OpenGL45::ShaderType[type]));
			VORTEX_GL_CALL(glShaderSource(id, 1, &source, 0));
			VORTEX_GL_CALL(glCompileShader(id));

			GLint status = 0;
			VORTEX_GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &status));
			if (status == GL_FALSE) {
				GLint max_length = 0;
				VORTEX_GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &max_length));
				String err_log;
				err_log.resize(max_length);
				VORTEX_GL_CALL(glGetShaderInfoLog(id, max_length, &max_length, &err_log[0]));

				VORTEX_LOG_ERROR("[OpenGL] Failed to compile %s shader:\n%s", ShaderType::ToString[type], err_log.data());

				VORTEX_GL_CALL(glDeleteShader(id));
				return RenderBackendMap::NullHandle;
			} else {
				shader_ids[i] = id;
				VORTEX_LOG_INFO("[OpenGL] %s Shader compiled successfully.", ShaderType::ToString[type]);
			}
		}

		GLuint program_id;
		VORTEX_GL_CALL(program_id = glCreateProgram());

		for (SizeType i = 0; i < count; ++i) {
			VORTEX_GL_CALL(glAttachShader(program_id, shader_ids[i]));
		}

		VORTEX_GL_CALL(glLinkProgram(program_id));
		VORTEX_GL_CALL(glValidateProgram(program_id));

		GLint program_status = 0;
		VORTEX_GL_CALL(glGetProgramiv(program_id, GL_LINK_STATUS, &program_status));

		if (program_status == GL_FALSE) {
			GLint max_length = 0;
			VORTEX_GL_CALL(glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &max_length));

			String err_log;
			err_log.resize(max_length);
			VORTEX_GL_CALL(glGetProgramInfoLog(program_id, max_length, &max_length, &err_log[0]));
			VORTEX_LOG_ERROR("[OpenGL] Program linking failed.\n%s", err_log.data());

			for (auto shader_id : shader_ids) {
				VORTEX_GL_CALL(glDeleteShader(shader_id));
			}
			VORTEX_GL_CALL(glDeleteProgram(program_id));
			return RenderBackendMap::NullHandle;
		} else {
			Shader data{};
			data.BackendID = program_id;
			CreateShaderData(data);

			auto handle = m_Datas.Insert<Shader>(data);
			VORTEX_LOG_DEBUG("[OpenGL] Shader Program linking successful.")
			return handle;
		}
	}
	void OpenGL45Backend::BindShader(Handle shader_handle) {
		VORTEX_ASSERT(d_ShaderChecks(shader_handle))
		const auto& shader = m_Datas.Get<Shader>(shader_handle);
		auto id = static_cast<GLuint>(shader.BackendID);
		VORTEX_GL_CALL(glUseProgram(id));
	}
	void OpenGL45Backend::SetUniform(Handle shader_handle, HashedString name, const void* data, SizeType count) {
		VORTEX_ASSERT(d_ShaderChecks(shader_handle))
		const auto& shader = m_Datas.Get<Shader>(shader_handle);

		auto gl_program_id = static_cast<GLuint>(shader.BackendID);

		GLint gl_location;
		auto it = shader.Locations.find(name);

		if (it != shader.Locations.end()) {
			gl_location = it->second;
			ElementType::Enum type = shader.Types.at(name);

			if (type == ElementType::Sampler1D ||
				type == ElementType::Sampler2D ||
				type == ElementType::Sampler3D) {
				auto binding = shader.BindingPositions.at(name);
				auto texture_handle = *static_cast<const Handle*>(data);
				VORTEX_ASSERT(d_TextureChecks(texture_handle))

				const auto& texture = m_Datas.Get<Texture>(texture_handle);

				auto gl_texture_id = static_cast<GLuint>(texture.BackendID);
				VORTEX_GL_CALL(glBindTextureUnit(binding, gl_texture_id));

			} else if (type == ElementType::Image1D ||
				type == ElementType::Image2D ||
				type == ElementType::Image3D) {
				auto binding = shader.BindingPositions.at(name);
				auto texture_handle = *static_cast<const Handle*>(data);
				VORTEX_ASSERT(d_TextureChecks(texture_handle))

				const auto& texture = m_Datas.Get<Texture>(texture_handle);

				auto gl_texture_id = static_cast<GLuint>(texture.BackendID);
				VORTEX_GL_CALL(glBindImageTexture(
					binding,
					gl_texture_id,
					0,
					GL_FALSE,
					0,
					GL_READ_WRITE,
					OpenGL45::PixelFormat::ImageTexture[texture.Format])
				);
			} else if (type == ElementType::UniformBlock) {
				auto binding = shader.BindingPositions.at(name);
				auto buffer_handle = *static_cast<const Handle*>(data);
				VORTEX_ASSERT(d_BufferChecks(buffer_handle))

				const auto& buffer = m_Datas.Get<Buffer>(buffer_handle);

				auto gl_buffer_id = static_cast<GLuint>(buffer.BackendID);

				VORTEX_GL_CALL(glUniformBlockBinding(gl_program_id, gl_location, binding));
				VORTEX_GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, binding, gl_buffer_id));
			} else {
				switch (type) {
					case ElementType::Float1: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniform1fv(gl_program_id, gl_location, count, float_ptr));
						break;
					}
					case ElementType::Float2: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniform2fv(gl_program_id, gl_location, count, float_ptr));
						break;
					}
					case ElementType::Float3: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniform3fv(gl_program_id, gl_location, count, float_ptr));
						break;
					}
					case ElementType::Float4: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniform4fv(gl_program_id, gl_location, count, float_ptr));
						break;
					}

					case ElementType::Matrix2: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniformMatrix2fv(gl_program_id, gl_location, count, false, float_ptr));
						break;
					}
					case ElementType::Matrix3: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniformMatrix3fv(gl_program_id, gl_location, count, false, float_ptr));
						break;
					}
					case ElementType::Matrix4: {
						const auto* float_ptr = static_cast<const float*>(data);
						VORTEX_GL_CALL(glProgramUniformMatrix4fv(gl_program_id, gl_location, count, false, float_ptr));
						break;
					}

					case ElementType::Int1: {
						const auto* int_ptr = static_cast<const Int32*>(data);
						VORTEX_GL_CALL(glProgramUniform1iv(gl_program_id, gl_location, count, int_ptr));
						break;
					}
					case ElementType::Int2: {
						const auto* int_ptr = static_cast<const Int32*>(data);
						VORTEX_GL_CALL(glProgramUniform2iv(gl_program_id, gl_location, count, int_ptr));
						break;
					}
					case ElementType::Int3: {
						const auto* int_ptr = static_cast<const Int32*>(data);
						VORTEX_GL_CALL(glProgramUniform3iv(gl_program_id, gl_location, count, int_ptr));
						break;
					}
					case ElementType::Int4: {
						const auto* int_ptr = static_cast<const Int32*>(data);
						VORTEX_GL_CALL(glProgramUniform4iv(gl_program_id, gl_location, count, int_ptr));
						break;
					}

					case ElementType::UInt1: {
						const auto* uint_ptr = static_cast<const UInt32*>(data);
						VORTEX_GL_CALL(glProgramUniform1uiv(gl_program_id, gl_location, count, uint_ptr));
						break;
					}
					case ElementType::UInt2: {
						const auto* uint_ptr = static_cast<const UInt32*>(data);
						VORTEX_GL_CALL(glProgramUniform2uiv(gl_program_id, gl_location, count, uint_ptr));
						break;
					}
					case ElementType::UInt3: {
						const auto* uint_ptr = static_cast<const UInt32*>(data);
						VORTEX_GL_CALL(glProgramUniform3uiv(gl_program_id, gl_location, count, uint_ptr));
						break;
					}
					case ElementType::UInt4: {
						const auto* uint_ptr = static_cast<const UInt32*>(data);
						VORTEX_GL_CALL(glProgramUniform4uiv(gl_program_id, gl_location, count, uint_ptr));
						break;
					}

					case ElementType::Sampler1D:
					case ElementType::Sampler2D:
					case ElementType::Sampler3D:

					case ElementType::Image1D:
					case ElementType::Image2D:
					case ElementType::Image3D:

					case ElementType::SamplerTypeCount:
					case ElementType::FloatingTypeCount:
					case ElementType::IntegerTypeCount:

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
					case ElementType::Count: VORTEX_ASSERT_MSG(false, "Invalid ElementType on shaders")
						break;
				}
			}
		}
	}
	void OpenGL45Backend::Dispatch(const Vortex::Int32* num_groups) {
		VORTEX_ASSERT(num_groups[0] <= m_HardwareLimits[HardwareLimit::MaxComputeWorkGroupX])
		VORTEX_ASSERT(num_groups[1] <= m_HardwareLimits[HardwareLimit::MaxComputeWorkGroupY])
		VORTEX_ASSERT(num_groups[2] <= m_HardwareLimits[HardwareLimit::MaxComputeWorkGroupZ])

		VORTEX_GL_CALL(glDispatchCompute(num_groups[0], num_groups[1], num_groups[2]));
		VORTEX_GL_CALL(glMemoryBarrier(GL_ALL_BARRIER_BITS));
	}
	void OpenGL45Backend::DestroyShader(Handle shader_handle) {
		VORTEX_ASSERT(d_ShaderChecks(shader_handle))
		const auto& shader = m_Datas.Get<Shader>(shader_handle);

		auto id = static_cast<GLuint>(shader.BackendID);
		VORTEX_GL_CALL(glDeleteProgram(id));
		m_Datas.Destroy(shader_handle);
	}
	void OpenGL45Backend::CreateShaderData(Shader& data) {
		auto gl_id = static_cast<GLuint>(data.BackendID);

		{
			//retrieve shader uniform count
			GLsizei uniform_count;
			VORTEX_GL_CALL(glGetProgramiv(gl_id, GL_ACTIVE_UNIFORMS, &uniform_count));

			GLint sampler_position{0};
			//Create cache for names and locations of every uniform and attribute in shader.
			for (GLsizei i = 0; i < uniform_count; i++) {
				GLsizei length;
				GLint size;
				GLenum gl_type;
				char name[128];
				VORTEX_GL_CALL(glGetActiveUniform(gl_id, i, 128, &length, &size, &gl_type, name));
				VORTEX_GL_CALL(GLint location = glGetUniformLocation(gl_id, name));

				HashedString hashed_str{name, static_cast<SizeType>(length) + 1};
				ElementType::Enum type = FindType(gl_type);

				data.Types[hashed_str] = type;
				data.Locations[hashed_str] = location;

				if (gl_type == GL_SAMPLER_1D || gl_type == GL_SAMPLER_2D || gl_type == GL_SAMPLER_3D) {
					data.BindingPositions[hashed_str] = sampler_position;
					VORTEX_GL_CALL(glProgramUniform1i(gl_id, location, sampler_position));
					++sampler_position;
				} else if (gl_type == GL_IMAGE_1D || gl_type == GL_IMAGE_2D || gl_type == GL_IMAGE_3D) {
					data.BindingPositions[hashed_str] = sampler_position;
					VORTEX_GL_CALL(glProgramUniform1i(gl_id, location, sampler_position));
					++sampler_position;
				}

				VORTEX_LOG_DEBUG("[OpenGL] Cached shader uniform. Location: %i Type: %s Name: %s", location, ElementType::ToString[type], name)
			}
		}
		{
			GLsizei uniform_block_count;
			int binding_position{0};
			VORTEX_GL_CALL(glGetProgramiv(gl_id, GL_ACTIVE_UNIFORM_BLOCKS, &uniform_block_count));
			for (GLsizei i = 0; i < uniform_block_count; ++i) {
				GLsizei length;
				char name[128];

				VORTEX_GL_CALL(glGetActiveUniformBlockName(
					gl_id,
					i,
					128,
					&length,
					name));

				HashedString hashed_str{name, static_cast<SizeType>(length) + 1};

				ElementType::Enum type = ElementType::UniformBlock;
				VORTEX_GL_CALL(GLint location = glGetUniformBlockIndex(gl_id, name));

				data.Types[hashed_str] = type;
				data.Locations[hashed_str] = location;
				data.BindingPositions[hashed_str] = binding_position;
				++binding_position;
				VORTEX_LOG_DEBUG("[OpenGL] Cached shader uniform block. Location: %i Type: %s Name: %s", location, ElementType::ToString[type], name)
			}
		}
		{
			GLsizei attribute_count;
			VORTEX_GL_CALL(glGetProgramiv(gl_id, GL_ACTIVE_ATTRIBUTES, &attribute_count));

			for (GLsizei i = 0; i < attribute_count; i++) {
				GLsizei length;
				GLint size;
				GLenum gl_type;
				char name[128];
				VORTEX_GL_CALL(glGetActiveAttrib(gl_id, i, 128, &length, &size, &gl_type, name));
				VORTEX_GL_CALL(GLint location = glGetAttribLocation(gl_id, name));

				HashedString hashed_str{name, static_cast<SizeType>(length) + 1};

				ElementType::Enum type = FindType(gl_type);

				data.Types[hashed_str] = type;
				data.Locations[hashed_str] = location;
				VORTEX_LOG_DEBUG("[OpenGL] Cached shader attribute: %i - %s", location, name)
			}
		}
	}

	Handle OpenGL45Backend::CreateFrameBuffer(const UInt16* size, FrameBufferAttachment::Enum* attachments, SizeType count) {
		VORTEX_ASSERT(attachments != nullptr)
		VORTEX_ASSERT(count > 0)

		GLuint gl_framebuffer_id;
		VORTEX_GL_CALL(glCreateFramebuffers(1, &gl_framebuffer_id));

		GLenum color_attachment_slot{0};

		GLenum draw_buffers[FrameBufferAttachment::ColorAttachmentCount];

		FrameBuffer frame_buffer{};
		frame_buffer.Size[0] = size[0];
		frame_buffer.Size[1] = size[1];

		for (SizeType i = 0; i < count; ++i) {
			auto attachment_type = attachments[i];
			auto attachment_texture_handle = CreateTexture2D(
				size,
				FrameBufferAttachment::Format[attachment_type],
				nullptr,
				TextureLODFilter::Linear,
				TextureLODFilter::Linear,
				TextureWrap::ClampEdge,
				TextureWrap::ClampEdge,
				false
			);
			const auto& attachment_texture = m_Datas.Get<Texture>(attachment_texture_handle);
			auto gl_attachment_texture_id = static_cast<GLuint>(attachment_texture.BackendID);

			GLenum current_attachment_enum;
			if (attachment_type < FrameBufferAttachment::ColorAttachmentCount) {
				current_attachment_enum = OpenGL45::FrameBufferAttachment[attachment_type] + color_attachment_slot;
				draw_buffers[color_attachment_slot] = current_attachment_enum;
				++color_attachment_slot;
			} else {
				current_attachment_enum = OpenGL45::FrameBufferAttachment[attachment_type];
			}
			VORTEX_GL_CALL(glNamedFramebufferTexture(gl_framebuffer_id, current_attachment_enum, gl_attachment_texture_id, 0));
			frame_buffer.Textures[attachment_type] = attachment_texture_handle;

			VORTEX_LOG_DEBUG("[OpenGL] Framebuffer: %zu Attachment: %s Texture: %zu", gl_framebuffer_id, FrameBufferAttachment::ToString[attachment_type], attachment_texture.BackendID)
		}

		VORTEX_GL_CALL(glNamedFramebufferDrawBuffers(gl_framebuffer_id, color_attachment_slot, draw_buffers));
		VORTEX_GL_CALL(auto result = glCheckNamedFramebufferStatus(gl_framebuffer_id, GL_FRAMEBUFFER));

		if (result != GL_FRAMEBUFFER_COMPLETE) {
			VORTEX_LOG_DEBUG("[OpenGL] Incomplete Framebuffer:%s", GetFrameBufferStatus(result))
		}

		VORTEX_ASSERT(result == GL_FRAMEBUFFER_COMPLETE)
		frame_buffer.BackendID = gl_framebuffer_id;

		auto handle = m_Datas.Insert<FrameBuffer>(frame_buffer);
		return handle;
	}
	void OpenGL45Backend::BindFrameBuffer(Handle framebuffer_handle) {
		VORTEX_ASSERT(d_FrameBufferChecks(framebuffer_handle))
		const auto& framebuffer = m_Datas.Get<FrameBuffer>(framebuffer_handle);

		auto id = static_cast<GLuint>(framebuffer.BackendID);
		VORTEX_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
	}
	void OpenGL45Backend::BindBackBuffer() {
		VORTEX_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void OpenGL45Backend::DestroyFrameBuffer(Handle framebuffer_handle) {
		VORTEX_ASSERT(d_FrameBufferChecks(framebuffer_handle))
		const auto& frame_buffer = m_Datas.Get<FrameBuffer>(framebuffer_handle);

		auto id = static_cast<GLuint>(frame_buffer.BackendID);
		VORTEX_GL_CALL(glDeleteFramebuffers(1, &id));

		for (auto tex_handle : frame_buffer.Textures) {
			DestroyTexture(tex_handle);
		}
		m_Datas.Destroy(framebuffer_handle);
	}

	Handle OpenGL45Backend::CreateTimerQuery() {
		GLuint id{0};
		VORTEX_GL_CALL(glGenQueries(1, &id));
		TimerQuery query{
			TimerQuery::Stopped
			, id
		};

		auto handle = m_Datas.Insert<TimerQuery>(query);

		return handle;
	}
	void OpenGL45Backend::BeginTimerQuery(Handle timer_query_handle) {
		VORTEX_ASSERT(d_TimerQueryChecks(timer_query_handle))
		auto& query = m_Datas.Get<TimerQuery>(timer_query_handle);

		if (query.State == TimerQuery::Stopped) {
			auto id = static_cast<GLuint>(query.BackendID);
			VORTEX_GL_CALL(glBeginQuery(GL_TIME_ELAPSED, id));
			query.State = TimerQuery::Running;
		}
	}
	bool OpenGL45Backend::EndTimerQuery(Handle timer_query_handle, UInt32& value) {
		VORTEX_ASSERT(d_TimerQueryChecks(timer_query_handle))
		auto& query = m_Datas.Get<TimerQuery>(timer_query_handle);

		if (query.State == TimerQuery::Running) {
			VORTEX_GL_CALL(glEndQuery(GL_TIME_ELAPSED));
			query.State = TimerQuery::Waiting;
		}

		if (query.State == TimerQuery::Waiting) {
			GLint available;
			auto id = static_cast<GLuint>(query.BackendID);
			VORTEX_GL_CALL(glGetQueryObjectiv(id, GL_QUERY_RESULT_AVAILABLE, &available));

			if (available) {
				VORTEX_GL_CALL(glGetQueryObjectuiv(id, GL_QUERY_RESULT, &value));
				query.State = TimerQuery::Stopped;
				return true;
			}
		}
		return false;
	}
	void OpenGL45Backend::DestroyTimerQuery(Handle timer_query_handle) {
		VORTEX_ASSERT(d_TimerQueryChecks(timer_query_handle))
		auto& query = m_Datas.Get<TimerQuery>(timer_query_handle);

		auto id = static_cast<GLuint>(query.BackendID);
		VORTEX_GL_CALL(glDeleteQueries(1, &id));

		m_Datas.Destroy(timer_query_handle);
	}
	SizeType OpenGL45Backend::GetHardwareLimit(HardwareLimit::Enum type) const {
		return m_HardwareLimits[type];
	}
	void OpenGL45Backend::SetState(RenderStates::Enum state, bool active) {
		VORTEX_ASSERT(state < RenderStates::Count)
		if (m_ActiveStates[state] != active) {
			if (active) {
				VORTEX_GL_CALL(glEnable(OpenGL45::States[state]));
			} else {
				VORTEX_GL_CALL(glDisable(OpenGL45::States[state]));
			}
			m_ActiveStates[state] = active;
		}
	}
	bool OpenGL45Backend::IsEnabled(RenderStates::Enum state) const {
		return m_ActiveStates[state];
	}
	void OpenGL45Backend::SetDepthTest(DepthTesting::Enum depth_testing) const {
		VORTEX_ASSERT(depth_testing < DepthTesting::Count)
		if (depth_testing == DepthTesting::Disabled) {
			VORTEX_GL_CALL(glDisable(GL_DEPTH_TEST));
		} else {
			VORTEX_GL_CALL(glEnable(GL_DEPTH_TEST));
			VORTEX_GL_CALL(glDepthMask(GL_TRUE));
			VORTEX_GL_CALL(glDepthFunc(OpenGL45::DepthTest[depth_testing]));
		}
	}
	void OpenGL45Backend::SetBlending(Blending::Enum blending) const {
		//disable depth writing when rendering geometry other than opaque.
		VORTEX_ASSERT(blending < Blending::Count)
		if (blending == Blending::Disabled) {
			VORTEX_GL_CALL(glDisable(GL_BLEND));
			VORTEX_GL_CALL(glDepthMask(GL_TRUE));
		} else {
			VORTEX_GL_CALL(glEnable(GL_BLEND));
			VORTEX_GL_CALL(glDepthMask(GL_FALSE));
			switch (blending) {
				case Blending::Additive: {
					VORTEX_GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
					break;
				}
				case Blending::Subtractive: {
					VORTEX_GL_CALL(glBlendFunc(GL_DST_COLOR, GL_ZERO));
					break;
				}

				case Blending::Opaque:
				case Blending::Count:break;
			}
		}
	}
	void OpenGL45Backend::SetScissor(const RectangleInt& dimension) const {
		VORTEX_GL_CALL(glScissor(dimension.x(), dimension.y(), dimension.Width(), dimension.Height()));
	}
	void OpenGL45Backend::SetViewport(const RectangleInt& size) const {
		VORTEX_GL_CALL(glViewport(size.x(), size.y(), size.Width(), size.Height()));
	}
	void OpenGL45Backend::ClearColor(const Color& color) const {
		VORTEX_GL_CALL(glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE));
		VORTEX_GL_CALL(glClearColor(color.Red(), color.Green(), color.Blue(), color.Alpha()));
		VORTEX_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}
	void OpenGL45Backend::ClearDepth(float reset) const {
		VORTEX_GL_CALL(glDepthMask(GL_TRUE));
		VORTEX_GL_CALL(glClearDepthf(reset));
		VORTEX_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
	}
	void OpenGL45Backend::ClearStencil(UInt32 reset) const {
		VORTEX_GL_CALL(glStencilMask(GL_TRUE));
		VORTEX_GL_CALL(glClearStencil(reset));
		VORTEX_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));
	}
}

/*
#include "windows.h"

extern "C" {
_declspec(dllexport)
DWORD NvOptimusEnablement = 0x00000001;
}*/