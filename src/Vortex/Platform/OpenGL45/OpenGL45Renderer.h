#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "OpenGL45/OpenGL45Enums.h"
#include "Vortex/Graphics/Renderer.h"

namespace Vortex::Graphics {
	class OpenGL45Renderer: public Renderer {
	public:
		OpenGL45Renderer();
		~OpenGL45Renderer() override;

	public:
		Handle CreateWindow(const Resolution& resolution, const char* title) override;
		void DestroyWindow(Handle window_handle) override;
		void SetWindowResolution(Handle window_handle, const Resolution& resolution) override;
		void SetWindowTitle(Handle window_handle, const char* title) override;
		void SetWindowIcon(Handle window_handle, UInt16 width, UInt16 height, void* pixels) override;
		void SetWindowFullscreen(Handle window_handle, Handle monitor_handle) override;
		void SetWindowEventCallback(Handle window_handle, EventCallbackFn event_fn) override;
		void SetCursorVisibility(Handle window_handle, bool value) override;
	protected:
		struct WindowDataStruct {
			EventCallbackFn event_callback_fn;
			Handle window_handle;
		};

	public:
		Handle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) override;
		void UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) override;
		void GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) override;
		void DestroyBuffer(Handle buffer_handle) override;

	public:
		Handle CreateTexture2D(const Vector2HalfInt& size, PixelFormat::Enum format, const void* pixels, TextureLODFilter::Enum min_filter, TextureLODFilter::Enum mag_filter, TextureWrap::Enum wrap_s, TextureWrap::Enum wrap_t, bool create_mipmap) override;
		void UpdateTexture2D(Handle texture_handle, const void* pixels) override;
		void GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) override;
		void DestroyTexture(Handle texture_handle) override;

	public:
		Handle CreateShader(const char** sources, ShaderType::Enum* types, SizeType count, ShaderTags::Enum tags) override;
		bool ReloadShader(Handle shader_handle, const char** sources, ShaderType::Enum* types, SizeType count) override;
		void SetUniform(Handle shader_handle, HashedString name, const void* data, SizeType count) const override;
		void DestroyShader(Handle shader_handle) override;
	protected:
		static GLuint CompileShaderSource(const char* source, ShaderType::Enum type);
		static GLuint LinkShaderProgram(GLuint* shader_ids, SizeType count);
		static ElementType::Enum FindType(GLenum gl_type);
		static void QueryShaderTypes(Shader& data, GLuint program_id);

	public:
		Handle CreateFrameBuffer(
			const Vector2HalfInt& size,
			const PixelFormat::Enum* attachment_formats,
			SizeType count
		) override;
		void DestroyFrameBuffer(Handle framebuffer_handle) override;

	public:
		Handle CreateComputeShader(const char* source, OnComputeShaderBindFn on_compute_shader_bind) override;
		void DestroyComputeShader(Handle handle) override;

	public:
		Handle CreateMesh(
			Topology::Enum topology,
			BufferUsage::Enum usage,
			const MeshLayout& layout,
			SizeType vertex_capacity,
			SizeType index_capacity
		) override;
		void SetMeshIndexCount(Handle mesh_handle, SizeType count) override;
		void SetMeshIndices(Handle mesh_handle, const UInt32* data, SizeType count) override;
		void SetMeshData(Handle mesh_handle, SizeType index, const void* data, SizeType size) override;
		void DrawMesh(Handle mesh_handle) const;
		void DestroyMesh(Handle mesh_handle) override;
	protected:
		static void AddVertexBuffer(
			GLuint gl_mesh_id,
			GLuint gl_buffer_id,
			const BufferLayout& buffer_layout,
			SizeType& buffer_binding_index
		);

	public:
		Handle CreateMaterial(Handle shader_handle, Vortex::Graphics::Blending::Enum blending, OnMaterialBindFn on_material_bind) override;
		void DestroyMaterial(Handle material_handle) override;

	public:
		Handle CreateView(
			const Math::RectangleInt& viewport,
			const Math::Matrix4& projection_matrix,
			const Math::Matrix4& view_matrix,
			DepthTesting::Enum depth_test,
			Handle framebuffer_handle
		) override;
		void DestroyView(Handle view_handle) override;

	public:
		Handle CreateDrawSurface(
			const Math::RectangleInt& area,
			const Math::Color& clear_color,
			float clear_depth,
			Int32 clear_stencil
		) override;
		void DestroyDrawSurface(Handle draw_surface_handle) override;

	public:
		void NextFrame() override;
		void OnEvent(const Event& event) override;

	protected:
		void SortDrawCommands();
		void ProcessDrawCommands();
		void ProcessComputeCommands();

	protected:
		GLFWwindow* m_CurentWindowContext;
		bool m_FirstTime;
	};
}