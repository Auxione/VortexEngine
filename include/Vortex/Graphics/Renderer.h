#pragma once
#include <unordered_map>
#include <vector>
#include <set>

#include "Vortex/Graphics/GraphicsEnum.h"
#include "Vortex/Graphics/BufferLayout.h"
#include "Vortex/Graphics/RenderBackend.h"
#include "Vortex/Graphics/AbstractData.h"

#include "Vortex/Core/Event.h"

#include "Vortex/Math/VortexMath.h"

#include "Vortex/Common/HandleMap.h"
#include "Vortex/Common/HashedString.h"
#include "Vortex/Common/IntegerPacker.h"

namespace Vortex::Graphics {
	struct View;
	struct Material;
	struct ComputeShader;
	struct PostProcess;
	struct Mesh;

	using RendererHandle = BasicHandle<UInt32, 10, 4, 18>;

	using Handle = RendererHandle::UnderlyingType;
	using RendererDataMap = RendererHandle::Map<
		View,
		Material,
		ComputeShader,
		PostProcess,
		Mesh
	>;

	template<typename T>
	class RenderObjectBinder {
	public:
		RenderObjectBinder(RenderBackend* render_backend, const T& object)
			: m_RenderBackend{render_backend}, m_Object{object} {}
		~RenderObjectBinder() = default;

	public:
		inline void Set(HashedString name, const void* data, SizeType count = 1) const {
			VORTEX_ASSERT(count != 0)
			m_RenderBackend->SetUniform(m_Object.Shader, name, data, count);
		}
		template<typename U>
		inline const U& Layout() const { return m_Object.Data.template Layout<U>(); }

	protected:
		RenderBackend* m_RenderBackend;
		const T& m_Object;
	};

	struct PostProcess;
	using PostProcessBinder = RenderObjectBinder<PostProcess>;
	using OnPostProcessingBindFn = void (*)(const PostProcessBinder&);

	struct PostProcess {
		Handle Shader;
		OnPostProcessingBindFn OnBind;
		AbstractData<256> Data;
	};

	struct ComputeShader;
	using ComputeShaderBinder = RenderObjectBinder<ComputeShader>;
	using OnComputeShaderBindFn = void (*)(const ComputeShaderBinder&);

	struct ComputeShader {
		Handle Shader;
		OnComputeShaderBindFn OnBind;
		AbstractData<256> Data;
	};

	struct Mesh {
		Handle VertexArray;
		Handle IndexBuffer;

		Handle Buffers[MeshAttribute::Count];

		SizeType IndexCount;
		SizeType IndexCapacity;
		SizeType VertexCapacity;
	};

	struct Material;
	using MaterialBinder = RenderObjectBinder<Material>;
	using OnMaterialBindFn = void (*)(const MaterialBinder&);

	struct Material {
		Handle Shader;
		OnMaterialBindFn OnBind;
		Blending::Enum Blending;
		AbstractData<256> Data;
	};

	struct View {
		RectangleInt Viewport;
		Color ClearColor;
		Matrix4 ProjectionMatrix;
		Matrix4 ViewMatrix;
		Handle FrameBuffer;
		DepthTesting::Enum DepthTest;
	};

	namespace ShaderConstants {
		enum Enum {
			Matrix4_Transform = 0,
			Matrix4_View,
			Matrix4_Projection,

			Float2_Resolution,

			Count
		};
		constexpr HashedString ToHashedString[]{
			"Matrix4_Transform"
			, "Matrix4_View"
			, "Matrix4_Projection"

			, "Float2_Resolution"
		};
		constexpr const char* ToString[]{
			"Matrix4_Transform"
			, "Matrix4_View"
			, "Matrix4_Projection"

			, "Float2_Resolution"
		};
		constexpr ElementType::Enum Type[]{
			ElementType::Matrix4
			, ElementType::Matrix4
			, ElementType::Matrix4

			, ElementType::Float2
		};
	}

	namespace DrawCall {
		using Type = UInt64;

		//	if ViewLayer is  HUD or World:
		//		if translucency opaque
		//			0-----------28------------------44--------------46--------------48-------------63
		//			0-----------32------------------56--------------58--------------60-------------63
		//			|	Depth	||	Material Handle	||	Blending	||	ViewLayer	||	View Handle	|
		//  		|	0		||			1		||		2		||		3		||		4		|
		using OpaquePacker = IntegerPacker<Type, 32, 24, 2, 2, 4>;

		//		else if translucency additive or subtractive
		//  		0-------------------24----------56--------------58--------------60-------------63
		//  		|	Material Handle	||	Depth	||	Blending	||	ViewLayer	||	View Handle	|
		//  		|		0			||		1	||		2		||		3		||		4		|
		using TranslucentPacker = IntegerPacker<Type, 24, 32, 2, 2, 4>;

		//	else if ViewLayer PostProcess
		//  		0-----------------------------------------------58--------------60-------------63
		//  		|	PostProcess Handle							||	ViewLayer	||	View Handle	|
		//  		|		0			 							||		1		||		2		|
		using PostProcessPacker = IntegerPacker<Type, 58, 2, 4>;

		inline DrawCall::Type EncodeDrawCall(
			Handle view_handle,
			ViewLayer::Enum view_layer,
			Blending::Enum blending,
			Handle material_handle,
			UInt32 depth
		) {

			DrawCall::Type draw_call{0};

			if (blending == Blending::Opaque) {
				OpaquePacker::Pack<0>(draw_call, depth);
				OpaquePacker::Pack<1>(draw_call, material_handle);
				OpaquePacker::Pack<2>(draw_call, Blending::Opaque);
				OpaquePacker::Pack<3>(draw_call, view_layer);
				OpaquePacker::Pack<4>(draw_call, view_handle);
			} else {
				TranslucentPacker::Pack<0>(draw_call, material_handle);
				TranslucentPacker::Pack<1>(draw_call, depth);
				TranslucentPacker::Pack<2>(draw_call, blending);
				TranslucentPacker::Pack<3>(draw_call, view_layer);
				TranslucentPacker::Pack<4>(draw_call, view_handle);
			}

			return draw_call;
		}

		inline DrawCall::Type EncodePostprocessCall(
			Handle view_handle,
			Handle postproccess_handle
		) {
			DrawCall::Type draw_call{0};
			PostProcessPacker::Pack<0>(draw_call, postproccess_handle);
			PostProcessPacker::Pack<1>(draw_call, ViewLayer::PostProcess);
			PostProcessPacker::Pack<2>(draw_call, view_handle);

			return draw_call;
		}

		inline Handle GetViewHandle(DrawCall::Type draw_call) {
			return static_cast<Handle>(OpaquePacker::Unpack<4>(draw_call));
		}
		inline ViewLayer::Enum GetViewLayer(DrawCall::Type draw_call) {
			return static_cast<ViewLayer::Enum>(OpaquePacker::Unpack<3>(draw_call));
		}

		inline void DecodeDrawCall(
			DrawCall::Type draw_call,
			Blending::Enum& blending,
			Handle& material_handle,
			UInt32& depth) {

			blending = static_cast<Blending::Enum>(OpaquePacker::Unpack<2>(draw_call));
			if (blending == Blending::Opaque) {
				depth = static_cast<UInt32>(OpaquePacker::Unpack<0>(draw_call));
				material_handle = static_cast<Handle>(OpaquePacker::Unpack<1>(draw_call));
			} else {
				material_handle = static_cast<Handle>(TranslucentPacker::Unpack<0>(draw_call));
				depth = static_cast<UInt32>(TranslucentPacker::Unpack<1>(draw_call));
			}
		}
		inline void SetDepth(DrawCall::Type& draw_call, UInt32 depth) {
			auto blending = static_cast<Blending::Enum>(OpaquePacker::Unpack<2>(draw_call));
			if (blending == Blending::Opaque) {
				OpaquePacker::Pack<0>(draw_call, static_cast<DrawCall::Type>(depth));
			} else {
				TranslucentPacker::Pack<1>(draw_call, static_cast<DrawCall::Type>(depth));
			}
		}

		inline void DecodePostProcessCall(
			DrawCall::Type draw_call,
			Handle& postproccess_handle) {
			//postproccess_handle = (draw_call >> ViewHandleBitStart) & ViewHandleMaxValue;
		}
	}

	class Renderer final {
	public:
		Renderer(RenderBackend* render_backend, const Int32* resolution);
		~Renderer();

	public:    // Buffer
		inline Handle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) {
			return m_RenderBackend->CreateBuffer(buffer_usage, buffer_layout, count, data);
		};
		inline void UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) {
			m_RenderBackend->UpdateBuffer(buffer_handle, offset, data_size, data);
		};
		inline void GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) {
			m_RenderBackend->GetBuffer(buffer_handle, offset, data_size, data);
		};
		inline void DestroyBuffer(Handle buffer_handle) {
			m_RenderBackend->DestroyBuffer(buffer_handle);
		};

	public:    // Shader
		inline Handle CreateShader(const char** sources, ShaderType::Enum* types, SizeType count) {
			return m_RenderBackend->CreateShader(sources, types, count);
		}
		inline void DestroyShader(Handle shader_handle) {
			return m_RenderBackend->DestroyShader(shader_handle);
		}

	public:    // Texture
		inline Handle CreateTexture2D(
			const UInt16* size,
			PixelFormat::Enum format,
			const void* pixels,
			TextureLODFilter::Enum min_filter,
			TextureLODFilter::Enum mag_filter,
			TextureWrap::Enum wrap_s,
			TextureWrap::Enum wrap_t
		) {
			return m_RenderBackend->CreateTexture2D(
				size,
				format,
				pixels,
				min_filter,
				mag_filter,
				wrap_s,
				wrap_t,
				true
			);
		}
		inline Handle CreateTexture2D(
			const UInt16* size,
			PixelFormat::Enum format,
			const void* pixels
		) {
			return m_RenderBackend->CreateTexture2D(
				size,
				format,
				pixels,
				TextureLODFilter::Linear,
				TextureLODFilter::Linear,
				TextureWrap::ClampBorder,
				TextureWrap::ClampBorder,
				true
			);
		}
		inline void UpdateTexture2D(Handle texture_handle, const void* pixels) {
			m_RenderBackend->UpdateTexture2D(
				texture_handle,
				pixels,
				true
			);
		}
		inline void GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) {
			m_RenderBackend->GetTexture(
				texture_handle,
				buffer_size,
				pixels
			);
		}
		inline void DestroyTexture(Handle texture_handle) {
			m_RenderBackend->DestroyTexture(texture_handle);
		}

	public: // ComputeShader
		Handle CreateComputeShader(const char* source, OnComputeShaderBindFn on_compute_shader_bind);
		void DestroyComputeShader(Handle handle);

	public: // Mesh
		inline Handle CreateDynamic2DMesh(Topology::Enum topology, SizeType vertex_count, SizeType index_count) {
			MeshAttribute::Enum attribs[]{
				MeshAttribute::Position
				, MeshAttribute::Color
				, MeshAttribute::UV
			};
			return CreateMesh(
				topology,
				BufferUsage::DynamicDraw,
				attribs,
				Vortex::ArrayCount(attribs),
				vertex_count,
				index_count
			);
		}

		Handle CreateMesh(
			Topology::Enum topology,
			BufferUsage::Enum usage,
			const MeshAttribute::Enum* attributes,
			SizeType attribute_count,
			SizeType vertex_count,
			SizeType index_count
		);

		void SetMeshIndexCount(Handle mesh_handle, SizeType count);
		void SetMeshIndices(Handle mesh_handle, const UInt32* data, SizeType count);
		void SetMeshAttribute(Handle mesh_handle, MeshAttribute::Enum attribute, const float* data, SizeType count);
		void DestroyMesh(Handle mesh_handle);

	public: // Material
		Handle CreateMaterial(Handle shader_handle, Vortex::Graphics::Blending::Enum blending, OnMaterialBindFn on_material_bind);
		void DestroyMaterial(Handle material_handle);

	public: // View
		Handle CreateView(const RectangleInt& viewport, const Matrix4& projection_matrix, const Matrix4& view_matrix);
		Handle CreateBufferedView(const RectangleInt& viewport, const Matrix4& projection_matrix, const Matrix4& view_matrix);
		void DestroyView(Handle view_handle);

	public:
		void OnEvent(const Event& event);

		inline void Submit(
			Handle view_handle,
			ViewLayer::Enum layer,
			Handle material_handle,
			const Matrix4& transform,
			Handle mesh
		) {
			auto blending = m_Datas.Get<Material>(material_handle).Blending;
			DrawCommand command{
				DrawCall::EncodeDrawCall(
					view_handle,
					layer,
					blending,
					material_handle,
					0
				)
				, transform
				, mesh
			};
#ifdef VORTEX_DEBUG
			command.d_ViewHandle = view_handle;
			command.d_ViewLayer = layer;
			command.d_Blending = blending;
			command.d_MaterialHandle = material_handle;
			command.d_Depth = 0;
#endif
			m_DrawCommands.push_back(command);
		}

		inline void Submit(
			Handle compute_shader_handle,
			const Vector3Int& groups
		) {
			VORTEX_ASSERT(groups.x() > 0)
			VORTEX_ASSERT(groups.y() > 0)
			VORTEX_ASSERT(groups.z() > 0)
			ComputeCommand command{compute_shader_handle, groups};
			m_ComputeCommands.push_back(command);
		}

	private:
		void ProcessComputeShaderCommands();
		void SortDrawCommands();
		void ProcessDrawCommands();

		void GenerateDefaultView(const Int32* resolution);

	public:
		inline void Process() {
			ProcessComputeShaderCommands();

			ProcessDrawCommands();
		}

	public:
		//in ms
		float GetRenderTime() const { return m_RenderTime; }
		float GetComputeShaderTime() const { return m_ComputeShaderTime; }

		inline Material& GetMaterial(Handle handle) {
			VORTEX_ASSERT(m_Datas.Is<Material>(handle))
			VORTEX_ASSERT(m_Datas.Contains(handle))
			return m_Datas.Get<Material>(handle);
		};
		inline View& GetView(Handle handle) {
			VORTEX_ASSERT(m_Datas.Is<View>(handle))
			VORTEX_ASSERT(m_Datas.Contains(handle))
			return m_Datas.Get<View>(handle);
		};
		inline ComputeShader& GetComputeShader(Handle handle) {
			VORTEX_ASSERT(m_Datas.Is<ComputeShader>(handle))
			VORTEX_ASSERT(m_Datas.Contains(handle))
			return m_Datas.Get<ComputeShader>(handle);
		};

		template<typename T>
		inline T& Get(Handle handle) {
			if constexpr(std::is_same_v<T, Material>) {
				return GetMaterial(handle);
			} else if constexpr(std::is_same_v<T, View>) {
				return GetView(handle);
			} else if constexpr(std::is_same_v<T, ComputeShader>) {
				return GetComputeShader(handle);
			}
		};

		template<typename T>
		inline const T& Get(Handle handle) const {
			if constexpr(std::is_same_v<T, ComputeShader>
				|| std::is_same_v<T, Mesh>
				|| std::is_same_v<T, Material>
				|| std::is_same_v<T, View>
				) {
				VORTEX_ASSERT(m_Datas.Is<T>(handle))
				VORTEX_ASSERT(m_Datas.Contains(handle))
				return m_Datas.Get<T>(handle);
			} else {
				return m_RenderBackend->Get<T>(handle);
			}
		};

		inline bool Contains(Handle handle) const {
			bool out{false};
			out |= m_Datas.Contains(handle);
			out |= m_RenderBackend->Contains(handle);
			return out;
		};

	private:
		struct DrawCommand {
			DrawCall::Type DrawCall;
			Matrix4 TransformMatrix;
			Handle MeshHandle;

#ifdef VORTEX_DEBUG
			Handle d_ViewHandle;
			ViewLayer::Enum d_ViewLayer;
			Blending::Enum d_Blending;
			Handle d_MaterialHandle;
			UInt32 d_Depth;
#endif
		};

		struct ComputeCommand {
			Handle ComputeShader;
			Vector3Int GroupCount;
		};

	private:
		RenderBackend* m_RenderBackend;
		RendererDataMap m_Datas;

		Handle m_RenderTimerQuery;
		float m_RenderTime;

		Handle m_ComputeShaderTimerQuery;
		float m_ComputeShaderTime;

		std::vector<DrawCommand> m_DrawCommands;
		std::vector<ComputeCommand> m_ComputeCommands;

	public:
		Handle DefaultView;

#ifdef VORTEX_DEBUG
	public:
		void d_CheckDrawCommands();
		void d_CheckComputeCommands();
#endif
	};
}