#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Containers/Vector.h"
#include "Vortex/Containers/Queue.h"
#include "Vortex/Common/Handle.h"

#include "Vortex/Graphics/RenderBackend.h"

#include <algorithm>
#include <unordered_map>

namespace Vortex::Graphics {
	VORTEX_HANDLE(MeshHandle);
	VORTEX_HANDLE(MaterialHandle);
	VORTEX_HANDLE(ViewHandle);
}

VORTEX_HANDLE_HASH(Vortex::Graphics::MeshHandle);
VORTEX_HANDLE_HASH(Vortex::Graphics::MaterialHandle);
VORTEX_HANDLE_HASH(Vortex::Graphics::ViewHandle);

namespace Vortex::Graphics {
	struct MeshData {
		VertexArrayHandle VertexArray;
		BufferHandle IndexBuffer;

		BufferHandle PositionBuffer;
		BufferHandle ColorBuffer;
		BufferHandle UV0Buffer;
		BufferHandle UV1Buffer;

		SizeType IndexCount;
	};

	namespace Translucency {
		enum Enum {
			Opaque = 0,
			Additive,
			Subtractive,
		};
	}

	struct MaterialData {
		ProgramHandle Program;
		TextureHandle Texture0;
		TextureHandle Texture1;
	};

	struct ViewData {
		Int32 Viewport[4];
		float ClearColor[4];
		float ProjectionMatrix[16];
		float ViewMatrix[16];
	};

	using DrawCallType = UInt64;
	class Renderer;

	struct DrawCommand {
		friend class Renderer;

		DrawCommand(const float* transform, MeshHandle mesh_handle, Translucency::Enum translucency)
			: m_DrawCall{0},
			  m_TransformMatrix{},
			  m_MeshHandle{mesh_handle} {
			m_DrawCall |= static_cast<DrawCallType>(translucency) << 56;
			std::memcpy(m_TransformMatrix, transform, 16 * sizeof(float));
		}
		//	if translucency opaque
		//  63--------------58------------------56------------------24--------------0
		//  |	View		||	Translucency	||	Depth			||	Material	|

		//	if translucency additive or subtractive
		//  63--------------58------------------56--------------32------------------0
		//  |	View		||	Translucency	||	Material	||	Depth			|

		inline void SetView(ViewHandle view_handle) {
			m_DrawCall |= static_cast<DrawCallType>(view_handle.id) << 58;
		}
		inline void SetMaterial(MaterialHandle material_handle) {
			if (GetTranslucency() == Translucency::Opaque) {
				m_DrawCall |= static_cast<DrawCallType>(material_handle.id);
			} else {
				m_DrawCall |= static_cast<DrawCallType>(material_handle.id) << 32;
			}
		}
		inline void SetDepth(UInt32 depth) {
			if (GetTranslucency() == Translucency::Opaque) {
				m_DrawCall |= static_cast<DrawCallType>(depth) << 24;
			} else {
				m_DrawCall |= static_cast<DrawCallType>(depth);
			}
		}

		inline Translucency::Enum GetTranslucency() const {
			return static_cast<Translucency::Enum>((m_DrawCall >> 56) & 0xF); //wrong
		}
		inline ViewHandle GetView() const {
			HandleType id = (m_DrawCall >> 58) & (0xFF);
			return ViewHandle{static_cast<HandleType>(id)};
		}
		inline MaterialHandle GetMaterial() const {
			HandleType id;
			if (GetTranslucency() == Translucency::Opaque) {
				id = (m_DrawCall) & (0xFFFFFF);
			} else {
				id = (m_DrawCall >> 32) & (0xFFFFFF);
			}
			return MaterialHandle{static_cast<HandleType>(id)};
		}
		inline UInt32 GetDepth() const {
			if (GetTranslucency() == Translucency::Opaque) {
				return (m_DrawCall >> 24) & (0xFFFFFFFFF);
			} else {
				return (m_DrawCall) & (0xFFFFFFFFF);
			}
		}
		inline MeshHandle GetMesh() const { return m_MeshHandle; }
		inline const float* GetTransform() const { return m_TransformMatrix; }

	protected:
		DrawCallType m_DrawCall{0};
		float m_TransformMatrix[16];
		MeshHandle m_MeshHandle;
	};

	template<typename T>
	struct DrawCommandGenerator {};

	class Renderer {
	public:
		explicit Renderer(RenderBackend* render_backend, const Int32* resolution);
		~Renderer();

	public: // Mesh
		MeshHandle CreateMesh(Topology::Enum topology, SizeType vertex_count, SizeType index_count);
		void Destroy(MeshHandle handle);

		void SetIndices(MeshHandle handle, const UInt32* data, SizeType count);
		void SetPositions(MeshHandle handle, const float* data, SizeType count);
		void SetColors(MeshHandle handle, const float* data, SizeType count);
		void SetUV0(MeshHandle handle, const float* data, SizeType count);
		void SetUV1(MeshHandle handle, const float* data, SizeType count);

		const MeshData& operator[](MeshHandle handle) const { return m_MeshDatas.at(handle); }
		inline bool IsValid(MeshHandle handle) const { return handle.id != InvalidID && m_MeshDatas.find(handle) != m_MeshDatas.end(); };

	public: // Material
		MaterialHandle CreateMaterial(ProgramHandle shader, TextureHandle texture0);
		void Destroy(MaterialHandle handle);

		MaterialData& operator[](MaterialHandle handle) { return m_MaterialDatas.at(handle); }
		const MaterialData& operator[](MaterialHandle handle) const { return m_MaterialDatas.at(handle); }

		inline bool IsValid(MaterialHandle handle) const {
			return handle.id != InvalidID && m_MaterialDatas.find(handle) != m_MaterialDatas.end();
		};
	public: // View
		ViewHandle CreateView(const Int32* dimensions, const float* projection_matrix, const float* view_matrix);
		void Destroy(ViewHandle handle);

		ViewData& operator[](ViewHandle handle) { return m_ViewDatas.at(handle); }
		const ViewData& operator[](ViewHandle handle) const { return m_ViewDatas.at(handle); }

		inline bool IsValid(ViewHandle handle) const { return handle.id != InvalidID && m_ViewDatas.find(handle) != m_ViewDatas.end(); };

	protected:
		RenderBackend* m_RenderBackend;

		std::unordered_map<MeshHandle, MeshData> m_MeshDatas;
		std::unordered_map<MaterialHandle, MaterialData> m_MaterialDatas;
		std::unordered_map<ViewHandle, ViewData> m_ViewDatas;

		HandleType m_NextMeshHandle;
		HandleType m_NextMaterialHandle;
		HandleType m_NextViewHandle;

	public:
		inline void Submit(DrawCommand draw_command) {
			m_DrawCommands.EmplaceBack(draw_command);
		}

		template<typename T>
		inline void Render(const T& obj) {
			DrawCommandGenerator<T> generator;
			generator(this, obj);
		}

		template<typename T>
		inline void Render(ViewHandle view, const float* transform, const T& obj) {
			DrawCommandGenerator<T> generator;
			generator(this, view, transform, obj);
		}

		void Process();

	private:
		void CreateStandardAssets(const Int32* resolution);

	public:
		ProgramHandle StandardProgram;
		TextureHandle StandardWhiteTexture;
		TextureHandle StandardErrorTexture;

		ViewHandle DefaultView;

	protected:
		Vector<DrawCommand> m_DrawCommands;
	};
}