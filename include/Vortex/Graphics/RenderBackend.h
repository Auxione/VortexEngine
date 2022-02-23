#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Common/HashedString.h"
#include "Vortex/Common/Handle.h"
#include "Vortex/Containers/Vector.h"

#include <unordered_map>

namespace Vortex::Graphics {
	namespace Topology {
		enum Enum {
			TriangleList = 0,
			TriangleStrip,
			TriangleFan,

			LineList,
			LineStrip,

			PointList,

			Count
		};
		constexpr static const char* ToString[]{
			"TriangleList"
			, "TriangleFan"
			, "TriangleStrip"

			, "LineList"
			, "LineStrip"

			, "PointList"
		};
	}

	namespace PixelFormat {
		enum Enum {
			Red_UI8 = 0,
			Alpha_UI8,
			RGB_UI8,
			RGBA_UI8,
			Stencil_UI8,

			Red_UI32,
			Alpha_UI32,

			Depth_F32,
			Red_F32,
			Alpha_F32,
			RGB_F32,
			RGBA_F32,

			DepthStencil,
			Count
		};
		constexpr static const char* ToString[]{
			"Red_UI8"
			, "Alpha_UI8"
			, "RGB_UI8"
			, "RGBA_UI8"
			, "Stencil_UI8"

			, "Red_UI32"
			, "Alpha_UI32"

			, "Depth_F32"
			, "Red_F32"
			, "Alpha_F32"
			, "RGB_F32"
			, "RGBA_F32"

			, "DepthStencil"
		};

		constexpr static const UInt8 BitsPerPixel[]{
			sizeof(UInt8)        //Red_UI8
			, sizeof(UInt8)        //Alpha_UI8
			, sizeof(UInt8)        //RGB_UI8,
			, sizeof(UInt8)        //RGBA_UI8
			, sizeof(UInt8)        //Stencil_UI8

			, sizeof(UInt32)    //Red_UI32
			, sizeof(UInt32)    //Alpha_UI32

			, sizeof(float)    //Depth_F32
			, sizeof(float)    //Red_F32
			, sizeof(float)    //Alpha_F32
			, sizeof(float)    //RGB_F32
			, sizeof(float)    //RGBA_F32

			, 32    //DepthStencil
		};
	}

	namespace TextureWrap {
		enum Enum {
			Repeat = 0,
			RepeatMirrored,
			ClampBorder,
			ClampEdge,

			Count
		};
		constexpr static const char* ToString[]{
			"Repeat"
			, "RepeatMirrored"
			, "ClampBorder"
			, "ClampEdge"
		};
	}

	namespace TextureLODFilter {
		enum Enum {
			Linear = 0,
			Nearest,

			Count
		};
		constexpr static const char* ToString[]{
			"Linear"
			, "Nearest"
		};
	}

	namespace ShaderType {
		enum Enum {
			Vertex = 0,
			Fragment,
			Geometry,
			Compute,
			Count
		};
		constexpr static const char* ToString[]{
			"Vertex"
			, "Fragment"
			, "Geometry"
			, "Compute"
		};
	}

	namespace BufferUsage {
		enum Enum {
			StaticDraw = 0,
			StaticRead,
			StaticCopy,

			DynamicDraw,
			DynamicRead,
			DynamicCopy,

			StreamDraw,
			StreamRead,
			StreamCopy,

			Count
		};

		constexpr static const char* ToString[]{
			"StaticDraw"
			, "StaticRead"
			, "StaticCopy"

			, "DynamicDraw"
			, "DynamicRead"
			, "DynamicCopy"

			, "StreamDraw"
			, "StreamRead"
			, "StreamCopy"
		};
	}

	namespace RenderLimits {
		enum Enum {
			MaxTextureSize,
			MaxTextureLayers,
			MaxTextureSamplers,

			MaxFrameBufferWidth,
			MaxFrameBufferHeight,
			MaxFrameBufferColorAttachments,
			MaxDrawBuffers,

			MaxVertexAttributes,

			Count
		};

		constexpr static const char* ToString[]{
			"MaxTextureSize"
			, "MaxTextureLayers"
			, "MaxTextureSamplers"

			, "MaxFrameBufferWidth"
			, "MaxFrameBufferHeight"
			, "MaxFrameBufferColorAttachments"
			, "MaxDrawBuffers"

			, "MaxVertexAttributes"
		};
	}

	namespace RenderStates {
		enum Enum {
			Blending,
			FaceCulling,
			DepthTest,
			Multisampling,
			ScissorTest,
			StencilTest,

			Count
		};
	}

	namespace FaceCulling {
		enum Enum {
			Back,
			Front,
			FrontAndBack,

			Count
		};
	}

	namespace DepthFunction {
		enum Enum {
			Never,
			Less,
			LessEqual,
			Equal,
			GreaterEqual,
			Greater,
			Always,

			Count
		};
	}

	namespace BlendFunction {
		enum Enum {
			Zero,
			One,
			SourceColor,
			OneMinusSourceColor,
			DestinationColor,
			OneMinusDestinationColor,
			SourceAlpha,
			OneMinusSourceAlpha,
			DestinationAlpha,
			OneMinusDestinationAlpha,
			ConstantColor,
			OneMinusConstantColor,
			ConstantAlpha,
			OneMinusConstantAlpha,

			Count
		};
	}

	namespace FrameBufferAttachment {
		enum Enum {
			Color,
			Depth,
			Stencil,
			DepthStencil,

			Count
		};
		constexpr static const char* ToString[]{
			"Color"
			, "Depth"
			, "Stencil"
			, "DepthStencil"
		};
	}

	namespace RenderElementType {
		enum Enum {
			Float1 = 0,
			Float2,
			Float3,
			Float4,

			FloatingTypeCount,

			Int1,
			Int2,
			Int3,
			Int4,

			UInt1,
			UInt2,
			UInt3,
			UInt4,

			Byte1,
			Byte2,
			Byte3,
			Byte4,

			UByte1,
			UByte2,
			UByte3,
			UByte4,

			Short1,
			Short2,
			Short3,
			Short4,

			UShort1,
			UShort2,
			UShort3,
			UShort4,

			IntegerTypeCount,

			Count
		};

		constexpr static const UInt16 Size[]{
			4 * 1      //Float1
			, 4 * 2    //Float2
			, 4 * 3    //Float3
			, 4 * 4    //Float4

			, 0        //FloatingTypeCount

			, 4 * 1    //Int1
			, 4 * 2    //Int2
			, 4 * 3    //Int3
			, 4 * 4    //Int4

			, 4 * 1    //UInt1
			, 4 * 2    //UInt2
			, 4 * 3    //UInt3
			, 4 * 4    //UInt4

			, 1 * 1    //Byte1
			, 1 * 2    //Byte2
			, 1 * 3    //Byte3
			, 1 * 4    //Byte4

			, 1 * 1    //UByte1
			, 1 * 2    //UByte2
			, 1 * 3    //UByte3
			, 1 * 4    //UByte4

			, 2 * 1        //Short1
			, 2 * 2        //Short2
			, 2 * 3        //Short3
			, 2 * 4        //Short4

			, 2 * 1        //UShort1
			, 2 * 2        //UShort2
			, 2 * 3        //UShort3
			, 2 * 4        //UShort4

			, 0        //IntegerTypeCount
		};

		constexpr static const UInt16 ComponentCount[]{
			1      //Float1
			, 2    //Float2
			, 3    //Float3
			, 4    //Float4

			, 0        //FloatingTypeCount

			, 1    //Int1
			, 2    //Int2
			, 3    //Int3
			, 4    //Int4

			, 1    //UInt1
			, 2    //UInt2
			, 3    //UInt3
			, 4    //UInt4

			, 1    //Byte1
			, 2    //Byte2
			, 3    //Byte3
			, 4    //Byte4

			, 1    //UByte1
			, 2    //UByte2
			, 3    //UByte3
			, 4    //UByte4

			, 1        //Short1
			, 2        //Short2
			, 3        //Short3
			, 4        //Short4

			, 1        //UShort1
			, 2        //UShort2
			, 3        //UShort3
			, 4        //UShort4

			, 0        //IntegerTypeCount
		};

		constexpr static const char* ToString[]{
			"Float1"
			, "Float2"
			, "Float3"
			, "Float4"

			, "FloatingTypeCount"

			, "Int1"
			, "Int2"
			, "Int3"
			, "Int4"

			, "UInt1"
			, "UInt2"
			, "UInt3"
			, "UInt4"

			, "Byte1"
			, "Byte2"
			, "Byte3"
			, "Byte4"

			, "UByte1"
			, "UByte2"
			, "UByte3"
			, "UByte4"

			, "Short1"
			, "Short2"
			, "Short3"
			, "Short4"

			, "UShort1"
			, "UShort2"
			, "UShort3"
			, "UShort4"

			, "IntegerTypeCount"
		};
	}

	/*
	#define RenderElementType_Mat2 \
	Vortex::Graphics::RenderElementType::Float2, \
	Vortex::Graphics::RenderElementType::Float2

	#define RenderElementType_Mat3 \
	Vortex::Graphics::RenderElementType::Float3, \
	Vortex::Graphics::RenderElementType::Float3, \
	Vortex::Graphics::RenderElementType::Float3

	#define RenderElementType_Mat4 \
	Vortex::Graphics::RenderElementType::Float4, \
	Vortex::Graphics::RenderElementType::Float4, \
	Vortex::Graphics::RenderElementType::Float4, \
	Vortex::Graphics::RenderElementType::Float4
	*/

	class BufferLayout {
	public:
		struct BufferElement {
			RenderElementType::Enum Type;
			UInt16 Size;
			UInt16 Offset;
			bool Normalized;
		};

		using iterator = Vector<BufferElement>::iterator;
		using const_iterator = Vector<BufferElement>::const_iterator;

	public:
		BufferLayout(): m_Stride{0}, m_Elements{3}, ElementPerInstance{0} {}

		BufferLayout(std::initializer_list<RenderElementType::Enum> types)
			: m_Stride{0}, m_Elements{types.size()}, ElementPerInstance{0} {

			UInt16 offset = 0;
			for (auto type : types) {
				BufferElement element{
					type
					, RenderElementType::Size[type]
					, 0
					, false
				};

				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
				m_Elements.EmplaceBack(element);
			}
		}

	public:
		BufferElement& operator[](SizeType index) { return m_Elements[index]; }
		const BufferElement& operator[](SizeType index) const { return m_Elements[index]; }

		SizeType Count() const { return m_Elements.Size(); }
		SizeType GetStride() const { return m_Stride; }

		SizeType CalculateBufferSize(SizeType count) const { return count * GetStride(); }

	public:
		BufferLayout::iterator begin() { return m_Elements.begin(); }
		BufferLayout::iterator end() { return m_Elements.end(); }
		BufferLayout::const_iterator cbegin() const { return m_Elements.cbegin(); }
		BufferLayout::const_iterator cend() const { return m_Elements.cend(); }

	public:
		UInt16 ElementPerInstance;

	private:
		SizeType m_Stride;
		Vector<BufferElement> m_Elements;
	};

	VORTEX_HANDLE(TextureHandle);
	VORTEX_HANDLE(BufferHandle);
	VORTEX_HANDLE(VertexArrayHandle);

	VORTEX_HANDLE(FrameBufferHandle);
	VORTEX_HANDLE(ShaderHandle);
	VORTEX_HANDLE(ProgramHandle);
}

VORTEX_HANDLE_HASH(Vortex::Graphics::TextureHandle);
VORTEX_HANDLE_HASH(Vortex::Graphics::BufferHandle);
VORTEX_HANDLE_HASH(Vortex::Graphics::VertexArrayHandle);

VORTEX_HANDLE_HASH(Vortex::Graphics::FrameBufferHandle);
VORTEX_HANDLE_HASH(Vortex::Graphics::ShaderHandle);
VORTEX_HANDLE_HASH(Vortex::Graphics::ProgramHandle);

namespace Vortex::Graphics {
	struct TextureData {
		PixelFormat::Enum Format;
		UInt16 Width;
		UInt16 Height;
		UInt16 Depth;
		UInt16 LayerCount;
		UInt8 BitsPerPixel;
		bool IsCubemap;
	};

	struct BufferData {
		BufferUsage::Enum BufferUsage;
		BufferLayout Layout;
		bool Mutable;
		SizeType Size;

		BufferData() = default;
		~BufferData() = default;
	};

	struct VertexArrayData {
		BufferHandle IndexBufferHandle;
		Vector<BufferHandle> VertexBufferHandles;
		bool Indexed;
		SizeType BufferBindIndex;
		Topology::Enum Topology;
	};

	struct ShaderData {
		ShaderType::Enum Type;
	};

	struct ProgramData {
		using LocationCache = std::unordered_map<HashedString, int>;
		LocationCache ShaderLocations;
	};

	struct FrameBufferData {
		UInt16 Width;
		UInt16 Height;
		Vector<TextureHandle> Handles;
		Vector<FrameBufferAttachment::Enum> Attachments;
		bool DestroyTextures;
	};

	class RenderBackend {
	public:
		virtual ~RenderBackend() = default;

	public:// Vertex
		virtual BufferHandle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) = 0;
		virtual void UpdateBuffer(BufferHandle handle, SizeType offset, SizeType data_size, const void* data) = 0;
		virtual void GetBuffer(BufferHandle handle, SizeType offset, SizeType data_size, void* data) = 0;
		virtual void Destroy(BufferHandle handle) = 0;

		virtual VertexArrayHandle CreateVertexArray(Topology::Enum topology) = 0;
		virtual void SetIndexBuffer(VertexArrayHandle handle, BufferHandle index_buffer_handle) = 0;
		virtual void AddVertexBuffer(VertexArrayHandle handle, BufferHandle vertex_buffer_handle) = 0;
		virtual void Destroy(VertexArrayHandle handle) = 0;

	public: // Textures
		virtual TextureHandle CreateTexture2D(
			UInt16 width,
			UInt16 height,
			PixelFormat::Enum format,
			const void* pixels,
			TextureLODFilter::Enum min_filter,
			TextureLODFilter::Enum mag_filter,
			TextureWrap::Enum wrap_s,
			TextureWrap::Enum wrap_t,
			bool create_mipmap
		) = 0;
		inline TextureHandle CreateTexture2D(UInt16 width, UInt16 height, PixelFormat::Enum format, const void* pixels) {
			return CreateTexture2D(
				width,
				height,
				format,
				pixels,
				TextureLODFilter::Linear,
				TextureLODFilter::Linear,
				TextureWrap::ClampEdge,
				TextureWrap::ClampEdge,
				true
			);
		}
		virtual void UpdateTexture2D(TextureHandle handle, const void* pixels, bool create_mipmap) = 0;
		inline void UpdateTexture2D(TextureHandle handle, const void* pixels) {
			UpdateTexture2D(handle, pixels, true);
		};
		virtual void GetTexture(TextureHandle handle, UInt16 buffer_size, void* pixels) = 0;
		virtual void Bind(TextureHandle handle, UInt16 sampler) = 0;
		virtual void Destroy(TextureHandle handle) = 0;

	public:
		virtual ShaderHandle CreateShader(ShaderType::Enum shader_type, const char* source) = 0;
		virtual void Destroy(ShaderHandle handle) = 0;

		virtual ProgramHandle CreateProgram(ShaderHandle* handles, SizeType count, bool consume) = 0;
		virtual void Bind(ProgramHandle handle) = 0;
		virtual void Destroy(ProgramHandle handle) = 0;

		//virtual void SetShaderBuffer(const char* name, BufferHandle buffer_handle, UInt16 binding = 0) = 0;

		virtual void SetShaderFloat1(ProgramHandle handle, HashedString name, float x) = 0;
		virtual void SetShaderFloat2(ProgramHandle handle, HashedString name, float x, float y) = 0;
		virtual void SetShaderFloat3(ProgramHandle handle, HashedString name, float x, float y, float z) = 0;
		virtual void SetShaderFloat4(ProgramHandle handle, HashedString name, float x, float y, float z, float w) = 0;

		virtual void SetShaderInt1(ProgramHandle handle, HashedString name, Int32 x) = 0;
		virtual void SetShaderInt2(ProgramHandle handle, HashedString name, Int32 x, Int32 y) = 0;
		virtual void SetShaderInt3(ProgramHandle handle, HashedString name, Int32 x, Int32 y, Int32 z) = 0;
		virtual void SetShaderInt4(ProgramHandle handle, HashedString name, Int32 x, Int32 y, Int32 z, Int32 w) = 0;

		virtual void SetShaderUInt1(ProgramHandle handle, HashedString name, UInt32 x) = 0;
		virtual void SetShaderUInt2(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y) = 0;
		virtual void SetShaderUInt3(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y, UInt32 z) = 0;
		virtual void SetShaderUInt4(ProgramHandle handle, HashedString name, UInt32 x, UInt32 y, UInt32 z, UInt32 w) = 0;

		inline void SetShaderFloat1(ProgramHandle handle, HashedString name, const float* values) { SetShaderFloat1(handle, name, values[0]); };
		inline void SetShaderFloat2(ProgramHandle handle, HashedString name, const float* values) { SetShaderFloat2(handle, name, values[0], values[1]); };
		inline void SetShaderFloat3(ProgramHandle handle, HashedString name, const float* values) { SetShaderFloat3(handle, name, values[0], values[1], values[2]); };
		inline void SetShaderFloat4(ProgramHandle handle, HashedString name, const float* values) { SetShaderFloat4(handle, name, values[0], values[1], values[2], values[3]); };

		inline void SetShaderInt1(ProgramHandle handle, HashedString name, const Int32* values) { SetShaderInt1(handle, name, values[0]); };
		inline void SetShaderInt2(ProgramHandle handle, HashedString name, const Int32* values) { SetShaderInt2(handle, name, values[0], values[1]); };
		inline void SetShaderInt3(ProgramHandle handle, HashedString name, const Int32* values) { SetShaderInt3(handle, name, values[0], values[1], values[2]); };
		inline void SetShaderInt4(ProgramHandle handle, HashedString name, const Int32* values) { SetShaderInt4(handle, name, values[0], values[1], values[2], values[3]); };

		inline void SetShaderUInt1(ProgramHandle handle, HashedString name, const UInt32* values) { SetShaderUInt1(handle, name, values[0]); };
		inline void SetShaderUInt2(ProgramHandle handle, HashedString name, const UInt32* values) { SetShaderUInt2(handle, name, values[0], values[1]); };
		inline void SetShaderUInt3(ProgramHandle handle, HashedString name, const UInt32* values) { SetShaderUInt3(handle, name, values[0], values[1], values[2]); };
		inline void SetShaderUInt4(ProgramHandle handle, HashedString name, const UInt32* values) { SetShaderUInt4(handle, name, values[0], values[1], values[2], values[3]); };

		virtual void SetShaderFloat1Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) = 0;
		virtual void SetShaderFloat2Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) = 0;
		virtual void SetShaderFloat3Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) = 0;
		virtual void SetShaderFloat4Array(ProgramHandle handle, HashedString name, const float* array, SizeType count) = 0;

		virtual void SetShaderInt1Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) = 0;
		virtual void SetShaderInt2Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) = 0;
		virtual void SetShaderInt3Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) = 0;
		virtual void SetShaderInt4Array(ProgramHandle handle, HashedString name, const Int32* array, SizeType count) = 0;

		virtual void SetShaderUInt1Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) = 0;
		virtual void SetShaderUInt2Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) = 0;
		virtual void SetShaderUInt3Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) = 0;
		virtual void SetShaderUInt4Array(ProgramHandle handle, HashedString name, const UInt32* array, SizeType count) = 0;

		virtual void SetShaderMatrix2(ProgramHandle handle, HashedString name, bool transposed, const float* value) = 0;
		virtual void SetShaderMatrix3(ProgramHandle handle, HashedString name, bool transposed, const float* value) = 0;
		virtual void SetShaderMatrix4(ProgramHandle handle, HashedString name, bool transposed, const float* value) = 0;

	public:
		virtual FrameBufferHandle CreateFrameBuffer(
			TextureHandle* textures,
			FrameBufferAttachment::Enum* attachments,
			SizeType count,
			bool destroy_textures
		) = 0;
		virtual void Bind(FrameBufferHandle handle) = 0;
		virtual void BindBackBuffer() = 0;
		virtual void Destroy(FrameBufferHandle handle) = 0;

	public:
		virtual SizeType GetRenderLimit(RenderLimits::Enum type) const = 0;

		virtual void SetState(RenderStates::Enum state, bool active) = 0;
		virtual bool IsEnabled(RenderStates::Enum state) const = 0;
		virtual void SetDepthFunction(DepthFunction::Enum func) const = 0;
		virtual void SetBlendFunction(BlendFunction::Enum source, BlendFunction::Enum target) const = 0;

		virtual void SetScissor(const Int32* dimension) const = 0;
		virtual void SetViewport(const Int32* size) const = 0;

		virtual void ClearColor(const float* color) const = 0;
		virtual void ClearDepth(float reset) const = 0;
		virtual void ClearStencil(UInt32 reset) const = 0;

		virtual void Draw(VertexArrayHandle handle, SizeType count) const = 0;
		virtual void DrawInstanced(VertexArrayHandle handle, SizeType count, SizeType instance_count) const = 0;

	public:
		inline const TextureData& GetData(TextureHandle handle) const { return m_TextureDatas.at(handle); };
		inline const BufferData& GetData(BufferHandle handle) const { return m_BufferDatas.at(handle); };
		inline const VertexArrayData& GetData(VertexArrayHandle handle) const { return m_VertexArrayDatas.at(handle); };

		inline const ShaderData& GetData(ShaderHandle handle) const { return m_ShaderDatas.at(handle); };
		inline const ProgramData& GetData(ProgramHandle handle) const { return m_ProgramDatas.at(handle); };
		inline const FrameBufferData& GetData(FrameBufferHandle handle) const { return m_FrameBufferDatas.at(handle); };

	public:
		inline bool IsValid(TextureHandle handle) const { return handle.id != InvalidID && m_TextureDatas.find(handle) != m_TextureDatas.end(); };
		inline bool IsValid(BufferHandle handle) const { return handle.id != InvalidID && m_BufferDatas.find(handle) != m_BufferDatas.end(); };
		inline bool IsValid(VertexArrayHandle handle) const { return handle.id != InvalidID && m_VertexArrayDatas.find(handle) != m_VertexArrayDatas.end(); };

		inline bool IsValid(FrameBufferHandle handle) const { return handle.id != InvalidID && m_FrameBufferDatas.find(handle) != m_FrameBufferDatas.end(); };
		inline bool IsValid(ShaderHandle handle) const { return handle.id != InvalidID && m_ShaderDatas.find(handle) != m_ShaderDatas.end(); };
		inline bool IsValid(ProgramHandle handle) const { return handle.id != InvalidID && m_ProgramDatas.find(handle) != m_ProgramDatas.end(); };

	protected:
		std::unordered_map<TextureHandle, TextureData> m_TextureDatas;
		std::unordered_map<BufferHandle, BufferData> m_BufferDatas;
		std::unordered_map<VertexArrayHandle, VertexArrayData> m_VertexArrayDatas;
		std::unordered_map<ShaderHandle, ShaderData> m_ShaderDatas;
		std::unordered_map<ProgramHandle, ProgramData> m_ProgramDatas;
		std::unordered_map<FrameBufferHandle, FrameBufferData> m_FrameBufferDatas;
	};
}