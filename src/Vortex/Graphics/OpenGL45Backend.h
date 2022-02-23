#pragma once
#include <glad/glad.h>

#include "Vortex/Graphics/RenderBackend.h"
#include "Vortex/Containers/Vector.h"

namespace Vortex::Graphics {
	class OpenGL45Backend: public RenderBackend {
	public:
		explicit OpenGL45Backend(GLADloadproc context_proc);
		~OpenGL45Backend() override;

	public: // Vertex buffer, index buffer
		BufferHandle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType size, const void* data) override;
		void UpdateBuffer(BufferHandle buffer_handle, SizeType offset, SizeType data_size, const void* data) override;
		void GetBuffer(BufferHandle handle, SizeType offset, SizeType data_size, void* data) override;
		void Destroy(BufferHandle buffer_handle) override;

		VertexArrayHandle CreateVertexArray(Topology::Enum topology) override;
		void SetIndexBuffer(VertexArrayHandle vertex_array_handle, BufferHandle index_buffer_handle) override;
		void AddVertexBuffer(VertexArrayHandle vertex_array_handle, BufferHandle vertex_buffer_handle) override;
		void Destroy(VertexArrayHandle vertex_array_handle) override;

	public: // Textures
		TextureHandle CreateTexture2D(
			UInt16 width,
			UInt16 height,
			PixelFormat::Enum format,
			const void* pixels,
			TextureLODFilter::Enum min_filter,
			TextureLODFilter::Enum mag_filter,
			TextureWrap::Enum wrap_s,
			TextureWrap::Enum wrap_t,
			bool create_mipmap
		) override;
		void UpdateTexture2D(TextureHandle handle, const void* pixels, bool create_mipmap) override;
		void GetTexture(TextureHandle handle, UInt16 buffer_size, void* data) override;
		void Bind(TextureHandle handle, UInt16 sampler) override;
		void Destroy(TextureHandle handle) override;

	public: //Shaders & Programs
		ShaderHandle CreateShader(ShaderType::Enum shader_type, const char* source) override;
		void Destroy(ShaderHandle handle) override;

		ProgramHandle CreateProgram(ShaderHandle* shader_handles, SizeType count, bool consume) override;
		void Bind(ProgramHandle handle) override;
		void Destroy(ProgramHandle handle) override;

		//virtual void SetShaderBuffer(const char* name, BufferHandle buffer_handle, UInt16 binding = 0) = 0;
		void SetShaderFloat1(ProgramHandle handle, HashedString name, float x) override;
		void SetShaderFloat2(ProgramHandle handle, HashedString name, float x, float y) override;
		void SetShaderFloat3(ProgramHandle handle, HashedString name, float x, float y, float z) override;
		void SetShaderFloat4(ProgramHandle handle, HashedString name, float x, float y, float z, float w) override;

		void SetShaderInt1(ProgramHandle handle, HashedString name, Int32 x) override;
		void SetShaderInt2(ProgramHandle handle, HashedString name, Int32 x, Int32 y) override;
		void SetShaderInt3(ProgramHandle handle, HashedString name, Int32 x, Int32 y, Int32 z) override;
		void SetShaderInt4(ProgramHandle handle, HashedString name, Int32 x, Int32 y, Int32 z, Int32 w) override;

		void SetShaderUInt1(ProgramHandle handle, HashedString name, UInt32 x) override;
		void SetShaderUInt2(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y) override;
		void SetShaderUInt3(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y, UInt32 z) override;
		void SetShaderUInt4(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y, UInt32 z, UInt32 w) override;

		void SetShaderFloat1Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) override;
		void SetShaderFloat2Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) override;
		void SetShaderFloat3Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) override;
		void SetShaderFloat4Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) override;

		void SetShaderInt1Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) override;
		void SetShaderInt2Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) override;
		void SetShaderInt3Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) override;
		void SetShaderInt4Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) override;

		void SetShaderUInt1Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) override;
		void SetShaderUInt2Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) override;
		void SetShaderUInt3Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) override;
		void SetShaderUInt4Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) override;

		void SetShaderMatrix2(ProgramHandle handle, HashedString name, bool transposed, const float* value) override;
		void SetShaderMatrix3(ProgramHandle handle, HashedString name, bool transposed, const float* value) override;
		void SetShaderMatrix4(ProgramHandle handle, HashedString name, bool transposed, const float* value) override;

		static void CreateProgramData(ProgramData& data, GLuint id);

		static inline bool GetShaderUniformLocation(ProgramData& program_data, HashedString name, GLint& pos) {
			auto it = program_data.ShaderLocations.find(name);
			if (it != program_data.ShaderLocations.end()) {
				pos = it->second;
				return true;
			}
			return false;
		}

	public:
		FrameBufferHandle CreateFrameBuffer(
			TextureHandle* textures,
			FrameBufferAttachment::Enum* attachments,
			SizeType count,
			bool destroy_textures
		) override;
		void Bind(FrameBufferHandle handle) override;
		void BindBackBuffer() override;
		void Destroy(FrameBufferHandle handle) override;

	public:
		SizeType GetRenderLimit(RenderLimits::Enum type) const override { return static_cast<SizeType>(m_Limits[type]); }

		void SetState(RenderStates::Enum state, bool active) override;
		bool IsEnabled(RenderStates::Enum state) const override { return m_ActiveStates[state]; }
		void SetDepthFunction(DepthFunction::Enum func) const override;
		void SetBlendFunction(BlendFunction::Enum source, BlendFunction::Enum target) const override;

		void SetScissor(const Int32* dimension) const override;
		void SetViewport(const Int32* size) const override;

		void ClearColor(const float* color) const override;
		void ClearDepth(float reset) const override;
		void ClearStencil(UInt32 reset) const override;
		void Draw(VertexArrayHandle handle, SizeType count) const override;
		void DrawInstanced(VertexArrayHandle handle, SizeType count, SizeType instance_count) const override;

	private:
		int m_Limits[RenderLimits::Count];
		bool m_ActiveStates[RenderStates::Count];
	};
}