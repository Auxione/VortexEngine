#include "Vortex/Graphics/LineRenderer.h"
#include "Vortex/Common/Console.h"

namespace Vortex::Graphics {
	Vortex::Graphics::Handle LineRenderer::s_ShaderHandle{Vortex::Graphics::Map::NullHandle};
	Vortex::Graphics::Handle LineRenderer::s_Material{Vortex::Graphics::Map::NullHandle};
	Vortex::SizeType LineRenderer::s_InstanceCount{0};

	LineRenderer::LineRenderer(
		Renderer* renderer
	)
		: m_Indices{},
		  m_PositionBuffer{},
		  m_ColorBuffer{},

		  m_TotalLineCapacity{0},
		  m_Renderer{renderer},

		  m_LineMeshes{},

		  m_CurrentVertexIndex{0} {

		if (s_InstanceCount == 0) {
			const char* sources[]{
				R"(
#version 450 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

out vec4 vertex_color;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Transform;

void main(){
	vertex_color = a_color;
	gl_Position = Projection * View * Transform * vec4(a_position, 1.0f);
}
				)", R"(
#version 450 core
layout(location = 0) out vec4 color;

in vec4 vertex_color;

void main() {
	color = vertex_color;
}
			)"
			};
			Vortex::Graphics::ShaderType::Enum types[]{
				Vortex::Graphics::ShaderType::Vertex
				, Vortex::Graphics::ShaderType::Fragment
			};
			s_ShaderHandle = m_Renderer->CreateShader(sources, types, Vortex::ArrayCount(types), ShaderTags::Lit);
			s_Material = m_Renderer->CreateMaterial(
				s_ShaderHandle,
				Vortex::Graphics::Blending::Opaque,
				nullptr
			);
		}
		++s_InstanceCount;

	}
	LineRenderer::~LineRenderer() {
		for (const auto& mesh : m_LineMeshes) {
			Vortex::Console::WriteDebug("[LineRenderer] Mesh %u cleaned automatically.", mesh.first);
			m_Renderer->DestroyMesh(mesh.first);
		}

		--s_InstanceCount;
		if (s_InstanceCount == 0) {
			m_Renderer->DestroyMaterial(s_Material);
			m_Renderer->DestroyShader(s_ShaderHandle);
		}
	}

	Handle LineRenderer::CreateLineMesh(SizeType line_capacity) {
		if (line_capacity > m_TotalLineCapacity) {
			m_Indices.resize(line_capacity * IndexPerLine, 0);
			m_PositionBuffer.resize(line_capacity * VertexPerLine * 3, 0.0f);
			m_ColorBuffer.resize(line_capacity * VertexPerLine * 4, 0.0f);

			//fill index buffer
			for (Vortex::UInt32 i = 0; i < m_Indices.capacity(); ++i) {
				m_Indices[i] = i;
			}

			m_TotalLineCapacity = line_capacity;
		}

		MeshLayout layout = CreateMeshLayout(
			ElementType::Float3,
			ElementType::Float4
		);
		auto mesh = m_Renderer->CreateMesh(
			Vortex::Graphics::Topology::LineList,
			Vortex::Graphics::BufferUsage::DynamicDraw,
			layout,
			line_capacity * VertexPerLine,
			line_capacity * IndexPerLine
		);

		m_Renderer->SetMeshIndices(mesh, m_Indices.data(), line_capacity * IndexPerLine);

		LineMesh line_mesh{};
		line_mesh.Capacity = line_capacity;
		m_LineMeshes.emplace(std::pair(mesh, line_mesh));
		return mesh;
	}

	void LineRenderer::Begin() {
		m_CurrentVertexIndex = 0;
	}
	void LineRenderer::Insert(const Vortex::Math::Vector3& position1, const Vortex::Math::Vector3& position2, const Vortex::Math::Color& color) {
		VORTEX_ASSERT(m_CurrentVertexIndex < m_TotalLineCapacity)

		//bake position
		m_PositionBuffer[m_CurrentVertexIndex * VertexPerLine * 3 + 0] = position1.x;
		m_PositionBuffer[m_CurrentVertexIndex * VertexPerLine * 3 + 1] = position1.y;
		m_PositionBuffer[m_CurrentVertexIndex * VertexPerLine * 3 + 2] = position1.z;

		m_PositionBuffer[m_CurrentVertexIndex * VertexPerLine * 3 + 3] = position2.x;
		m_PositionBuffer[m_CurrentVertexIndex * VertexPerLine * 3 + 4] = position2.y;
		m_PositionBuffer[m_CurrentVertexIndex * VertexPerLine * 3 + 5] = position2.z;

		//bake colors
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 0] = color.r;
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 1] = color.g;
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 2] = color.b;
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 3] = color.a;

		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 4] = color.r;
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 5] = color.g;
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 6] = color.b;
		m_ColorBuffer[m_CurrentVertexIndex * VertexPerLine * 4 + 7] = color.a;

		++m_CurrentVertexIndex;
	}
	void LineRenderer::End(Handle line_mesh_handle) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_CurrentVertexIndex <= m_LineMeshes[line_mesh_handle].Capacity)

		auto vertex_count = m_CurrentVertexIndex * VertexPerLine;
		auto index_count = m_CurrentVertexIndex * IndexPerLine;

		const auto& line_mesh = m_LineMeshes[line_mesh_handle];
		m_Renderer->SetMeshIndexCount(line_mesh_handle, index_count);
		m_Renderer->SetMeshData(
			line_mesh_handle,
			0,
			m_PositionBuffer.data(),
			vertex_count * 3 * sizeof(float)
		);
		m_Renderer->SetMeshData(
			line_mesh_handle,
			1,
			m_ColorBuffer.data(),
			vertex_count * 4 * sizeof(float)
		);
	}

	void LineRenderer::BakeCircle(
		Handle line_mesh_handle,
		float radius,
		const Vortex::Math::Color& color,
		Vortex::SizeType number_of_segments
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= number_of_segments)

		Vortex::Math::Vector3 start_pos{
			radius * Vortex::Math::Cos(0.0f)
			, radius * Vortex::Math::Sin(0.0f)
			, 0.0f
		};
		Vortex::Math::Vector3 last_pos{start_pos};

		Begin();
		for (Vortex::SizeType i = 0; i <= number_of_segments; i++) {
			float theta = Vortex::Math::TwoPi<float>() * static_cast<float>(i) / static_cast<float>(number_of_segments); //get the current angle

			Vortex::Math::Vector3 current_pos{
				radius * Vortex::Math::Cos(theta)
				, radius * Vortex::Math::Sin(theta)
				, 0.0f
			};

			Insert(current_pos, last_pos, color);
			last_pos = current_pos;
		}
		End(line_mesh_handle);
	}
	void LineRenderer::BakeCircularSector(
		Handle line_mesh_handle,
		float radius,
		Vortex::Math::Angle angle,
		const Vortex::Math::Color& color,
		Vortex::SizeType number_of_segments
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= number_of_segments + 2)

		Vortex::Math::Vector3 center_pos{
			0.0f
			, 0.0f
			, 0.0f
		};

		Vortex::Math::Vector3 last_pos{
			radius * Vortex::Math::Cos(0.0f)
			, radius * Vortex::Math::Sin(0.0f)
			, 0.0f
		};
		Begin();
		Insert(center_pos, last_pos, color);
		for (Vortex::SizeType i = 0; i <= number_of_segments; i++) {
			float theta = angle.ToRadians() * static_cast<float>(i) / static_cast<float>(number_of_segments);

			Vortex::Math::Vector3 current_pos{
				radius * Vortex::Math::Cos(theta)
				, radius * Vortex::Math::Sin(theta)
				, 0.0f
			};

			Insert(current_pos, last_pos, color);
			last_pos = current_pos;
		}
		Insert(center_pos, last_pos, color);
		End(line_mesh_handle);
	}
	void LineRenderer::BakeCircularSectorCentered(
		Handle line_mesh_handle,
		float radius,
		Vortex::Math::Angle angle,
		const Vortex::Math::Color& color,
		Vortex::SizeType number_of_segments
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= number_of_segments + 2)

		Vortex::Math::Vector3 center_pos{
			0.0f
			, 0.0f
			, 0.0f
		};

		auto start_angle = angle / 2.0f;
		Vortex::Math::Vector3 last_pos{
			radius * Vortex::Math::Cos<float>(start_angle.ToRadians())
			, radius * Vortex::Math::Sin<float>(start_angle.ToRadians())
			, 0.0f
		};
		Begin();
		Insert(center_pos, last_pos, color);
		for (Vortex::SizeType i = 0; i <= number_of_segments; i++) {
			float theta = start_angle.ToRadians() - (angle.ToRadians() * static_cast<float>(i) / static_cast<float>(number_of_segments));

			Vortex::Math::Vector3 current_pos{
				radius * Vortex::Math::Cos(theta)
				, radius * Vortex::Math::Sin(theta)
				, 0.0f
			};

			Insert(current_pos, last_pos, color);
			last_pos = current_pos;
		}
		Insert(center_pos, last_pos, color);
		End(line_mesh_handle);
	}
	void LineRenderer::BakeRectangle(Handle line_mesh_handle, const Vortex::Math::Vector2& size, const Vortex::Math::Color& color) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= 4)

		Vortex::Math::Vector3 position1{0.0f, 0.0f, 0.0f};
		Vortex::Math::Vector3 position2{size[0], 0.0f, 0.0f};
		Vortex::Math::Vector3 position3{size[0], size[1], 0.0f};
		Vortex::Math::Vector3 position4{0.0f, size[1], 0.0f};

		Begin();

		Insert(position1, position2, color);
		Insert(position2, position3, color);
		Insert(position3, position4, color);
		Insert(position4, position1, color);

		End(line_mesh_handle);
	}
	void LineRenderer::BakeRectangleCentered(Handle line_mesh_handle, const Vortex::Math::Vector2& size, const Vortex::Math::Color& color) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= 4)

		Vortex::Math::Vector3 position1{-size[0] * 0.5f, -size[1] * 0.5f, 0.0f};
		Vortex::Math::Vector3 position2{size[0] * 0.5f, -size[1] * 0.5f, 0.0f};
		Vortex::Math::Vector3 position3{size[0] * 0.5f, size[1] * 0.5f, 0.0f};
		Vortex::Math::Vector3 position4{-size[0] * 0.5f, size[1] * 0.5f, 0.0f};

		Begin();

		Insert(position1, position2, color);
		Insert(position2, position3, color);
		Insert(position3, position4, color);
		Insert(position4, position1, color);

		End(line_mesh_handle);
	}

	void LineRenderer::Bake2DGrid(
		Handle line_mesh_handle,
		const Vortex::Math::Vector2Int& grid_size,
		const Vortex::Math::Vector2& cell_size,
		const Vortex::Math::Color& color
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= static_cast<SizeType>((grid_size.x + 1) * (grid_size.y + 1)))

		Begin();
		float x_min = 0.0f;
		float x_max = static_cast<float>(grid_size.x) * cell_size.x;

		float y_min = 0.0f;
		float y_max = static_cast<float>(grid_size.y) * cell_size.y;

		for (int i = 0; i < grid_size.x; ++i) {
			float current_x_pos = static_cast<float>(i) * cell_size.x;

			Vortex::Math::Vector3 y_begin{current_x_pos, y_min, 0.0f};
			Vortex::Math::Vector3 y_end{current_x_pos, y_max, 0.0f};
			Insert(y_begin, y_end, color);
		}
		for (int i = 0; i < grid_size.y; ++i) {
			float current_y_pos = static_cast<float>(i) * cell_size.y;

			Vortex::Math::Vector3 x_begin{x_min, current_y_pos, 0.0f};
			Vortex::Math::Vector3 x_end{x_max, current_y_pos, 0.0f};
			Insert(x_begin, x_end, color);
		}
		End(line_mesh_handle);
	}

	void LineRenderer::BakeDimensionArrows(
		Handle line_mesh_handle,
		float size,
		const Math::Color& forward_color,
		const Math::Color& up_color,
		const Math::Color& right_color,
		bool bake_labels,
		float label_width,
		float label_height
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))

		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= 3)
		Math::Vector3 origin{0.0f, 0.0f, 0.0f};
		Begin();

		Insert(origin, Math::Vector3::Forward() * size, forward_color);
		Insert(origin, Math::Vector3::Up() * size, up_color);
		Insert(origin, Math::Vector3::Right() * size, right_color);

		if (bake_labels) {
			//check capacity for extra lines from labels
			VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= (3 + 8))
			float label_offset = Math::Max(label_width, label_height) + size;
			//X
			Math::Vector3 x_character_origin{Math::Vector3::Right() * label_offset};
			Math::Vector3 x_top_left{x_character_origin + Math::Vector3{0.0f, -label_height, -label_width}};
			Math::Vector3 x_top_right{x_character_origin + Math::Vector3{0.0f, -label_height, label_width}};
			Math::Vector3 x_bottom_left{x_character_origin + Math::Vector3{0.0f, label_height, -label_width}};
			Math::Vector3 x_bottom_right{x_character_origin + Math::Vector3{0.0f, label_height, label_width}};

			Insert(x_top_left, x_bottom_right, right_color);
			Insert(x_top_right, x_bottom_left, right_color);

			//Y
			Math::Vector3 y_character_origin{Math::Vector3::Up() * label_offset};
			Math::Vector3 y_top_left{y_character_origin + Math::Vector3{-label_width, 0.0f, +label_height}};
			Math::Vector3 y_top_right{y_character_origin + Math::Vector3{label_width, 0.0f, +label_height}};
			Math::Vector3 y_bottom{y_character_origin + Math::Vector3{0.0f, 0.0f, -label_height}};

			Insert(y_top_left, y_character_origin, up_color);
			Insert(y_top_right, y_character_origin, up_color);
			Insert(y_bottom, y_character_origin, up_color);

			//Z
			Math::Vector3 z_character_origin{Math::Vector3::Forward() * label_offset};
			Math::Vector3 z_top_left{z_character_origin + Math::Vector3{-label_width, -label_height, 0.0f}};
			Math::Vector3 z_top_right{z_character_origin + Math::Vector3{label_width, -label_height, 0.0f}};
			Math::Vector3 z_bottom_left{z_character_origin + Math::Vector3{-label_width, label_height, 0.0f}};
			Math::Vector3 z_bottom_right{z_character_origin + Math::Vector3{label_width, label_height, 0.0f}};

			Insert(z_top_left, z_top_right, forward_color);
			Insert(z_top_right, z_bottom_left, forward_color);
			Insert(z_bottom_left, z_bottom_right, forward_color);
		}

		End(line_mesh_handle);
	}
	void LineRenderer::BakeCubeCentered(
		Handle line_mesh_handle,
		Math::Vector3 size,
		const Math::Color& color
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= 12)

		constexpr static Vortex::Math::Vector3 vertex_positions[]{
			{  -1.0f, -1.0f, -1.0f}
			, {+1.0f, -1.0f, -1.0f}
			, {+1.0f, +1.0f, -1.0f}
			, {-1.0f, +1.0f, -1.0f}

			, {-1.0f, -1.0f, +1.0f}
			, {+1.0f, -1.0f, +1.0f}
			, {+1.0f, +1.0f, +1.0f}
			, {-1.0f, +1.0f, +1.0f}
		};

		Begin();
		for (int i = 0; i < 4; ++i) {
			Insert(vertex_positions[i] * size * 0.5f, vertex_positions[(i + 1) % 4] * size * 0.5f, color);
			Insert(vertex_positions[i + 4] * size * 0.5f, vertex_positions[(i + 1) % 4 + 4] * size * 0.5f, color);
			Insert(vertex_positions[i] * size * 0.5f, vertex_positions[i + 4] * size * 0.5f, color);
		}

		End(line_mesh_handle);
	}
	void LineRenderer::Bake3DGrid(
		Handle line_mesh_handle,
		const Math::Vector3Int& grid_size,
		const Math::Vector3& cell_size,
		const Math::Color& color
	) {
		VORTEX_ASSERT(IsValid(line_mesh_handle))
		VORTEX_ASSERT(m_LineMeshes[line_mesh_handle].Capacity >= static_cast<SizeType>((grid_size.x + 1) * (grid_size.y + 1) * (grid_size.z + 1)))

		float x_min = 0.0f;
		float x_max = static_cast<float>(grid_size.x) * cell_size.x;

		float y_min = 0.0f;
		float y_max = static_cast<float>(grid_size.y) * cell_size.y;

		float z_min = 0.0f;
		float z_max = static_cast<float>(grid_size.z) * cell_size.z;

		Begin();
		for (int i = 0; i <= grid_size.x; ++i) {
			float current_x_pos = static_cast<float>(i) * cell_size.x;
			for (int j = 0; j <= grid_size.y; ++j) {
				float current_y_pos = static_cast<float>(j) * cell_size.y;

				Vortex::Math::Vector3 begin{current_x_pos, current_y_pos, z_min};
				Vortex::Math::Vector3 end{current_x_pos, current_y_pos, z_max};
				Insert(begin, end, color);
			}
		}
		for (int i = 0; i <= grid_size.x; ++i) {
			float current_x_pos = static_cast<float>(i) * cell_size.x;
			for (int j = 0; j <= grid_size.z; ++j) {
				float current_z_pos = static_cast<float>(j) * cell_size.z;

				Vortex::Math::Vector3 begin{current_x_pos, y_min, current_z_pos};
				Vortex::Math::Vector3 end{current_x_pos, y_max, current_z_pos};
				Insert(begin, end, color);
			}
		}
		for (int i = 0; i <= grid_size.z; ++i) {
			float current_z_pos = static_cast<float>(i) * cell_size.z;
			for (int j = 0; j <= grid_size.y; ++j) {
				float current_y_pos = static_cast<float>(j) * cell_size.y;

				Vortex::Math::Vector3 begin{x_min, current_y_pos, current_z_pos};
				Vortex::Math::Vector3 end{x_max, current_y_pos, current_z_pos};
				Insert(begin, end, color);
			}
		}

		End(line_mesh_handle);
	}
}