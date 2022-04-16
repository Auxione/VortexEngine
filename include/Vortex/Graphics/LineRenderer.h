#pragma once
#include <vector>
#include <unordered_map>

#include "Vortex/Graphics/Renderer.h"
#include "Vortex/Math/VortexMath.h"

namespace Vortex::Graphics {
	class LineRenderer {
		struct LineMesh {
			Vortex::Graphics::Handle MeshHandle;
			SizeType Capacity;
		};

		constexpr static SizeType VertexPerLine = 2;
		constexpr static SizeType IndexPerLine = 2;

	public:
		explicit LineRenderer(Renderer* renderer);
		virtual ~LineRenderer();

	public:
		Handle CreateLineMesh(SizeType capacity);
		inline void Destroy(Handle line_mesh_handle) {
			VORTEX_ASSERT(IsValid(line_mesh_handle))
			const auto& line_mesh = m_LineMeshes[line_mesh_handle];
			m_Renderer->DestroyMesh(line_mesh.MeshHandle);
			m_LineMeshes.erase(line_mesh_handle);
		}
		inline bool IsValid(Handle line_mesh_handle) const {
			return m_LineMeshes.find(line_mesh_handle) != m_LineMeshes.end();
		};

	public:
		void Begin();
		void Insert(const Math::Vector3& position1, const Math::Vector3& position2, const Math::Color& color);
		void End(Handle line_mesh_handle);

	private:
		Vortex::SizeType m_CurrentVertexIndex;

	public: // ========= 2D =========
		void BakeCircle(
			Handle line_mesh_handle,
			float radius,
			const Math::Color& color,
			SizeType number_of_segments = 18
		);
		void BakeCircularSector(
			Handle line_mesh_handle,
			float radius,
			Math::Angle angle,
			const Math::Color& color,
			SizeType number_of_segments = 18
		);
		void BakeCircularSectorCentered(
			Handle line_mesh_handle,
			float radius,
			Math::Angle angle,
			const Math::Color& color,
			SizeType number_of_segments = 18
		);
		void BakeRectangle(
			Handle line_mesh_handle,
			const Math::Vector2& size,
			const Math::Color& color
		);
		void BakeRectangleCentered(
			Handle line_mesh_handle,
			const Math::Vector2& size,
			const Math::Color& color
		);
		void Bake2DGrid(
			Handle line_mesh_handle,
			const Math::Vector2Int& grid_size,
			const Math::Vector2& cell_size,
			const Math::Color& color
		);

	public: // ========= 3D =========
		void BakeDimensionArrows(
			Handle line_mesh_handle,
			float size,
			const Math::Color& forward_color = Math::Colors::Values[Math::Colors::Blue],
			const Math::Color& up_color = Math::Colors::Values[Math::Colors::Green],
			const Math::Color& right_color = Math::Colors::Values[Math::Colors::Red],
			bool bake_labels = true,
			float label_width = 6.0f,
			float label_height = 8.0f
		);
		void BakeCube(
			Handle line_mesh_handle,
			Math::Vector3 size,
			const Math::Color& color
		);

	public:
		static Graphics::Handle GetDefaultMaterial() { return s_Material; };

	private:
		static Graphics::Handle s_ShaderHandle;
		static Graphics::Handle s_Material;
		static SizeType s_InstanceCount;

	private:
		std::vector<UInt32> m_Indices;
		std::vector<float> m_PositionBuffer;
		std::vector<float> m_ColorBuffer;

		SizeType m_TotalLineCapacity;
		Renderer* m_Renderer;

		std::unordered_map<Handle, LineMesh> m_LineMeshes;
	};
}