#include "Vortex/Graphics/Renderer.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex::Graphics {
	Renderer::Renderer(RenderBackend* render_backend)
		: m_NextMeshHandle{0},
		  m_NextMaterialHandle{0},
		  m_NextViewHandle{0},
		  m_RenderBackend{render_backend} {
	}

	Renderer::~Renderer() {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		for (const auto& node : m_MeshDatas) {
			const auto& mesh_data = node.second;
			m_RenderBackend->Destroy(mesh_data.IndexBuffer);
			m_RenderBackend->Destroy(mesh_data.PositionBuffer);
			m_RenderBackend->Destroy(mesh_data.NormalBuffer);
			m_RenderBackend->Destroy(mesh_data.UVBuffer);
			m_RenderBackend->Destroy(mesh_data.ColorBuffer);
			m_RenderBackend->Destroy(mesh_data.VertexArray);
		}
	}

	MeshHandle Renderer::CreateMesh(Topology::Enum topology, SizeType vertex_count, SizeType index_count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)

		auto vertex_array = m_RenderBackend->CreateVertexArray(topology);
		auto index_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::UInt1}, index_count, nullptr);

		auto position_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float3}, vertex_count * 3, nullptr);
		auto normal_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float3}, vertex_count * 3, nullptr);
		auto color_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float4}, vertex_count * 4, nullptr);
		auto uv_buffer = m_RenderBackend->CreateBuffer(BufferUsage::DynamicDraw, {RenderElementType::Float2}, vertex_count * 2, nullptr);

		m_RenderBackend->SetIndexBuffer(vertex_array, index_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, position_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, normal_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, color_buffer);
		m_RenderBackend->AddVertexBuffer(vertex_array, uv_buffer);

		MeshHandle handle{m_NextMeshHandle};
		++m_NextMeshHandle;
		MeshData data{
			vertex_array
			, index_buffer
			, position_buffer
			, normal_buffer
			, color_buffer
			, uv_buffer
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
		m_RenderBackend->Destroy(mesh_data.NormalBuffer);
		m_RenderBackend->Destroy(mesh_data.UVBuffer);
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
	void Renderer::SetNormals(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.NormalBuffer, 0, count * sizeof(float) * 3, data);
	}
	void Renderer::SetColors(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.ColorBuffer, 0, count * sizeof(float) * 4, data);
	}
	void Renderer::SetUVs(MeshHandle handle, const float* data, SizeType count) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))
		const auto& mesh_data = m_MeshDatas.at(handle);
		m_RenderBackend->UpdateBuffer(mesh_data.UVBuffer, 0, count * sizeof(float) * 2, data);
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

	ProgramHandle Renderer::SetProgram(MaterialHandle handle, ProgramHandle program) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))

		auto& material_data = m_MaterialDatas.at(handle);
		auto old_handle = material_data.Program;
		material_data.Program = program;

		return old_handle;
	}
	TextureHandle Renderer::SetDiffuse(MaterialHandle handle, TextureHandle diffuse) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))

		auto& material_data = m_MaterialDatas.at(handle);
		auto old_handle = material_data.Diffuse;
		material_data.Diffuse = diffuse;

		return old_handle;
	}
	TextureHandle Renderer::SetMask(MaterialHandle handle, TextureHandle mask) {
		VORTEX_ASSERT(m_RenderBackend != nullptr)
		VORTEX_ASSERT(IsValid(handle))

		auto& material_data = m_MaterialDatas.at(handle);
		auto old_handle = material_data.Mask;
		material_data.Mask = mask;

		return old_handle;
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

				m_RenderBackend->Bind(material_data_ptr->Diffuse, 0);
				m_RenderBackend->Bind(material_data_ptr->Mask, 1);
			}
			const auto& mesh_data = m_MeshDatas.at(mesh_handle);

			m_RenderBackend->SetShaderMatrix4(material_data_ptr->Program, "Mat4_Model", false, transform);
			m_RenderBackend->Draw(mesh_data.VertexArray, mesh_data.IndexCount);
		}

		m_DrawCommands.Clear();
	}
}