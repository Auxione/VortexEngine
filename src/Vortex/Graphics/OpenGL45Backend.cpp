#include <glad/glad.h>
#include <filesystem>

#include "Vortex/Common/Logger.h"
#include "Vortex/Memory/HeapAllocator.h"
#include "OpenGL45Backend.h"

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

void ClearGLError() {
	while (glGetError() != GL_NO_ERROR) {}
}

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

namespace Vortex::OpenGL {
	constexpr static GLint BufferUsage[]{
		GL_STATIC_DRAW            // StaticDraw
		, GL_STATIC_READ        // StaticRead
		, GL_STATIC_COPY        // StaticCopy
		, GL_DYNAMIC_DRAW        // DynamicDraw
		, GL_DYNAMIC_READ        // DynamicRead
		, GL_DYNAMIC_COPY        // DynamicCopy
		, GL_STREAM_DRAW        // StreamDraw
		, GL_STREAM_READ        // StreamRead
		, GL_STREAM_COPY        // StreamCopy
	};

	constexpr static GLint Topology[]{
		GL_TRIANGLES            // TriangleList
		, GL_TRIANGLE_STRIP        // TriangleStrip
		, GL_TRIANGLE_FAN        // TriangleFan
		, GL_LINES                // LineList
		, GL_LINE_STRIP            // LineStrip
		, GL_POINTS                // PointList
	};

	constexpr static GLint ShaderType[]{
		GL_VERTEX_SHADER        // Vertex
		, GL_FRAGMENT_SHADER    // Fragment
		, GL_GEOMETRY_SHADER    // Geometry
		, GL_COMPUTE_SHADER        // Compute
	};

	constexpr static GLint TextureLODFilter[]{
		GL_LINEAR                // Linear
		, GL_NEAREST            // Nearest
	};

	constexpr static GLint TextureWrapType[]{
		GL_REPEAT                // Repeat
		, GL_MIRRORED_REPEAT    // RepeatMirrored
		, GL_CLAMP_TO_BORDER    // ClampToBorder
		, GL_CLAMP_TO_EDGE        // ClampToEdge
	};

	constexpr static GLint PixelFormatInternal[]{
		GL_R8                    // Red_UI8
		, GL_R8                    // Alpha_UI8
		, GL_RGB8                // RGB_UI8
		, GL_RGBA8            // RGBA_UI8
		, GL_STENCIL_INDEX8        // Stencil_UI8
		, GL_R32UI                // Red_UI32
		, GL_R32UI                // Alpha_UI32
		, GL_DEPTH_COMPONENT32F    // Depth_F32
		, GL_R32F                // Red_F32
		, GL_R32F                // Alpha_F32
		, GL_RGB32F                // RGB_F32
		, GL_RGBA32F            // RGBA_F32
		, GL_DEPTH24_STENCIL8    // DepthStencil
	};

	constexpr static GLenum PixelFormat[]{
		GL_RED                    // Red_UI8
		, GL_RED                // Alpha_UI8
		, GL_RGB                // RGB_UI8
		, GL_RGBA                // RGBA_UI8
		, GL_STENCIL_INDEX        // Stencil_UI8
		, GL_RGBA_INTEGER        // Red_UI32
		, GL_RGBA_INTEGER        // Alpha_UI32
		, GL_DEPTH_COMPONENT    // Depth_F32
		, GL_RED                // Red_F32
		, GL_RED                // Alpha_F32
		, GL_RGB                // RGB_F32
		, GL_RGBA                // RGBA_F32
		, GL_DEPTH_STENCIL        // DepthStencil
	};

	constexpr static GLenum PixelFormatType[]{
		GL_UNSIGNED_BYTE        // Red_UI8
		, GL_UNSIGNED_BYTE        // Alpha_UI8
		, GL_UNSIGNED_BYTE        // RGB_UI8
		, GL_UNSIGNED_BYTE        // RGBA_UI8
		, GL_UNSIGNED_BYTE        // Stencil_UI8
		, GL_UNSIGNED_INT        // Red_UI32
		, GL_UNSIGNED_INT        // Alpha_UI32
		, GL_FLOAT                // Depth_F32
		, GL_FLOAT                // Red_F32
		, GL_FLOAT                // Alpha_F32
		, GL_FLOAT                // RGB_F32
		, GL_FLOAT                // RGBA_F32
		, GL_UNSIGNED_INT_24_8    // DepthStencil
	};

	constexpr static GLenum States[]{
		GL_BLEND        // Blending
		, GL_CULL_FACE      // FaceCulling
		, GL_DEPTH_TEST      // DepthTest
		, GL_MULTISAMPLE      // Multisampling
		, GL_SCISSOR_TEST      // ScissorTest
		, GL_STENCIL_TEST       // StencilTest
		, GL_FALSE       // WireframeMode handled by SetState function
	};
	constexpr static GLenum FaceCulling[]{
		GL_BACK    //Back
		, GL_FRONT    //Front
		, GL_FRONT_AND_BACK    //FrontAndBack
	};

	constexpr static GLenum DepthFunc[]{
		GL_NEVER, //Never
		GL_LESS, //Less
		GL_LEQUAL, //LessEqual
		GL_EQUAL, //Equal
		GL_GEQUAL, //GreaterEqual
		GL_GREATER, //Greater
		GL_ALWAYS //Always
	};

	constexpr static GLenum FrameBufferAttachment[]{
		GL_COLOR_ATTACHMENT0,        // Color
		GL_DEPTH_ATTACHMENT,        // Depth
		GL_STENCIL_ATTACHMENT,        // Stencil
		GL_DEPTH_STENCIL_ATTACHMENT    // DepthStencil
	};

	constexpr static GLenum BlendFunc[]{
		GL_ZERO
		, GL_ONE
		, GL_SRC_COLOR
		, GL_ONE_MINUS_SRC_COLOR
		, GL_DST_COLOR
		, GL_ONE_MINUS_DST_COLOR
		, GL_SRC_ALPHA
		, GL_ONE_MINUS_SRC_ALPHA
		, GL_DST_ALPHA
		, GL_ONE_MINUS_DST_ALPHA
		, GL_CONSTANT_COLOR
		, GL_ONE_MINUS_CONSTANT_COLOR
		, GL_CONSTANT_ALPHA
		, GL_ONE_MINUS_CONSTANT_ALPHA
	};

	constexpr static GLenum RenderElementType[]{
		GL_FLOAT,    //Float1
		GL_FLOAT,    //Float2
		GL_FLOAT,    //Float3
		GL_FLOAT,    //Float4

		0,            //FloatingTypeCount

		GL_INT,        //Int1
		GL_INT,        //Int2
		GL_INT,        //Int3
		GL_INT,        //Int4

		GL_UNSIGNED_INT,    //UInt1
		GL_UNSIGNED_INT,    //UInt2
		GL_UNSIGNED_INT,    //UInt3
		GL_UNSIGNED_INT,    //UInt4

		GL_BYTE,        //Byte1
		GL_BYTE,        //Byte2
		GL_BYTE,        //Byte3
		GL_BYTE,        //Byte4

		GL_UNSIGNED_BYTE,    //UByte1
		GL_UNSIGNED_BYTE,    //UByte2
		GL_UNSIGNED_BYTE,    //UByte3
		GL_UNSIGNED_BYTE,    //UByte4

		GL_SHORT,        //Short1
		GL_SHORT,        //Short2
		GL_SHORT,        //Short3
		GL_SHORT,        //Short4

		GL_UNSIGNED_SHORT,    //UShort1
		GL_UNSIGNED_SHORT,    //UShort2
		GL_UNSIGNED_SHORT,    //UShort3
		GL_UNSIGNED_SHORT,    //UShort4

		0,            //IntegerTypeCount

	};

	constexpr const char* GetFrameBufferStatus(GLenum error) {
		switch (error) {
			default:
			case GL_FRAMEBUFFER_UNDEFINED: return "GL_FRAMEBUFFER_UNDEFINED";
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS: return "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			case GL_FRAMEBUFFER_COMPLETE: return "GL_FRAMEBUFFER_COMPLETE";
		}
	}

}

namespace Vortex::Graphics {
	OpenGL45Backend::OpenGL45Backend(GLADloadproc context_proc)
		: m_Limits{}, m_ActiveStates{} {
		if (!gladLoadGLLoader(context_proc)) {
			VORTEX_LOG_ERROR("[OpenGL] Failed to initialize OpenGL 4.5");
			return;
		}

		VORTEX_LOG_INFO("[OpenGL] Version    : %s", glGetString(GL_VERSION));
		VORTEX_LOG_INFO("[OpenGL] Vendor     : %s", glGetString(GL_VENDOR));
		VORTEX_LOG_INFO("[OpenGL] API        : %s", glGetString(GL_RENDERER));
		//VORTEX_LOG_DEBUG("[OpenGL] Extensions : %s", glGetString(GL_EXTENSIONS))

		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_SIZE, static_cast<GLint*>(&m_Limits[RenderLimits::MaxTextureSize])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_ARRAY_TEXTURE_LAYERS, static_cast<GLint*>(&m_Limits[RenderLimits::MaxTextureLayers])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, static_cast<GLint*>(&m_Limits[RenderLimits::MaxTextureSamplers])));

		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_FRAMEBUFFER_WIDTH, static_cast<GLint*>(&m_Limits[RenderLimits::MaxFrameBufferWidth])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_FRAMEBUFFER_HEIGHT, static_cast<GLint*>(&m_Limits[RenderLimits::MaxFrameBufferHeight])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, static_cast<GLint*>(&m_Limits[RenderLimits::MaxFrameBufferColorAttachments])));
		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_DRAW_BUFFERS, static_cast<GLint*>(&m_Limits[RenderLimits::MaxDrawBuffers])));

		VORTEX_GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, static_cast<GLint*>(&m_Limits[RenderLimits::MaxVertexAttributes])));

		for (int type = 0; type < RenderLimits::Count;
			 ++type) {
			VORTEX_LOG_DEBUG("[OpenGL] %s : %i", RenderLimits::ToString[type], m_Limits[type]);
		}

		VORTEX_LOG_INFO("[OpenGL] Initialized successfully.");
	}

	OpenGL45Backend::~OpenGL45Backend() {
		for (const auto& node : m_TextureDatas) {
			auto id = static_cast<GLuint>(node.first.id);
			VORTEX_GL_CALL(glDeleteTextures(1, &id));
		}
		for (const auto& node : m_BufferDatas) {
			auto id = static_cast<GLuint>(node.first.id);
			VORTEX_GL_CALL(glDeleteBuffers(1, &id));
		}
		for (const auto& node : m_VertexArrayDatas) {
			auto id = static_cast<GLuint>(node.first.id);
			VORTEX_GL_CALL(glDeleteVertexArrays(1, &id));
		}
		for (const auto& node : m_ShaderDatas) {
			auto id = static_cast<GLuint>(node.first.id);
			VORTEX_GL_CALL(glDeleteShader(id));
		}
		for (const auto& node : m_ProgramDatas) {
			auto id = static_cast<GLuint>(node.first.id);
			VORTEX_GL_CALL(glDeleteProgram(id));
		}
		for (const auto& node : m_FrameBufferDatas) {
			auto id = static_cast<GLuint>(node.first.id);
			VORTEX_GL_CALL(glDeleteRenderbuffers(1, &id));
		}
	}

	BufferHandle OpenGL45Backend::CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) {
		GLuint id;
		VORTEX_GL_CALL(glCreateBuffers(1, &id));

		auto gl_size = static_cast<GLsizei>(count * buffer_layout.GetStride());
		VORTEX_GL_CALL(glNamedBufferData(id, gl_size, data, OpenGL::BufferUsage[buffer_usage]));

		BufferHandle handle{static_cast<HandleType>(id)};

		BufferData buffer_data{
			buffer_usage
			, buffer_layout
			, true
			, count * buffer_layout.GetStride()
		};

		m_BufferDatas[handle] = buffer_data;
		return handle;
	}

	void OpenGL45Backend::UpdateBuffer(BufferHandle handle, SizeType offset, SizeType data_size, const void* data) {
		VORTEX_ASSERT(IsValid(handle))
		const auto& buffer_data = m_BufferDatas.at(handle);
		VORTEX_ASSERT(buffer_data.Mutable)
		VORTEX_ASSERT(buffer_data.Size >= offset + data_size)

		auto id = static_cast<GLuint>(handle.id);
		auto gl_offs = static_cast<GLsizei>(offset);
		auto gl_size = static_cast<GLsizei>(data_size);
		VORTEX_GL_CALL(glNamedBufferSubData(id, gl_offs, gl_size, data));
	}

	void OpenGL45Backend::GetBuffer(BufferHandle handle, SizeType offset, SizeType data_size, void* data) {
		VORTEX_ASSERT(IsValid(handle))
		const auto& buffer_data = m_BufferDatas.at(handle);
		VORTEX_ASSERT(buffer_data.Mutable)
		VORTEX_ASSERT(buffer_data.Size >= offset + data_size)

		auto id = static_cast<GLuint>(handle.id);
		auto gl_offs = static_cast<GLsizei>(offset);
		auto gl_size = static_cast<GLsizei>(data_size);
		VORTEX_GL_CALL(glGetNamedBufferSubData(id, gl_offs, gl_size, data));
	}

	void OpenGL45Backend::Destroy(BufferHandle handle) {
		if (!IsValid(handle)) {
			VORTEX_LOG_WARNING("[OpenGL] Tried to destroy invalid buffer handle.");
			return;
		}
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glDeleteBuffers(1, &id));
		m_BufferDatas.erase(handle);
	}

	VertexArrayHandle OpenGL45Backend::CreateVertexArray(Topology::Enum topology) {
		GLuint id;
		VORTEX_GL_CALL(glCreateVertexArrays(1, &id));

		VertexArrayHandle handle{static_cast<HandleType>(id)};
		VertexArrayData vertex_array_data{
			InvalidID
			, Vector<BufferHandle>{}
			, false
			, 0
			, topology
		};

		m_VertexArrayDatas[handle] = vertex_array_data;
		return handle;
	}

	void OpenGL45Backend::SetIndexBuffer(VertexArrayHandle handle, BufferHandle index_buffer_handle) {
		VORTEX_ASSERT(IsValid(handle))
		VORTEX_ASSERT(IsValid(index_buffer_handle))

		auto& vao_data = m_VertexArrayDatas.at(handle);
		vao_data.Indexed = true;
		vao_data.IndexBufferHandle = index_buffer_handle;

		auto vao_id = static_cast<GLuint>(handle.id);
		auto ibo_id = static_cast<GLuint>(index_buffer_handle.id);

		VORTEX_GL_CALL(glVertexArrayElementBuffer(vao_id, ibo_id));
	}
	void OpenGL45Backend::AddVertexBuffer(VertexArrayHandle handle, BufferHandle vertex_buffer_handle) {
		VORTEX_ASSERT(IsValid(handle))
		VORTEX_ASSERT(IsValid(vertex_buffer_handle))

		auto& vao_data = m_VertexArrayDatas.at(handle);
		auto& vbo_data = m_BufferDatas.at(vertex_buffer_handle);
		const auto& layout = vbo_data.Layout;

		auto vao_id = static_cast<GLuint>(handle.id);
		auto vbo_id = static_cast<GLuint>(vertex_buffer_handle.id);

		SizeType attribute_index = vao_data.BufferBindIndex;

		for (SizeType i = 0; i < layout.Count(); ++i) {
			auto element_type = layout[i].Type;
			auto element_offset = layout[i].Offset;

			VORTEX_GL_CALL(glEnableVertexArrayAttrib(vao_id, attribute_index));
			VORTEX_GL_CALL(glVertexArrayAttribBinding(vao_id, attribute_index, vao_data.BufferBindIndex));

			if (element_type < RenderElementType::FloatingTypeCount) {
				auto element_norm = layout[i].Normalized;
				VORTEX_GL_CALL(glVertexArrayAttribFormat(
					vao_id,
					attribute_index,
					RenderElementType::ComponentCount[element_type],
					OpenGL::RenderElementType[element_type],
					element_norm,
					element_offset
				));
				++attribute_index;
			} else {
				VORTEX_GL_CALL(glVertexArrayAttribIFormat(
					vao_id,
					attribute_index,
					RenderElementType::ComponentCount[element_type],
					OpenGL::RenderElementType[element_type],
					element_offset
				));
				++attribute_index;
			}
		}

		auto gl_stride = static_cast<GLsizei>(layout.GetStride());
		VORTEX_GL_CALL(glVertexArrayVertexBuffer(vao_id, vao_data.BufferBindIndex, vbo_id, 0, gl_stride));
		VORTEX_GL_CALL(glVertexArrayBindingDivisor(vao_id, vao_data.BufferBindIndex, layout.ElementPerInstance));
		vao_data.VertexBufferHandles.EmplaceBack(vertex_buffer_handle);
		vao_data.BufferBindIndex = attribute_index;
	}

	void OpenGL45Backend::Destroy(VertexArrayHandle handle) {
		if (!IsValid(handle)) {
			VORTEX_LOG_WARNING("[OpenGL] Tried to destroy invalid vertex array handle.");
			return;
		}
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glDeleteVertexArrays(1, &id));
		m_VertexArrayDatas.erase(handle);
	}

	TextureHandle OpenGL45Backend::CreateTexture2D(
		UInt16 width,
		UInt16 height,
		PixelFormat::Enum format,
		const void* data,
		TextureLODFilter::Enum min_filter,
		TextureLODFilter::Enum mag_filter,
		TextureWrap::Enum wrap_s,
		TextureWrap::Enum wrap_t,
		bool create_mipmap) {
		VORTEX_ASSERT(width <= m_Limits[RenderLimits::MaxTextureSize])
		VORTEX_ASSERT(height <= m_Limits[RenderLimits::MaxTextureSize])

		GLuint id;
		VORTEX_GL_CALL(glCreateTextures(GL_TEXTURE_2D, 1, &id));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, OpenGL::TextureLODFilter[min_filter]));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, OpenGL::TextureLODFilter[mag_filter]));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_WRAP_S, OpenGL::TextureWrapType[wrap_s]));
		VORTEX_GL_CALL(glTextureParameteri(id, GL_TEXTURE_WRAP_T, OpenGL::TextureWrapType[wrap_t]));

		VORTEX_GL_CALL(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
		VORTEX_GL_CALL(glPixelStorei(GL_PACK_ALIGNMENT, 1));

		VORTEX_GL_CALL(glTextureStorage2D(id,
										  1, //num levels
										  OpenGL::PixelFormatInternal[format],
										  width,
										  height
		));

		//swizzle data if it is alpha texture
		if (format == PixelFormat::Alpha_UI8 ||
			format == PixelFormat::Alpha_UI32 ||
			format == PixelFormat::Alpha_F32) {
			GLint swizzleMask[]{GL_ONE, GL_ONE, GL_ONE, GL_RED};
			VORTEX_GL_CALL(glTextureParameteriv(id, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask));
		}

		if (data != nullptr) {
			VORTEX_GL_CALL(glTextureSubImage2D(id,
											   0,            //level
											   0,            //x_off
											   0,            //y_off
											   width,        //width
											   height,       //height
											   OpenGL::PixelFormat[format],
											   OpenGL::PixelFormatType[format],
											   data
			));
			if (create_mipmap) {
				VORTEX_GL_CALL(glGenerateTextureMipmap(id));
			}
		}

		TextureHandle handle{static_cast<HandleType>(id)};
		TextureData texture_data{
			format
			, width
			, height
			, 0
			, 1
			, PixelFormat::BitsPerPixel[format]
			, false
		};
		m_TextureDatas[handle] = texture_data;
		return handle;
	}
	void OpenGL45Backend::UpdateTexture2D(TextureHandle handle, const void* pixels, bool create_mipmap) {
		VORTEX_ASSERT(IsValid(handle))
		VORTEX_ASSERT(pixels != nullptr)

		const auto& texture_data = m_TextureDatas.at(handle);
		auto id = static_cast<GLuint>(handle.id);

		VORTEX_GL_CALL(glTextureSubImage2D(id,
										   0,            //level
										   0,            //x_off
										   0,            //y_off
										   texture_data.Width,        //width
										   texture_data.Height,       //height
										   OpenGL::PixelFormat[texture_data.Format],
										   OpenGL::PixelFormatType[texture_data.Format],
										   pixels
		));

		if (create_mipmap) {
			VORTEX_GL_CALL(glGenerateTextureMipmap(id));
		}
	}

	void OpenGL45Backend::GetTexture(TextureHandle handle, UInt16 buffer_size, void* data) {
		VORTEX_ASSERT(IsValid(handle))
		VORTEX_ASSERT(data != nullptr)

		const auto& texture_data = m_TextureDatas.at(handle);
		auto id = static_cast<GLuint>(handle.id);

		VORTEX_GL_CALL(glGetTextureImage(id,
										 0,
										 OpenGL::PixelFormatInternal[texture_data.Format],
										 OpenGL::PixelFormatType[texture_data.Format],
										 buffer_size,
										 data)
		);
	}
	void OpenGL45Backend::Bind(TextureHandle handle, UInt16 sampler) {
		VORTEX_ASSERT(IsValid(handle))
		VORTEX_ASSERT(sampler < m_Limits[RenderLimits::MaxTextureSamplers])
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glBindTextureUnit(sampler, id));
	}

	void OpenGL45Backend::Destroy(TextureHandle handle) {
		if (!IsValid(handle)) {
			VORTEX_LOG_WARNING("[OpenGL] Tried to destroy invalid texture handle.");
			return;
		}
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glDeleteTextures(1, &id));
		m_TextureDatas.erase(handle);
	}

	ShaderHandle OpenGL45Backend::CreateShader(ShaderType::Enum shader_type, const char* source) {
		GLuint id;
		VORTEX_GL_CALL(id = glCreateShader(OpenGL::ShaderType[shader_type]));
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

			VORTEX_LOG_ERROR("[OpenGL] Failed to compile %s shader:\n%s", ShaderType::ToString[shader_type], err_log.data());

			VORTEX_GL_CALL(glDeleteShader(id));

			return {InvalidID};
		} else {
			ShaderData shader_data{
				shader_type
			};

			ShaderHandle handle{static_cast<HandleType>(id)};
			m_ShaderDatas[handle] = shader_data;

			VORTEX_LOG_INFO("[OpenGL] %s Shader compiled successfully.", ShaderType::ToString[shader_type]);
			return handle;
		}
	}

	void OpenGL45Backend::Destroy(ShaderHandle handle) {
		if (!IsValid(handle)) {
			VORTEX_LOG_WARNING("[OpenGL] Tried to destroy invalid shader handle.");
			return;
		}
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glDeleteShader(id));
		m_ShaderDatas.erase(handle);
	}

	ProgramHandle OpenGL45Backend::CreateProgram(ShaderHandle* shader_handles, SizeType count, bool consume) {
		GLuint id;
		VORTEX_GL_CALL(id = glCreateProgram());

		for (SizeType i = 0; i < count; ++i) {
			VORTEX_ASSERT(IsValid(shader_handles[i]))
			auto shader_id = static_cast<GLuint>(shader_handles[i].id);
			VORTEX_GL_CALL(glAttachShader(id, shader_id));
		}
		VORTEX_GL_CALL(glLinkProgram(id));
		VORTEX_GL_CALL(glValidateProgram(id));

		GLint status = 0;
		VORTEX_GL_CALL(glGetProgramiv(id, GL_LINK_STATUS, &status));

		if (consume) {
			for (SizeType i = 0; i < count; ++i) {
				Destroy(shader_handles[i]);
			}
		}

		if (status == GL_FALSE) {
			GLint max_length = 0;
			VORTEX_GL_CALL(glGetProgramiv(id, GL_INFO_LOG_LENGTH, &max_length));

			String err_log;
			err_log.resize(max_length);
			VORTEX_GL_CALL(glGetProgramInfoLog(id, max_length, &max_length, &err_log[0]));
			VORTEX_LOG_ERROR("[OpenGL] Program linking failed.\n%s", err_log.data());

			VORTEX_GL_CALL(glDeleteProgram(id));
			return {InvalidID};
		} else {

			ProgramHandle handle{static_cast<HandleType>(id)};
			ProgramData data{};
			CreateProgramData(data, id);
			m_ProgramDatas[handle] = data;
			VORTEX_LOG_DEBUG("[OpenGL] Shader Program linking successful.");
			return handle;
		}
	}

	void OpenGL45Backend::CreateProgramData(ProgramData& data, GLuint id) {
		//retrieve shader uniform count
		GLsizei uniform_count;
		VORTEX_GL_CALL(glGetProgramiv(id, GL_ACTIVE_UNIFORMS, &uniform_count));

		//Create cache for names and locations of every uniform and attribute in shader.
		for (GLsizei i = 0; i < uniform_count; i++) {
			GLsizei length;
			GLint size;
			GLenum type;
			char name[128];
			VORTEX_GL_CALL(glGetActiveUniform(id, i, 128, &length, &size, &type, name));
			VORTEX_GL_CALL(GLint location = glGetUniformLocation(id, name));

			HashedString hashed_str{name, static_cast<SizeType>(length) + 1};

			//data.ShaderLocations.Emplace(hashed_str, location);
			data.ShaderLocations[hashed_str] = location;
			VORTEX_LOG_DEBUG("[OpenGL] Cached shader uniform: %i - %s", location, name)
		}

		GLsizei attribute_count;
		VORTEX_GL_CALL(glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &attribute_count));

		for (GLsizei i = 0; i < attribute_count; i++) {
			GLsizei length;
			GLint size;
			GLenum type;
			char name[128];
			VORTEX_GL_CALL(glGetActiveAttrib(id, i, 128, &length, &size, &type, name));
			VORTEX_GL_CALL(GLint location = glGetAttribLocation(id, name));

			HashedString hashed_str{name, static_cast<SizeType>(length) + 1};

			//data.ShaderLocations.Emplace(hashed_str, location);
			data.ShaderLocations[hashed_str] = location;
			VORTEX_LOG_DEBUG("[OpenGL] Cached shader attribute: %i - %s", location, name)
		}
	}

	void OpenGL45Backend::Destroy(ProgramHandle handle) {
		if (!IsValid(handle)) {
			VORTEX_LOG_WARNING("[OpenGL] Tried to destroy invalid shader program handle.");
			return;
		}
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glDeleteProgram(id));
		m_ProgramDatas.erase(handle);
	}

	void OpenGL45Backend::SetShaderFloat1(ProgramHandle handle, HashedString name, float x) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform1f(program_id, location, x));
		}
	}
	void OpenGL45Backend::SetShaderFloat2(ProgramHandle handle, HashedString name, float x, float y) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform2f(program_id, location, x, y));
		}
	}
	void OpenGL45Backend::SetShaderFloat3(ProgramHandle handle, HashedString name, float x, float y, float z) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform3f(program_id, location, x, y, z));
		}
	}
	void OpenGL45Backend::SetShaderFloat4(ProgramHandle handle, HashedString name, float x, float y, float z, float w) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform4f(program_id, location, x, y, z, w));
		}
	}

	void OpenGL45Backend::SetShaderInt1(ProgramHandle handle, HashedString name, Int32 x) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform1i(program_id, location, x));
		}
	}
	void OpenGL45Backend::SetShaderInt2(ProgramHandle handle, HashedString name, Int32 x, Int32 y) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform2i(program_id, location, x, y));
		}
	}
	void OpenGL45Backend::SetShaderInt3(ProgramHandle handle, HashedString name, Int32 x, Int32 y, Int32 z) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform3i(program_id, location, x, y, z));
		}
	}
	void OpenGL45Backend::SetShaderInt4(ProgramHandle handle, HashedString name, Int32 x, Int32 y, Int32 z, Int32 w) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform4i(program_id, location, x, y, z, w));
		}
	}

	void OpenGL45Backend::SetShaderUInt1(ProgramHandle handle, HashedString name, UInt32 x) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform1ui(program_id, location, x));
		}
	}
	void OpenGL45Backend::SetShaderUInt2(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform2ui(program_id, location, x, y));
		}
	}
	void OpenGL45Backend::SetShaderUInt3(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y, UInt32 z) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform3ui(program_id, location, x, y, z));
		}
	}
	void OpenGL45Backend::SetShaderUInt4(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y, UInt32 z, UInt32 w) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform4ui(program_id, location, x, y, z, w));
		}
	}

	void OpenGL45Backend::SetShaderFloat1Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform1fv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderFloat2Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform2fv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderFloat3Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform3fv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderFloat4Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform4fv(program_id, location, count, array));
		}
	}

	void OpenGL45Backend::SetShaderInt1Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform1iv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderInt2Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform2iv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderInt3Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform3iv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderInt4Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform4iv(program_id, location, count, array));
		}
	}

	void OpenGL45Backend::SetShaderUInt1Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform1uiv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderUInt2Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform2uiv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderUInt3Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform3uiv(program_id, location, count, array));
		}
	}
	void OpenGL45Backend::SetShaderUInt4Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniform4uiv(program_id, location, count, array));
		}
	}

	void OpenGL45Backend::SetShaderMatrix2(ProgramHandle handle, HashedString name, bool transposed, const float* value) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniformMatrix2fv(program_id, location, 1, transposed, value));
		}
	}
	void OpenGL45Backend::SetShaderMatrix3(ProgramHandle handle, HashedString name, bool transposed, const float* value) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniformMatrix3fv(program_id, location, 1, transposed, value));
		}
	}
	void OpenGL45Backend::SetShaderMatrix4(ProgramHandle handle, HashedString name, bool transposed, const float* value) {
		VORTEX_ASSERT(IsValid(handle))
		auto program_id = static_cast<GLuint>(handle.id);
		GLint location;
		if (GetShaderUniformLocation(m_ProgramDatas[handle], name, location)) {
			VORTEX_GL_CALL(glProgramUniformMatrix4fv(program_id, location, 1, transposed, value));
		}
	}

	void OpenGL45Backend::Bind(ProgramHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glUseProgram(id));
	}

	FrameBufferHandle OpenGL45Backend::CreateFrameBuffer(
		TextureHandle* textures,
		FrameBufferAttachment::Enum* attachments,
		SizeType count,
		bool destroy_textures
	) {
		VORTEX_ASSERT(textures != nullptr)
		VORTEX_ASSERT(attachments != nullptr)
		VORTEX_ASSERT(count > 0)

		GLuint id;
		VORTEX_GL_CALL(glCreateFramebuffers(1, &id));

		GLenum color_attachment_slot{0};

		UInt16 width{0};
		UInt16 height{0};

		GLenum draw_buffers[32];

		for (SizeType i = 0; i < count; ++i) {
			auto texture_handle = textures[i];
			auto attachment_type = attachments[i];
			VORTEX_ASSERT(IsValid(texture_handle))

			//calculate max size of the textures
			const auto& tex_data = GetData(texture_handle);
			if (tex_data.Width > width) width = tex_data.Width;
			if (tex_data.Height > height) height = tex_data.Height;

			GLenum current_attachment_enum;
			if (attachment_type == FrameBufferAttachment::Color) {
				current_attachment_enum = OpenGL::FrameBufferAttachment[attachment_type] + color_attachment_slot;
				draw_buffers[color_attachment_slot] = current_attachment_enum;
				color_attachment_slot++;
			} else {
				current_attachment_enum = OpenGL::FrameBufferAttachment[attachment_type];
			}

			auto tex_id = static_cast<GLuint>(texture_handle.id);

			VORTEX_GL_CALL(glNamedFramebufferTexture(id, current_attachment_enum, tex_id, 0));

			VORTEX_LOG_DEBUG("[OpenGL] Framebuffer: %zu Attachment: %s Texture: %zu", id, FrameBufferAttachment::ToString[attachment_type], texture_handle.id)
		}

		VORTEX_GL_CALL(glNamedFramebufferDrawBuffers(id, color_attachment_slot, draw_buffers));

		VORTEX_GL_CALL(auto result = glCheckNamedFramebufferStatus(id, GL_FRAMEBUFFER));
		if (result != GL_FRAMEBUFFER_COMPLETE) {
			VORTEX_LOG_ERROR("[OpenGL] Incomplete Framebuffer:%s", OpenGL::GetFrameBufferStatus(result));
			VORTEX_ASSERT(false)
		}

		FrameBufferHandle handle{id};
		FrameBufferData data{
			width
			, height
			, {textures, count}
			, {attachments, count}
			, destroy_textures
		};

		m_FrameBufferDatas[handle] = data;
		return handle;
	}
	void OpenGL45Backend::Bind(FrameBufferHandle handle) {
		VORTEX_ASSERT(IsValid(handle))
		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, id));
	}
	void OpenGL45Backend::BindBackBuffer() {
		VORTEX_GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	void OpenGL45Backend::Destroy(FrameBufferHandle handle) {
		VORTEX_ASSERT(IsValid(handle))

		auto id = static_cast<GLuint>(handle.id);
		VORTEX_GL_CALL(glDeleteFramebuffers(1, &id));

		const auto& data = m_FrameBufferDatas.at(handle);
		if (data.DestroyTextures) {
			for (auto tex_handle : data.Handles) {
				Destroy(tex_handle);
			}
		}
		m_FrameBufferDatas.erase(handle);
	}

	void OpenGL45Backend::SetState(RenderStates::Enum state, bool active) {
		VORTEX_ASSERT(state < RenderStates::Count)
		if (m_ActiveStates[state] != active) {
			if (active) {
				VORTEX_GL_CALL(glEnable(OpenGL::States[state]));
			} else {
				VORTEX_GL_CALL(glDisable(OpenGL::States[state]));
			}
			m_ActiveStates[state] = active;
		}
	}

	void OpenGL45Backend::SetDepthFunction(DepthFunction::Enum func) const {
		VORTEX_ASSERT(func < DepthFunction::Count)
		VORTEX_ASSERT(m_ActiveStates[RenderStates::DepthTest])
		VORTEX_GL_CALL(glDepthFunc(OpenGL::DepthFunc[func]));
	}
	void OpenGL45Backend::SetBlendFunction(BlendFunction::Enum source, BlendFunction::Enum target) const {
		VORTEX_ASSERT(source < BlendFunction::Count)
		VORTEX_ASSERT(target < BlendFunction::Count)
		VORTEX_ASSERT(m_ActiveStates[RenderStates::Blending])
		VORTEX_GL_CALL(glBlendFunc(OpenGL::BlendFunc[source], OpenGL::BlendFunc[target]));
	}

	void OpenGL45Backend::SetScissor(const Int32* dimension) const {
		VORTEX_ASSERT(dimension != nullptr)
		VORTEX_ASSERT(m_ActiveStates[RenderStates::ScissorTest])
		VORTEX_GL_CALL(glScissor(dimension[0], dimension[1], dimension[2], dimension[3]));
	}
	void OpenGL45Backend::SetViewport(const Int32* size) const {
		VORTEX_ASSERT(size != nullptr)
		VORTEX_GL_CALL(glViewport(size[0], size[1], size[2], size[3]));
	}

	void OpenGL45Backend::ClearColor(const float* color) const {
		VORTEX_ASSERT(color != nullptr)
		VORTEX_GL_CALL(glClearColor(color[0], color[1], color[2], color[3]));
		VORTEX_GL_CALL(glClear(GL_COLOR_BUFFER_BIT));
	}
	void OpenGL45Backend::ClearDepth(float reset) const {
		VORTEX_GL_CALL(glClearDepthf(reset));
		VORTEX_GL_CALL(glClear(GL_DEPTH_BUFFER_BIT));
	}
	void OpenGL45Backend::ClearStencil(UInt32 reset) const {
		VORTEX_GL_CALL(glClearStencil(reset));
		VORTEX_GL_CALL(glClear(GL_STENCIL_BUFFER_BIT));
	}
	void OpenGL45Backend::Draw(VertexArrayHandle handle, SizeType count) const {
		VORTEX_ASSERT(IsValid(handle))
		const auto& data = m_VertexArrayDatas.at(handle);
		auto gl_id = static_cast<GLuint>(handle.id);

		VORTEX_GL_CALL(glBindVertexArray(gl_id));
		if (data.Indexed) {
			auto index_buffer_h = data.IndexBufferHandle;
			VORTEX_ASSERT(IsValid(index_buffer_h))

			const auto& ibo_data = m_BufferDatas.at(index_buffer_h);
			auto type = ibo_data.Layout[0].Type;

			VORTEX_ASSERT(type == RenderElementType::UInt1 || type == RenderElementType::UShort1 || type == RenderElementType::UByte1)

			VORTEX_GL_CALL(glDrawElements(
				OpenGL::Topology[data.Topology],
				count,
				OpenGL::RenderElementType[type],
				nullptr
			));
		} else {
			VORTEX_GL_CALL(glDrawArrays(
				OpenGL::Topology[data.Topology],
				0,
				count
			));
		}
	}
	void OpenGL45Backend::DrawInstanced(VertexArrayHandle handle, SizeType count, SizeType instance_count) const {
		VORTEX_ASSERT(IsValid(handle))
		const auto& data = m_VertexArrayDatas.at(handle);
		auto gl_id = static_cast<GLuint>(handle.id);

		VORTEX_GL_CALL(glBindVertexArray(gl_id));
		if (data.Indexed) {
			auto index_buffer_h = data.IndexBufferHandle;
			VORTEX_ASSERT(IsValid(index_buffer_h))

			const auto& ibo_data = m_BufferDatas.at(index_buffer_h);
			auto type = ibo_data.Layout[0].Type;

			VORTEX_ASSERT(type == RenderElementType::UInt1 || type == RenderElementType::UShort1 || type == RenderElementType::UByte1)

			VORTEX_GL_CALL(glDrawElementsInstanced(
				OpenGL::Topology[data.Topology],
				count,
				OpenGL::RenderElementType[type],
				nullptr,
				instance_count
			));
		} else {
			VORTEX_GL_CALL(glDrawArraysInstanced(
				OpenGL::Topology[data.Topology],
				0,
				count,
				instance_count
			));
		}
	}
}