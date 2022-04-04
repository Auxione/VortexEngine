#pragma once
#include <glad/glad.h>

#include "Vortex/Graphics/RenderBackend.h"

namespace Vortex::Graphics {
	class OpenGL45Backend: public RenderBackend {
	public:
		explicit OpenGL45Backend();
		~OpenGL45Backend() override;

	public:
		Handle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) override;
		void UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) override;
		void GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) override;
		void DestroyBuffer(Handle buffer_handle) override;

	public:
		Handle CreateVertexArray(Topology::Enum topology) override;
		void SetIndexBuffer(Handle vertex_array_handle, Handle index_buffer_handle) override;
		void AddVertexBuffer(Handle vertex_array_handle, Handle vertex_buffer_handle) override;
		void Draw(Handle vertex_array_handle, SizeType count) const override;
		void DrawInstanced(Handle vertex_array_handle, SizeType count, SizeType instance_count) const override;
		void DestroyVertexArray(Handle vertex_array_handle) override;

	public:
		Handle CreateTexture2D(const UInt16* size, PixelFormat::Enum format, const void* pixels, TextureLODFilter::Enum min_filter, TextureLODFilter::Enum mag_filter, TextureWrap::Enum wrap_s, TextureWrap::Enum wrap_t, bool create_mipmap) override;
		void UpdateTexture2D(Handle texture_handle, const void* pixels, bool create_mipmap) override;
		void GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) override;
		void DestroyTexture(Handle texture_handle) override;

	public:
		Handle CreateShader(const char** sources, ShaderType::Enum* types, SizeType count) override;
		void BindShader(Handle shader_handle) override;
		void SetUniform(Handle shader_handle, HashedString name, const void* data, SizeType count) override;
		void Dispatch(const Vortex::Int32* num_groups) override;
		void DestroyShader(Handle shader_handle) override;

		static void CreateShaderData(Shader& data);

	public:
		Handle CreateFrameBuffer(const UInt16* size, FrameBufferAttachment::Enum* attachments, SizeType count) override;
		void BindFrameBuffer(Handle framebuffer_handle) override;
		void BindBackBuffer() override;
		void DestroyFrameBuffer(Handle framebuffer_handle) override;

	public:
		Handle CreateTimerQuery() override;
		void BeginTimerQuery(Handle timer_query_handle) override;
		bool EndTimerQuery(Handle timer_query_handle, UInt32& value) override;
		void DestroyTimerQuery(Handle timer_query_handle) override;

	public:
		SizeType GetHardwareLimit(HardwareLimit::Enum type) const override;
		void SetState(RenderStates::Enum state, bool active) override;
		bool IsEnabled(RenderStates::Enum state) const override;
		void SetDepthTest(DepthTesting::Enum depth_testing) const override;
		void SetBlending(Blending::Enum blending) const override;
		void SetScissor(const RectangleInt& dimension) const override;
		void SetViewport(const RectangleInt& size) const override;
		void ClearColor(const Color& color) const override;
		void ClearDepth(float reset) const override;
		void ClearStencil(UInt32 reset) const override;

	public:
		static ElementType::Enum FindType(GLenum gl_type);

	private:
		int m_HardwareLimits[HardwareLimit::Count];
		bool m_ActiveStates[RenderStates::Count];
	};
}