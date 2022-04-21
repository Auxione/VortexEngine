#pragma once
#include "Vortex/Core/Event.h"

#include "Vortex/Math/VortexMath.h"

#include "Vortex/Common/IntegerPacker.h"

#include "Vortex/Graphics/GraphicsEnum.h"
#include "Vortex/Graphics/BufferLayout.h"
#include "Vortex/Graphics/MeshLayout.h"
#include "Vortex/Graphics/GraphicsStructs.h"

namespace Vortex::Graphics {
	class Renderer {
	public:
		virtual ~Renderer() = default;

	public:    // Window
		virtual Handle CreateWindow(const Resolution& resolution, const char* title) = 0;
		virtual void DestroyWindow(Handle window_handle) = 0;

		virtual void SetWindowResolution(Handle window_handle, const Resolution& resolution) = 0;
		virtual void SetWindowTitle(Handle window_handle, const char* title) = 0;
		virtual void SetWindowIcon(Handle window_handle, UInt16 width, UInt16 height, void* pixels) = 0;
		virtual void SetWindowFullscreen(Handle window_handle, Handle monitor_handle) = 0;
		virtual void SetWindowEventCallback(Handle window_handle, EventCallbackFn event_fn) = 0;
		virtual void SetCursorVisibility(Handle window_handle, bool value) = 0;

	public:    // Buffer
		virtual Handle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) = 0;
		virtual void UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) = 0;
		virtual void GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) = 0;
		virtual void DestroyBuffer(Handle buffer_handle) = 0;

	public:    // Texture
		virtual Handle CreateTexture2D(
			const Vector2HalfInt& size,
			PixelFormat::Enum format,
			const void* pixels,
			TextureLODFilter::Enum min_filter,
			TextureLODFilter::Enum mag_filter,
			TextureWrap::Enum wrap_s,
			TextureWrap::Enum wrap_t,
			bool create_mipmap
		) = 0;
		inline Handle CreateTexture2D(
			const Vector2HalfInt& size,
			PixelFormat::Enum format,
			const void* pixels
		) {
			return CreateTexture2D(
				size,
				format,
				pixels,
				TextureLODFilter::Linear,
				TextureLODFilter::Linear,
				TextureWrap::ClampEdge,
				TextureWrap::ClampEdge,
				true
			);
		}
		virtual void UpdateTexture2D(Handle texture_handle, const void* pixels) = 0;
		virtual void GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) = 0;
		virtual void DestroyTexture(Handle texture_handle) = 0;

	public:    // Shader
		virtual Handle CreateShader(const char** sources, ShaderType::Enum* types, SizeType count, ShaderTags::Enum tags) = 0;
		inline Handle CreateShader(const char** sources, ShaderType::Enum* types, SizeType count) {
			return CreateShader(sources, types, count, ShaderTags::Undefined);
		}
		virtual bool ReloadShader(Handle shader_handle, const char** sources, ShaderType::Enum* types, SizeType count) = 0;
		virtual void SetUniform(Handle shader_handle, HashedString name, const void* data, SizeType count) const = 0;
		inline void SetUniform(Handle shader_handle, HashedString name, const void* data) const {
			SetUniform(shader_handle, name, data, 1);
		}
		virtual void DestroyShader(Handle shader_handle) = 0;

	public:    // Framebuffer
		virtual Handle CreateFrameBuffer(
			const Vector2HalfInt& size,
			const PixelFormat::Enum* attachment_formats,
			SizeType count
		) = 0;
		Handle GetFrameBufferTexture(Handle framebuffer_handle, SizeType index) const {
			VORTEX_ASSERT(d_FrameBufferChecks(framebuffer_handle))
			return m_DataMap.Get<FrameBuffer>(framebuffer_handle).TextureHandles[index];
		}
		virtual void DestroyFrameBuffer(Handle framebuffer_handle) = 0;

	public: // ComputeShader
		virtual Handle CreateComputeShader(const char* source, OnComputeShaderBindFn on_compute_shader_bind) = 0;
		template<typename T>
		inline T& GetComputeShaderData(Handle compute_shader_handle) {
			VORTEX_ASSERT(d_ComputeShaderChecks(compute_shader_handle))
			return m_DataMap.Get<ComputeShader>(compute_shader_handle).Data.template Layout<T>();
		}
		template<typename T>
		inline const T& GetComputeShaderData(Handle compute_shader_handle) const {
			VORTEX_ASSERT(d_ComputeShaderChecks(compute_shader_handle))
			return m_DataMap.Get<ComputeShader>(compute_shader_handle).Data.template Layout<T>();
		}
		virtual void DestroyComputeShader(Handle compute_shader_handle) = 0;

	public: // Mesh
		virtual Handle CreateMesh(
			Topology::Enum topology,
			BufferUsage::Enum usage,
			const MeshLayout& layout,
			SizeType vertex_capacity,
			SizeType index_capacity
		) = 0;

		virtual void SetMeshIndexCount(Handle mesh_handle, SizeType count) = 0;
		virtual void SetMeshIndices(Handle mesh_handle, const UInt32* data, SizeType count) = 0;
		virtual void SetMeshData(Handle mesh_handle, SizeType index, const void* data, SizeType size) = 0;
		virtual void DestroyMesh(Handle mesh_handle) = 0;

	public: // Material
		virtual Handle CreateMaterial(
			Handle shader_handle,
			Vortex::Graphics::Blending::Enum blending,
			OnMaterialBindFn on_material_bind
		) = 0;
		inline Material& GetMaterial(Handle material_handle) {
			VORTEX_ASSERT(d_MaterialChecks(material_handle))
			return m_DataMap.Get<Material>(material_handle);
		}
		inline const Material& GetMaterial(Handle material_handle) const {
			VORTEX_ASSERT(d_MaterialChecks(material_handle))
			return m_DataMap.Get<Material>(material_handle);
		}
		template<typename T>
		inline T& GetMaterialData(Handle material_handle) { return GetMaterial(material_handle).Data.template Layout<T>(); }
		template<typename T>
		inline const T& GetMaterialData(Handle material_handle) const { return GetMaterial(material_handle).Data.template Layout<T>(); }
		virtual void DestroyMaterial(Handle material_handle) = 0;

	public: // View
		virtual Handle CreateView(
			const Math::RectangleInt& viewport,
			const Math::Matrix4& projection_matrix,
			const Math::Matrix4& view_matrix,
			DepthTesting::Enum depth_test,
			Handle framebuffer_handle
		) = 0;
		inline Handle CreateView(
			const Math::RectangleInt& viewport,
			const Math::Matrix4& projection_matrix,
			const Math::Matrix4& view_matrix,
			DepthTesting::Enum depth_test,
			const PixelFormat::Enum* framebuffer_attachment_formats,
			SizeType framebuffer_attachment_count
		) {

			Vector2HalfInt size{
				static_cast<UInt16>(viewport.width)
				, static_cast<UInt16>(viewport.height)
			};

			auto framebuffer_handle = CreateFrameBuffer(
				size,
				framebuffer_attachment_formats,
				framebuffer_attachment_count
			);

			return CreateView(
				viewport,
				projection_matrix,
				view_matrix,
				depth_test,
				framebuffer_handle
			);
		}
		inline Handle CreateView(
			const Math::RectangleInt& viewport,
			const Math::Matrix4& projection_matrix,
			const Math::Matrix4& view_matrix,
			DepthTesting::Enum depth_test = Graphics::DepthTesting::Less
		) {

			PixelFormat::Enum framebuffer_attachment_formats[]{
				PixelFormat::RGBA_UI8
				, PixelFormat::DepthStencil
			};

			return CreateView(
				viewport,
				projection_matrix,
				view_matrix,
				depth_test,
				framebuffer_attachment_formats,
				ArrayCount(framebuffer_attachment_formats)
			);
		}
		inline View& GetView(Handle view_handle) {
			VORTEX_ASSERT(d_ViewChecks(view_handle))
			return m_DataMap.Get<View>(view_handle);
		}
		virtual void DestroyView(Handle view_handle) = 0;

	public: // DrawSurface
		virtual Handle CreateDrawSurface(
			const Math::RectangleInt& area,
			const Math::Color& clear_color,
			float clear_depth,
			Int32 clear_stencil
		) = 0;
		inline Handle CreateDrawSurface(const Math::RectangleInt& area) {
			return CreateDrawSurface(
				area,
				Math::Colors::Values[Math::Colors::VortexCustom],
				1.0f,
				0
			);
		}
		inline DrawSurface& GetDrawSurface(Handle draw_surface_handle) {
			VORTEX_ASSERT(d_DrawSurfaceChecks(draw_surface_handle))
			return m_DataMap.Get<DrawSurface>(draw_surface_handle);
		}
		inline const DrawSurface& GetDrawSurface(Handle draw_surface_handle) const {
			VORTEX_ASSERT(d_DrawSurfaceChecks(draw_surface_handle))
			return m_DataMap.Get<DrawSurface>(draw_surface_handle);
		}
		virtual void DestroyDrawSurface(Handle draw_surface_handle) = 0;

	public:
		virtual void NextFrame() = 0;

		virtual void OnEvent(const Event& event) = 0;

	protected:
		using SortingKeyType = UInt64;
		//	DrawKey layout:
		//	if ViewLayer is HUD or World:
		//		if Blending == Opaque:

		//			0------------28------------------44-------------46----------------------62-------------63
		//			|	Depth	||	Material Handle	||	Blending	||	Draw Surface Handle	||	ViewLayer	|
		using OpaquePacker = IntegerPacker<SortingKeyType, 28, 16, 2, 16, 2>;

		//		else if Blending != Opaque:

		//			0-------------------28----------44--------------46----------------------62-------------63
		//			|	Material Handle	||	Depth	||	Blending	||	Draw Surface Handle	||	ViewLayer	|
		using TranslucentPacker = IntegerPacker<SortingKeyType, 16, 28, 2, 16, 2>;

		//		else if ViewLayer == PostProcess

		//			0-----------------------16----------------------------------------------62-------------63
		//			|	PostProcess Handle	||												||	ViewLayer	|
		using PostProcessPacker = IntegerPacker<SortingKeyType, 16, 46, 2>;

		constexpr static ViewLayer::Enum GetSortingKeyViewLayer(SortingKeyType sorting_key) {
			return static_cast<ViewLayer::Enum>(OpaquePacker::Unpack<4>(sorting_key));
		}

		constexpr static SortingKeyType GenerateDrawKey(
			ViewLayer::Enum view_layer,
			Handle draw_surface_handle,
			Blending::Enum blending,
			Handle material_handle
		) {
			SortingKeyType sorting_key{0};
			VORTEX_ASSERT(view_layer != ViewLayer::PostProcess)

			if (blending == Blending::Opaque) {
				OpaquePacker::Pack<0>(sorting_key, 0); // depth
				OpaquePacker::Pack<1>(sorting_key, material_handle);
				OpaquePacker::Pack<2>(sorting_key, Blending::Opaque);
				OpaquePacker::Pack<3>(sorting_key, draw_surface_handle);
				OpaquePacker::Pack<4>(sorting_key, view_layer);
			} else {
				TranslucentPacker::Pack<0>(sorting_key, material_handle);
				TranslucentPacker::Pack<1>(sorting_key, 0); // depth
				TranslucentPacker::Pack<2>(sorting_key, blending);
				TranslucentPacker::Pack<3>(sorting_key, draw_surface_handle);
				TranslucentPacker::Pack<4>(sorting_key, view_layer);
			}

			return sorting_key;
		}
		constexpr static void SetDrawKeyDepth(SortingKeyType& sorting_key, UInt16 depth) {
			VORTEX_ASSERT(static_cast<ViewLayer::Enum>(OpaquePacker::Unpack<4>(sorting_key)) != ViewLayer::PostProcess)

			if (static_cast<Blending::Enum>(OpaquePacker::Unpack<2>(sorting_key)) == Blending::Opaque) {
				OpaquePacker::Pack<0>(sorting_key, static_cast<SortingKeyType>(depth));
			} else {
				TranslucentPacker::Pack<1>(sorting_key, static_cast<SortingKeyType>(depth));
			}
		}
		constexpr static UInt16 GetDrawKeyDepth(SortingKeyType sorting_key) {
			VORTEX_ASSERT(static_cast<ViewLayer::Enum>(OpaquePacker::Unpack<4>(sorting_key)) != ViewLayer::PostProcess)

			if (static_cast<Blending::Enum>(OpaquePacker::Unpack<2>(sorting_key)) == Blending::Opaque) {
				return static_cast<UInt16>(OpaquePacker::Unpack<0>(sorting_key));
			} else {
				return static_cast<UInt16>(TranslucentPacker::Unpack<1>(sorting_key));
			}
		}

		constexpr static SortingKeyType GeneratePostProcessKey(Handle postprocess_handle) {
			SortingKeyType sorting_key{0};

			PostProcessPacker::Pack<0>(sorting_key, postprocess_handle);
			PostProcessPacker::Pack<1>(sorting_key, 0); // unused
			PostProcessPacker::Pack<2>(sorting_key, ViewLayer::PostProcess);

			return sorting_key;
		}

		constexpr static void GetDrawKeyData(
			SortingKeyType sorting_key,
			Handle& draw_surface_handle,
			Blending::Enum& blending,
			Handle& material_handle) {

			blending = static_cast<Blending::Enum>(OpaquePacker::Unpack<2>(sorting_key));
			draw_surface_handle = static_cast<Blending::Enum>(OpaquePacker::Unpack<3>(sorting_key));

			VORTEX_ASSERT(static_cast<ViewLayer::Enum>(OpaquePacker::Unpack<4>(sorting_key)) != ViewLayer::PostProcess)

			if (blending == Blending::Opaque) {
				material_handle = static_cast<Handle>(OpaquePacker::Unpack<1>(sorting_key));
			} else {
				material_handle = static_cast<Handle>(TranslucentPacker::Unpack<0>(sorting_key));
			}
		}

		constexpr static void GetPostProcessKeyData(
			SortingKeyType sorting_key,
			Handle& post_process_handle) {
			post_process_handle = static_cast<Handle>(PostProcessPacker::Unpack<0>(sorting_key));

			VORTEX_ASSERT(static_cast<ViewLayer::Enum>(PostProcessPacker::Unpack<2>(sorting_key)) == ViewLayer::PostProcess)
		}

		struct DrawCommand {
			SortingKeyType Key;
			Math::Matrix4 TransformMatrix;
			Handle MeshHandle;

			//Windows created with default view which is wrapper for backbuffer.
			//default view has orthographic projection with identity ViewMatrix.

			//Can be View Handle Or Window Handle
			Handle DrawHandle;

#ifdef VORTEX_DEBUG
			ViewLayer::Enum d_ViewLayer;
			Handle d_DrawSurfaceHandle;
			Blending::Enum d_Blending;
			Handle d_MaterialHandle;
			UInt32 d_Depth;
#endif
		};

		struct ComputeCommand {
			Handle ComputeShaderHandle;
			Math::Vector3Int GroupCount;
		};

	public:
		inline void SubmitDraw(
			Handle view_or_window_handle,
			ViewLayer::Enum view_layer,
			Handle draw_surface_handle,
			Handle material_handle,
			const Math::Matrix4& transform,
			Handle mesh_handle
		) {
			VORTEX_ASSERT(d_MaterialChecks(material_handle))
			VORTEX_ASSERT(d_DrawSurfaceChecks(draw_surface_handle))
			VORTEX_ASSERT(d_MeshChecks(mesh_handle))

			auto blending = m_DataMap.Get<Material>(material_handle).Blending;

			DrawCommand command{};
			command.Key = GenerateDrawKey(
				view_layer,
				draw_surface_handle,
				blending,
				material_handle
			);
			command.TransformMatrix = transform;
			command.MeshHandle = mesh_handle;
			command.DrawHandle = view_or_window_handle;

#ifdef VORTEX_DEBUG
			command.d_ViewLayer = view_layer;
			command.d_DrawSurfaceHandle = draw_surface_handle;
			command.d_Blending = blending;
			command.d_MaterialHandle = material_handle;
			command.d_Depth = 0;
#endif
			m_DrawCommands.push_back(command);
		}

		inline void SubmitCompute(Handle compute_shader_handle, const Math::Vector3Int& num_of_groups) {
			VORTEX_ASSERT(d_ComputeShaderChecks(compute_shader_handle))

			ComputeCommand command{};
			command.ComputeShaderHandle = compute_shader_handle;
			command.GroupCount = num_of_groups;
			m_ComputeCommands.push_back(command);
		}

	protected:
		std::vector<DrawCommand> m_DrawCommands;
		std::vector<ComputeCommand> m_ComputeCommands;
		Map m_DataMap;

#ifdef VORTEX_DEBUG
	public:
		inline bool d_DrawCommandChecks() const {
			bool out{true};
			for (const auto& cmd : m_DrawCommands) {
				Blending::Enum blending;
				Handle material_handle;
				Handle draw_surface_handle;
				UInt16 depth{GetDrawKeyDepth(cmd.Key)};
				auto view_layer = GetSortingKeyViewLayer(cmd.Key);

				GetDrawKeyData(
					cmd.Key,
					draw_surface_handle,
					blending,
					material_handle
				);
				VORTEX_ASSERT(out &= m_DataMap.Contains(cmd.DrawHandle))
				VORTEX_ASSERT(out &= m_DataMap.Contains(material_handle))
				VORTEX_ASSERT(out &= m_DataMap.Contains(draw_surface_handle))
				VORTEX_ASSERT(out &= m_DataMap.Contains(cmd.MeshHandle))

				VORTEX_ASSERT(out &= view_layer == cmd.d_ViewLayer)
				VORTEX_ASSERT(out &= blending == cmd.d_Blending)
				VORTEX_ASSERT(out &= material_handle == cmd.d_MaterialHandle)
				VORTEX_ASSERT(out &= draw_surface_handle == cmd.d_DrawSurfaceHandle)
				VORTEX_ASSERT(out &= depth == cmd.d_Depth)
			}
			return out;
		}
		inline bool d_WindowChecks(Handle window_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<Window>(window_handle))
			bool out{true};
			const auto& window = m_DataMap.Get<Window>(window_handle);
			VORTEX_ASSERT(out &= m_DataMap.Is<View>(window.DefaultViewHandle))

			VORTEX_ASSERT(out &= window.Resolution.Width > 0)
			VORTEX_ASSERT(out &= window.Resolution.Height > 0)
			return out;
		}

		inline bool d_TextureChecks(Handle texture_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<Texture>(texture_handle))
			bool out{true};
			const auto& texture = m_DataMap.Get<Texture>(texture_handle);

			VORTEX_ASSERT(out &= texture.PixelFormat < PixelFormat::Count)
			VORTEX_ASSERT(out &= texture.MinificationFilter < TextureLODFilter::Count)
			VORTEX_ASSERT(out &= texture.MagnificationFilter < TextureLODFilter::Count)

			VORTEX_ASSERT(out &= texture.WrapS <= TextureWrap::Count)
			VORTEX_ASSERT(out &= texture.WrapT <= TextureWrap::Count)
			VORTEX_ASSERT(out &= texture.WrapR <= TextureWrap::Count)
			for (SizeType i = 0; i < texture.Size.Length(); ++i) {
				VORTEX_ASSERT(out &= texture.Size[i] > 0)
			}
			VORTEX_ASSERT(out &= texture.LayerCount > 0)
			VORTEX_ASSERT(out &= texture.BitsPerPixel > 0)
			VORTEX_ASSERT(out &= texture.DataSize > 0)
			return out;
		}
		inline bool d_BufferChecks(Handle buffer_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<Buffer>(buffer_handle))
			bool out{true};
			const auto& buffer = m_DataMap.Get<Buffer>(buffer_handle);
			VORTEX_ASSERT(out &= buffer.Size > 0)
			return out;
		}
		inline bool d_ShaderChecks(Handle shader_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<Shader>(shader_handle))
			bool out{true};
			return out;
		}
		inline bool d_FrameBufferChecks(Handle framebuffer_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<FrameBuffer>(framebuffer_handle));
			bool out{true};
			const auto& framebuffer = m_DataMap.Get<FrameBuffer>(framebuffer_handle);
			for (SizeType i = 0; i < framebuffer.Size.Length(); ++i) {
				VORTEX_ASSERT(out &= framebuffer.Size[i] > 0)
			}

			for (const auto& texture_handle : framebuffer.TextureHandles) {
				VORTEX_ASSERT(d_TextureChecks(texture_handle))
			}
			return out;
		}
		inline bool d_ComputeShaderChecks(Handle compute_shader_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<ComputeShader>(compute_shader_handle))
			bool out{true};
			const auto& compute_shader = m_DataMap.Get<ComputeShader>(compute_shader_handle);
			VORTEX_ASSERT(d_ShaderChecks(compute_shader.ShaderHandle))
			VORTEX_ASSERT(out &= compute_shader.OnBind != nullptr)
			return out;
		}
		inline bool d_MeshChecks(Handle mesh_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<Mesh>(mesh_handle))
			bool out{true};
			const auto& mesh = m_DataMap.Get<Mesh>(mesh_handle);
			VORTEX_ASSERT(out &= d_BufferChecks(mesh.IndexBufferHandle))

			for (const auto& buffer_handle : mesh.BufferHandles) {
				VORTEX_ASSERT(d_BufferChecks(buffer_handle))
			}

			VORTEX_ASSERT(out &= mesh.IndexCapacity > 0)
			VORTEX_ASSERT(out &= mesh.Topology < Topology::Count)
			return out;
		}
		inline bool d_MaterialChecks(Handle material_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<Material>(material_handle))
			bool out{true};
			const auto& material = m_DataMap.Get<Material>(material_handle);

			VORTEX_ASSERT(out &= d_ShaderChecks(material.ShaderHandle))
			VORTEX_ASSERT(out &= material.Blending < Blending::Count)
			return out;
		}
		inline bool d_DrawSurfaceChecks(Handle draw_surface_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<DrawSurface>(draw_surface_handle))
			bool out{true};
			return out;
		}
		inline bool d_ViewChecks(Handle view_handle) const {
			VORTEX_ASSERT(m_DataMap.Is<View>(view_handle))
			bool out{true};
			const auto& view = m_DataMap.Get<View>(view_handle);
			VORTEX_ASSERT(out &= d_FrameBufferChecks(view.FramebufferHandle))
			VORTEX_ASSERT(out &= view.DepthTest < DepthTesting::Count)
			return out;
		}
#endif
	};
}