#include "Vortex/Graphics/Renderer.h"
#include "Vortex/Debug/Assert.h"

namespace Vortex::Graphics {
	Renderer::Renderer(RenderBackend* render_backend, const Int32* resolution)
		: m_RenderBackend{render_backend},
		  m_RenderTimerQuery{m_RenderBackend->CreateTimerQuery()},
		  m_RenderTime{0},
		  m_ComputeShaderTimerQuery{m_RenderBackend->CreateTimerQuery()},
		  m_ComputeShaderTime{0},

		  DefaultView{} {

		GenerateDefaultView(resolution);
	}

	Renderer::~Renderer() {
		VORTEX_LOG_DEBUG("[Renderer] Shutdown begin.")
		DestroyView(DefaultView);
		m_RenderBackend->DestroyTimerQuery(m_ComputeShaderTimerQuery);
		m_RenderBackend->DestroyTimerQuery(m_RenderTimerQuery);

#ifdef VORTEX_DEBUG
		if (!m_Datas.d_ActiveIDs.empty()) {
			VORTEX_LOG_DEBUG("[Renderer] following handles was active:")
			for (auto handle : m_Datas.d_ActiveIDs) {
				if (m_Datas.Is<Mesh>(handle)) {
					VORTEX_LOG_DEBUG("[Renderer] Mesh %u", handle)
				} else if (m_Datas.Is<Material>(handle)) {
					VORTEX_LOG_DEBUG("[Renderer] Material %u", handle)
				} else if (m_Datas.Is<View>(handle)) {
					VORTEX_LOG_DEBUG("[Renderer] View %u", handle)
				} else if (m_Datas.Is<ComputeShader>(handle)) {
					VORTEX_LOG_DEBUG("[Renderer] ComputeShader %u", handle)
				}
			}
		}
#endif
		VORTEX_LOG_DEBUG("[Renderer] Shutdown end.")
	}

	Handle Renderer::CreateComputeShader(const char* source, OnComputeShaderBindFn on_compute_shader_bind) {
		ShaderType::Enum type{ShaderType::Compute};

		auto shader_handle = m_RenderBackend->CreateShader(&source, &type, 1);
		auto handle = m_Datas.Insert<ComputeShader>(
			{
				shader_handle
				, on_compute_shader_bind
			}
		);
		return handle;
	}
	void Renderer::DestroyComputeShader(Handle handle) {
		VORTEX_ASSERT(m_Datas.Is<ComputeShader>(handle))
		const auto& compute_shader = m_Datas.Get<ComputeShader>(handle);

		m_RenderBackend->DestroyShader(compute_shader.Shader);

	}

	Handle Renderer::CreateMesh(
		Topology::Enum topology,
		BufferUsage::Enum usage,
		const MeshAttribute::Enum* attributes,
		SizeType attribute_count,
		SizeType vertex_count,
		SizeType index_count) {
		VORTEX_ASSERT(attributes != nullptr)
		VORTEX_ASSERT(attribute_count < MeshAttribute::Count)

		auto vertex_array = m_RenderBackend->CreateVertexArray(topology);
		auto index_buffer = m_RenderBackend->CreateBuffer(usage, {{ElementType::UInt1}}, index_count, nullptr);
		m_RenderBackend->SetIndexBuffer(vertex_array, index_buffer);

		Mesh data{
			vertex_array
			, index_buffer
			, {0}
			, index_count
			, index_count
			, vertex_count
		};

#ifdef VORTEX_DEBUG
		//raise assert on duplicates
		for (int i = 0; i < attribute_count; ++i) {
			for (int j = 0; j < attribute_count; ++j) {
				if (i == j) continue;
				VORTEX_ASSERT(attributes[i] != attributes[j])
			}
		}
#endif

		for (int i = 0; i < attribute_count; ++i) {
			auto attribute = attributes[i];
			auto attribute_element_type = MeshAttribute::ElementType[attribute];
			auto attribute_element_size = ElementType::Size[attribute_element_type];

			auto buffer = CreateBuffer(
				usage,
				{{attribute_element_type}},
				vertex_count * attribute_element_size,
				nullptr
			);
			data.Buffers[attribute] = buffer;
			m_RenderBackend->AddVertexBuffer(vertex_array, buffer);
		}

		auto handle = m_Datas.Insert<Mesh>(data);

		return handle;
	}
	void Renderer::SetMeshIndexCount(Handle mesh_handle, SizeType count) {
		VORTEX_ASSERT(m_Datas.Contains(mesh_handle))
		VORTEX_ASSERT(m_Datas.Is<Mesh>(mesh_handle))
		auto& mesh = m_Datas.Get<Mesh>(mesh_handle);

		if (count < mesh.IndexCapacity) {
			mesh.IndexCount = count;
		} else {
			mesh.IndexCount = mesh.IndexCapacity;
		}
	}
	void Renderer::SetMeshIndices(Handle mesh_handle, const UInt32* data, SizeType count) {
		VORTEX_ASSERT(m_Datas.Contains(mesh_handle))
		VORTEX_ASSERT(m_Datas.Is<Mesh>(mesh_handle))
		auto& mesh = m_Datas.Get<Mesh>(mesh_handle);

		if (count > mesh.IndexCapacity) {
			count = mesh.IndexCapacity;
		}

		m_RenderBackend->UpdateBuffer(mesh.IndexBuffer, 0, count * sizeof(UInt32), data);
		mesh.IndexCount = count;
	}

	void Renderer::SetMeshAttribute(Handle mesh_handle, MeshAttribute::Enum attribute, const float* data, SizeType count) {
		VORTEX_ASSERT(m_Datas.Contains(mesh_handle))
		VORTEX_ASSERT(m_Datas.Is<Mesh>(mesh_handle))
		VORTEX_ASSERT(attribute < MeshAttribute::Count)
		auto& mesh = m_Datas.Get<Mesh>(mesh_handle);

		if (count > mesh.VertexCapacity) {
			count = mesh.VertexCapacity;
		}

		auto attribute_handle = mesh.Buffers[attribute];
		auto attribute_elem_type = MeshAttribute::ElementType[attribute];
		auto attribute_size = ElementType::Size[attribute_elem_type];

		VORTEX_ASSERT_MSG(m_RenderBackend->Contains(attribute_handle), "Mesh does not have %s attribute.", MeshAttribute::ToString[attribute])

		m_RenderBackend->UpdateBuffer(attribute_handle, 0, count * attribute_size, data);
	}
	void Renderer::DestroyMesh(Handle mesh_handle) {
		VORTEX_ASSERT(m_Datas.Is<Mesh>(mesh_handle))
		VORTEX_ASSERT(m_Datas.Contains(mesh_handle))
		const auto& mesh = m_Datas.Get<Mesh>(mesh_handle);

		for (auto buffer_handle : mesh.Buffers) {
			if (m_RenderBackend->Contains(buffer_handle)) {
				m_RenderBackend->DestroyBuffer(buffer_handle);
			}
		}
		m_RenderBackend->DestroyBuffer(mesh.IndexBuffer);
		m_RenderBackend->DestroyVertexArray(mesh.VertexArray);
		m_Datas.Destroy(mesh_handle);
	}

	Handle Renderer::CreateMaterial(Handle shader_handle, Vortex::Graphics::Blending::Enum blending, OnMaterialBindFn on_material_bind) {
		auto handle = m_Datas.Insert<Material>(
			{
				shader_handle
				, on_material_bind
				, blending
			}
		);

		return handle;
	}
	void Renderer::DestroyMaterial(Handle material_handle) {
		VORTEX_ASSERT(m_Datas.Is<Material>(material_handle))
		const auto& material = m_Datas.Get<Material>(material_handle);
		m_Datas.Destroy(material_handle);
	}

	Handle Renderer::CreateView(
		const RectangleInt& viewport,
		const Matrix4& projection_matrix,
		const Matrix4& view_matrix
	) {

		Color color{0.2f, 0.3f, 0.3f, 1.0f};
		View view{
			viewport
			, color
			, projection_matrix
			, view_matrix
			, RendererDataMap::NullHandle
		};

		auto handle = m_Datas.Insert<View>(view);
		return handle;
	}
	Handle Renderer::CreateBufferedView(
		const RectangleInt& viewport,
		const Matrix4& projection_matrix,
		const Matrix4& view_matrix
	) {
		UInt16 attachment_size[]{
			static_cast<UInt16>(viewport.Width())
			, static_cast<UInt16>(viewport.Height())
		};

		FrameBufferAttachment::Enum attachments[]{
			FrameBufferAttachment::Color0
			, FrameBufferAttachment::DepthStencil
		};
		auto attachments_size = Vortex::ArrayCount(attachments);

		Handle frame_buffer = m_RenderBackend->CreateFrameBuffer(attachment_size, attachments, attachments_size);

		Color color{0.2f, 0.3f, 0.3f, 1.0f};
		View view{
			viewport
			, color
			, projection_matrix
			, view_matrix
			, frame_buffer
		};

		auto handle = m_Datas.Insert<View>(view);
		return handle;
	}
	void Renderer::DestroyView(Handle view_handle) {
		VORTEX_ASSERT(m_Datas.Is<View>(view_handle))
		const auto& view = m_Datas.Get<View>(view_handle);

		if (m_RenderBackend->Contains(view.FrameBuffer)) {
			m_RenderBackend->DestroyFrameBuffer(view.FrameBuffer);
		}
		m_Datas.Destroy(view_handle);
	}

	void Renderer::OnEvent(const Event& event) {
		if (event.Type == EventType::WindowResize) {
			auto& view = m_Datas.Get<View>(DefaultView);

			view.ProjectionMatrix.SetOrthographic(
				0.0f,
				static_cast<float>(event.Size[0]),
				static_cast<float>(event.Size[1]),
				0.0f,
				-10.0f,
				10.0f
			);
			view.Viewport.Width() = event.Size[0];
			view.Viewport.Height() = event.Size[1];
		}
	}

	void Renderer::ProcessComputeShaderCommands() {
		if (!m_ComputeCommands.empty()) {
#ifdef VORTEX_DEBUG
			d_CheckComputeCommands();
#endif

			m_RenderBackend->BeginTimerQuery(m_ComputeShaderTimerQuery);
			for (const auto& command :  m_ComputeCommands) {
				auto& compute_shader = m_Datas.Get<ComputeShader>(command.ComputeShader);

				m_RenderBackend->BindShader(compute_shader.Shader);
				ComputeShaderBinder binder{m_RenderBackend, compute_shader};
				compute_shader.OnBind(binder);

				m_RenderBackend->Dispatch(command.GroupCount.Data());
			}

			UInt32 queried_time;
			if (m_RenderBackend->EndTimerQuery(m_ComputeShaderTimerQuery, queried_time)) {
				m_ComputeShaderTime = static_cast<float>(queried_time) / 1000000.0f;
			}
			m_ComputeCommands.clear();
		}
	}

	void Renderer::SortDrawCommands() {
		for (auto& command :  m_DrawCommands) {
			//calculate depths
			if (DrawCall::GetViewLayer(command.DrawCall) != ViewLayer::PostProcess) {
				Handle view_handle = DrawCall::GetViewHandle(command.DrawCall);
				Vector3 mesh_position;
				Vector3 view_position;

				m_Datas.Get<View>(view_handle).ViewMatrix.ExtractTranslation(view_position);
				command.TransformMatrix.ExtractTranslation(mesh_position);

				float distance{mesh_position.Distance(view_position)};

				DrawCall::SetDepth(command.DrawCall, static_cast<UInt32>(distance));
			}
		}

		std::sort(m_DrawCommands.begin(), m_DrawCommands.end(),
				  [](const DrawCommand& lhs, const DrawCommand& rhs) {
					  return lhs.DrawCall < rhs.DrawCall;
				  }
		);
	}
	void Renderer::ProcessDrawCommands() {
		if (!m_DrawCommands.empty()) {
#ifdef VORTEX_DEBUG
			d_CheckDrawCommands();
#endif
			SortDrawCommands();

			Handle active_view_handle{RendererDataMap::NullHandle};
			Blending::Enum active_blending{Blending::Null};
			ViewLayer::Enum active_view_layer{ViewLayer::Null};

			Handle active_material_handle{RendererDataMap::NullHandle};

			Matrix4 inverted_view_matrix;
			Vector2 view_resolution;
			m_RenderBackend->BeginTimerQuery(m_RenderTimerQuery);

			for (const auto& command :  m_DrawCommands) {
				auto command_view_handle = DrawCall::GetViewHandle(command.DrawCall);
				const auto& command_view = m_Datas.Get<View>(command_view_handle);

				//set view
				if (command_view_handle != active_view_handle) {
					active_view_handle = command_view_handle;

					if (m_Datas.Contains(command_view.FrameBuffer)) {
						m_RenderBackend->BindFrameBuffer(command_view.FrameBuffer);
					} else {
						m_RenderBackend->BindBackBuffer();
					}

					m_RenderBackend->ClearColor(command_view.ClearColor);
					m_RenderBackend->SetViewport(command_view.Viewport);
					m_RenderBackend->SetDepthTest(DepthTesting::Less);

					inverted_view_matrix = command_view.ViewMatrix;
					inverted_view_matrix.Invert();

					view_resolution.Set(
						static_cast<float>(command_view.Viewport.x()),
						static_cast<float>(command_view.Viewport.y())
					);
				}

				auto command_view_layer = DrawCall::GetViewLayer(command.DrawCall);

				if (command_view_layer != ViewLayer::PostProcess) {
					Blending::Enum command_blending;
					Handle command_material_handle;
					UInt32 command_depth;
					Handle command_mesh_handle = command.MeshHandle;

					DrawCall::DecodeDrawCall(command.DrawCall, command_blending, command_material_handle, command_depth);

					const auto& command_material = m_Datas.Get<Material>(command_material_handle);
					const auto& command_transform = command.TransformMatrix;

					if (command_blending != active_blending) {
						active_blending = command_blending;
						m_RenderBackend->SetBlending(command_blending);
					}

					//clear depth when changing layers
					if (command_view_layer != active_view_layer) {
						m_RenderBackend->ClearDepth(1.0f);
					}

					//bind material
					if (command_material_handle != active_material_handle || command_view_layer != active_view_layer) {
						active_material_handle = command_material_handle;
						active_view_layer = command_view_layer;

						m_RenderBackend->BindShader(command_material.Shader);
						if (command_material.OnBind != nullptr) {
							MaterialBinder material_binder{m_RenderBackend, command_material};
							command_material.OnBind(material_binder);
						}

						if (command_view_layer == ViewLayer::World) {
							m_RenderBackend->SetUniform(
								command_material.Shader,
								ShaderConstants::ToHashedString[ShaderConstants::Matrix4_View],
								inverted_view_matrix.Data(),
								1
							);
						} else if (command_view_layer == ViewLayer::HUD) {
							Matrix4 identity;

							m_RenderBackend->SetUniform(
								command_material.Shader,
								ShaderConstants::ToHashedString[ShaderConstants::Matrix4_View],
								identity.Data(),
								1
							);
						}

						const auto& projection_matrix = command_view.ProjectionMatrix;
						m_RenderBackend->SetUniform(
							command_material.Shader,
							ShaderConstants::ToHashedString[ShaderConstants::Matrix4_Projection],
							projection_matrix.Data(),
							1
						);

						m_RenderBackend->SetUniform(
							command_material.Shader,
							ShaderConstants::ToHashedString[ShaderConstants::Float2_Resolution],
							view_resolution.Data(),
							1
						);
					}

					m_RenderBackend->SetUniform(
						command_material.Shader,
						ShaderConstants::ToHashedString[ShaderConstants::Matrix4_Transform],
						command_transform.Data(),
						1
					);

					const auto& command_mesh = m_Datas.Get<Mesh>(command_mesh_handle);
					m_RenderBackend->Draw(command_mesh.VertexArray, command_mesh.IndexCount);
				} else {

					// Apply Post Processing to view
					Handle command_postprocess_handle{RendererDataMap::NullHandle};
					DrawCall::DecodePostProcessCall(command.DrawCall, command_postprocess_handle);

					const auto& command_postprocess = m_Datas.Get<PostProcess>(command_postprocess_handle);
					m_RenderBackend->BindShader(command_postprocess.Shader);

					PostProcessBinder postprocess_binder{m_RenderBackend, command_postprocess};
					command_postprocess.OnBind(postprocess_binder);

				}
			}
			UInt32 queried_time;
			if (m_RenderBackend->EndTimerQuery(m_RenderTimerQuery, queried_time)) {
				m_RenderTime = static_cast<float>(queried_time) / 1000000.0f;
			}
			m_RenderBackend->BindBackBuffer();
			m_DrawCommands.clear();
		}
	}
	void Renderer::GenerateDefaultView(const Int32* resolution) {
		Matrix4 identity;
		Matrix4 projection;
		projection.SetOrthographic(
			0.0f,
			static_cast<float>(resolution[0]),
			static_cast<float>(resolution[1]),
			0.0f,
			-10.0f,
			10.0f
		);

		Int32 viewport[]{0, 0, resolution[0], resolution[1]};

		Color color{0.2f, 0.3f, 0.3f, 1.0f};
		DefaultView = m_Datas.Insert<View>(
			{
				viewport
				, color
				, projection
				, identity
				, RendererDataMap::NullHandle
			}
		);
	}

#ifdef VORTEX_DEBUG
	void Renderer::d_CheckDrawCommands() {
		for (auto& command :  m_DrawCommands) {

			auto view_handle = DrawCall::GetViewHandle(command.DrawCall);
			VORTEX_ASSERT(view_handle == command.d_ViewHandle)
			VORTEX_ASSERT(m_Datas.Contains(view_handle))

			auto view_layer = DrawCall::GetViewLayer(command.DrawCall);
			VORTEX_ASSERT(view_layer == command.d_ViewLayer)

			if (view_layer == ViewLayer::PostProcess) {
				Handle postprocess_handle;

				DrawCall::DecodePostProcessCall(command.DrawCall, postprocess_handle);
				VORTEX_ASSERT(m_Datas.Contains(postprocess_handle))

			} else {
				Blending::Enum blending;
				Handle material_handle;
				UInt32 depth;

				DrawCall::DecodeDrawCall(
					command.DrawCall,
					blending,
					material_handle,
					depth
				);

				VORTEX_ASSERT(blending == command.d_Blending)
				VORTEX_ASSERT(material_handle == command.d_MaterialHandle)
				VORTEX_ASSERT(depth == command.d_Depth)

				VORTEX_ASSERT(m_Datas.Contains(material_handle))
				VORTEX_ASSERT(m_Datas.Contains(command.MeshHandle))
			}
		}
	}
	void Renderer::d_CheckComputeCommands() {
		for (auto& command :  m_ComputeCommands) {
			VORTEX_ASSERT(m_Datas.Contains(command.ComputeShader))
		}
	}
#endif
}