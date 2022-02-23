#include "Vortex/Graphics/Renderer.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex::Graphics {
	Renderer::Renderer(RenderBackend* render_backend, const Int32* resolution)
	: m_NextMeshHandle{0},
	m_NextMaterialHandle{0},
	m_NextViewHandle{0},
	m_RenderBackend{render_backend},
	StandardProgram{} {

		CreateStandardAssets(resolution);
	}

	Renderer::~Renderer() {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		for (const auto& node : m_MeshDatas) {
			const auto& mesh_data = node.second;
			m_RenderBackend->Destroy(mesh_data.IndexBuffer);
			m_RenderBackend->Destroy(mesh_data.PositionBuffer);
			m_RenderBackend->Destroy(mesh_data.UV0Buffer);
			m_RenderBackend->Destroy(mesh_data.UV1Buffer);
			m_RenderBackend->Destroy(mesh_data.ColorBuffer);
			m_RenderBackend->Destroy(mesh_data.VertexArray);
		}
		m_RenderBackend->Destroy(StandardProgram);
		m_RenderBackend->Destroy(StandardWhiteTexture);
		m_RenderBackend->Destroy(StandardErrorTexture);
	}

	void Renderer::CreateStandardAssets(const Int32* resolution) {
		char vertex_shader_source[]{
			R"(
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_uv0;
layout(location = 3) in vec2 a_uv1;

out vec4 vertex_color;
out vec2 vertex_uv0;
out vec2 vertex_uv1;

uniform mat4 Mat4_Projection;
uniform mat4 Mat4_View;
uniform mat4 Mat4_Model;

void main(){
	vertex_color = a_color;
	vertex_uv0 = a_uv0;
	vertex_uv1 = a_uv1;
	gl_Position = Mat4_Projection * Mat4_View * Mat4_Model * vec4(a_position, 1.0f);
}
			)"
		};

		char fragment_shader_source[]{
			R"(
#version 450 core

uniform sampler2D Texture0;
uniform sampler2D Texture1;

in vec4 vertex_color;
in vec2 vertex_uv0;
in vec2 vertex_uv1;

void main() {
	vec4 tex0_color = texture2D(Texture0, vertex_uv0);
	vec4 tex1_color = texture2D(Texture1, vertex_uv1);

	gl_FragColor = vertex_color * tex0_color * tex1_color;
}
			)"
		};

		Vortex::Graphics::ShaderHandle handle[2];
		handle[0] = m_RenderBackend->CreateShader(Vortex::Graphics::ShaderType::Vertex, vertex_shader_source);
		handle[1] = m_RenderBackend->CreateShader(Vortex::Graphics::ShaderType::Fragment, fragment_shader_source);

		StandardProgram = m_RenderBackend->CreateProgram(handle, 2, true);
		m_RenderBackend->SetShaderInt1(StandardProgram, "Texture0", 0);
		m_RenderBackend->SetShaderInt1(StandardProgram, "Texture1", 1);

		UInt8 white_texture[]{
			255, 255, 255, 255
			, 255, 255, 255, 255
			, 255, 255, 255, 255
			, 255, 255, 255, 255
		};
		StandardWhiteTexture = m_RenderBackend->CreateTexture2D(2, 2, PixelFormat::RGBA_UI8, white_texture);

		UInt8 error_texture[]{
			255, 0, 255
			, 0, 0, 0
			, 0, 0, 0
			, 255, 0, 255
		};
		StandardErrorTexture = m_RenderBackend->CreateTexture2D(2, 2, PixelFormat::RGB_UI8, error_texture);

		StandardMaterial = CreateMaterial(StandardProgram, StandardWhiteTexture, StandardWhiteTexture);

		float identity_matrix[]{
			1.0f, 0.0f, 0.0f, 0.0f
			, 0.0f, 1.0f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.0f, 0.0f, 0.0f, 1.0f
		};

		Int32 viewport[]{
			0, 0, resolution[0], resolution[1]
		};

		float projection_mat[16];
		std::memcpy(projection_mat, identity_matrix, 16 * sizeof(float));
		{
			float left = 0.0f;
			float right = static_cast<float>(resolution[0]);
			float bottom = static_cast<float>(resolution[1]);
			float top = 0.0f;
			float near = -10.0f;
			float far = 10.0f;

			float dRL_inv = 1.0f / (right - left);
			float dTB_inv = 1.0f / (top - bottom);
			float dNF_inv = 1.0f / (near - far);

			projection_mat[0] = 2.0f * dRL_inv;
			projection_mat[5] = 2.0f * dTB_inv;
			projection_mat[10] = 2.0f * dNF_inv;

			projection_mat[12] = -(right + left) * dRL_inv;
			projection_mat[13] = -(top + bottom) * dTB_inv;
			projection_mat[14] = -(far + near) * dNF_inv;
			projection_mat[15] = 1.0f;
		}

		DefaultView = CreateView(viewport, projection_mat, identity_matrix);
	}

	MeshHandle Renderer::CreateMesh(Topology::Enum topology, SizeType vertex_count, SizeType index_count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)

		auto vertex_array = m_RenderBackend->CreateVertexArray(topology);
		auto index_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::UInt1}, index_count, nullptr);

		auto position_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float3}, vertex_count * 3, nullptr);
		auto color_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float4}, vertex_count * 4, nullptr);
		auto uv0_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float2}, vertex_count * 2, nullptr);
		auto uv1_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float2}, vertex_count * 2, nullptr);

		m_RenderBackend->SetIndexBuffer(vertex_array, index_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, position_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, color_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, uv0_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, uv1_buffer);

		MeshHandle handle{m_NextMeshHandle};
		++m_NextMeshHandle;
		MeshData data{
			vertex_array
			, index_buffer
			, position_buffer
			, color_buffer
			, uv0_buffer
			, uv1_buffer
			, index_count
		};

		m_MeshDatas[handle] = data;
		return handle;
	}
	void Renderer::Destroy(MeshHandle handle) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);

		m_RenderBackend->Destroy(mesh_data.IndexBuffer);
		m_RenderBackend->Destroy(mesh_data.PositionBuffer);
		m_RenderBackend->Destroy(mesh_data.UV0Buffer);
		m_RenderBackend->Destroy(mesh_data.UV1Buffer);
		m_RenderBackend->Destroy(mesh_data.ColorBuffer);
		m_RenderBackend->Destroy(mesh_data.VertexArray);

		m_MeshDatas.erase(handle);
	}

	void Renderer::SetIndices(MeshHandle handle, const UInt32* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.IndexBuffer, 0, count * sizeof(UInt32), data);
		mesh_data.IndexCount = count;
	}
	void Renderer::SetPositions(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.PositionBuffer, 0, count * sizeof(float) * 3, data);
	}
	void Renderer::SetColors(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.ColorBuffer, 0, count * sizeof(float) * 4, data);
	}

	void Renderer::SetUV0(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.UV0Buffer, 0, count * sizeof(float) * 2, data);
	}
	void Renderer::SetUV1(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.UV1Buffer, 0, count * sizeof(float) * 2, data);
	}

	MaterialHandle Renderer::CreateMaterial(ProgramHandle shader, TextureHandle diffuse, TextureHandle mask) {
		MaterialHandle handle{m_NextMaterialHandle};
		++m_NextMaterialHandle;
		MaterialData data{
			shader
			, diffuse
			, mask
		};

		m_MaterialDatas[handle] = data;
		return handle;
	}
	void Renderer::Destroy(MaterialHandle handle) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mat_data = m_MaterialDatas.at(handle);

		m_MaterialDatas.erase(handle);
	}

	ViewHandle Renderer::CreateView(const Int32* viewport, const float* projection_matrix, const float* view_matrix) {
		ViewHandle handle{m_NextViewHandle};
		++m_NextViewHandle;
		ViewData data;

		float color[]{0.2f, 0.3f, 0.3f, 1.0f};
		std::memcpy(data.Viewport, viewport, 4 * sizeof(float));
		std::memcpy(data.ClearColor, color, 4 * sizeof(float));
		std::memcpy(data.ProjectionMatrix, projection_matrix, 16 * sizeof(float));
		std::memcpy(data.ViewMatrix, view_matrix, 16 * sizeof(float));

		m_ViewDatas[handle] = data;
		return handle;
	}
	void Renderer::Destroy(ViewHandle handle) {
		VORTEX_ASSERT(IsValid(handle))

		m_ViewDatas.erase(handle);
	}
	void Renderer::SetClearColor(ViewHandle handle, const float* color) {
		VORTEX_ASSERT(IsValid(handle))
		auto& view_data = m_ViewDatas.at(handle);

		std::memcpy(view_data.ClearColor, color, 4 * sizeof(float));
	}
	void Renderer::SetViewport(ViewHandle handle, const Int32* viewport) {
		VORTEX_ASSERT(IsValid(handle))
		auto& view_data = m_ViewDatas.at(handle);

		std::memcpy(view_data.Viewport, viewport, 4 * sizeof(float));
	}
	void Renderer::SetProjectionMatrix(ViewHandle handle, const float* projection) {
		VORTEX_ASSERT(IsValid(handle))
		auto& view_data = m_ViewDatas.at(handle);

		std::memcpy(view_data.ProjectionMatrix, projection, 16 * sizeof(float));
	}
	void Renderer::SetViewMatrix(ViewHandle handle, const float* view) {
		VORTEX_ASSERT(IsValid(handle))
		auto& view_data = m_ViewDatas.at(handle);

		std::memcpy(view_data.ViewMatrix, view, 16 * sizeof(float));
	}

	void Renderer::Process() {
		//std::sort(m_DrawCommands.Data(), m_DrawCommands.Data() + m_DrawCommands.Size());

		MaterialHandle current_material;
		ViewHandle current_view;
		Translucency::Enum current_translucency{Translucency::Opaque};

		const ViewData* view_data_ptr{nullptr};
		const MaterialData* material_data_ptr{nullptr};

		m_RenderBackend->SetState(RenderStates::Blending, false);

		for (auto command :  m_DrawCommands) {
			auto mesh_handle = command.GetMesh();
			auto mat_handle = command.GetMaterial();
			auto view_handle = command.GetView();
			const auto* transform = command.GetTransform();
			auto translucency = command.GetTranslucency();

			//set translucency
			if (current_translucency != translucency) {
				current_translucency = translucency;
				switch (translucency) {
					case Translucency::Opaque: {
						m_RenderBackend->SetState(RenderStates::Blending, false);
						break;
					}
					case Translucency::Additive: {
						m_RenderBackend->SetState(RenderStates::Blending, true);
						m_RenderBackend->SetBlendFunction(BlendFunction::SourceAlpha, BlendFunction::OneMinusSourceAlpha);
						break;
					}
					case Translucency::Subtractive: {
						m_RenderBackend->SetState(RenderStates::Blending, true);
						m_RenderBackend->SetBlendFunction(BlendFunction::DestinationColor, BlendFunction::Zero);
						break;
					}
				}
			}

			//set view
			if (current_view != view_handle) {
				current_view = view_handle;
				view_data_ptr = &m_ViewDatas.at(current_view);

				m_RenderBackend->ClearColor(view_data_ptr->ClearColor);
				m_RenderBackend->SetViewport(view_data_ptr->Viewport);
			}

			//set material
			if (current_material != mat_handle) {
				current_material = mat_handle;
				material_data_ptr = &m_MaterialDatas.at(mat_handle);

				m_RenderBackend->Bind(material_data_ptr->Program);

				const float* projection_mat = view_data_ptr->ProjectionMatrix;
				const float* view_mat = view_data_ptr->ViewMatrix;

				m_RenderBackend->SetShaderMatrix4(material_data_ptr->Program, "Mat4_Projection", false, projection_mat);
				m_RenderBackend->SetShaderMatrix4(material_data_ptr->Program, "Mat4_View", false, view_mat);

				m_RenderBackend->Bind(material_data_ptr->Texture0, 0);
				m_RenderBackend->Bind(material_data_ptr->Texture1, 1);
			}
			const auto& mesh_data = m_MeshDatas.at(mesh_handle);

			m_RenderBackend->SetShaderMatrix4(material_data_ptr->Program, "Mat4_Model", false, transform);
			m_RenderBackend->Draw(mesh_data.VertexArray, mesh_data.IndexCount);
		}

		m_DrawCommands.Clear();
	}
}