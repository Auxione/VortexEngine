#pragma once
#include "Vortex/Memory/Memory.h"

namespace Vortex::Graphics {
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

			, sizeof(UInt32)    //DepthStencil
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

	namespace RenderStates {
		enum Enum {
			FaceCulling,
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

	namespace DepthTesting {
		enum Enum {
			Never = 0,
			Less,
			LessEqual,
			Equal,
			GreaterEqual,
			Greater,
			Always,

			Count,

			Disabled = Never
		};
	}

	namespace Blending {
		enum Enum {
			Opaque = 0,
			Additive,
			Subtractive,

			Count,

			Disabled = Opaque
		};
		constexpr static const char* ToString[]{
			"Opaque"
			, "Additive"
			, "Subtractive"
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

	namespace ElementType {
		enum Enum {
			Float1 = 0,
			Float2,
			Float3,
			Float4,

			Matrix2,
			Matrix3,
			Matrix4,

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

			Sampler1D,
			Sampler2D,
			Sampler3D,

			SamplerTypeCount,

			Count
		};

		constexpr static const UInt16 Size[]{
			4 * 1      //Float1
			, 4 * 2    //Float2
			, 4 * 3    //Float3
			, 4 * 4    //Float4

			, 2 * 2    //Matrix2
			, 3 * 3    //Matrix3
			, 4 * 4    //Matrix4

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

			, 4    //Matrix2
			, 9    //Matrix3
			, 16    //Matrix4

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

			, 1        //Sampler1D
			, 1        //Sampler2D
			, 1        //Sampler3D

			, 0        //SamplerTypeCount
		};

		constexpr static const char* ToString[]{
			"Float1"
			, "Float2"
			, "Float3"
			, "Float4"

			, "Matrix2"
			, "Matrix3"
			, "Matrix4"

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

			, "Sampler1D"
			, "Sampler2D"
			, "Sampler3D"

			, "SamplerTypeCount"
		};
	}

	namespace ViewLayer {
		enum Enum {
			World = 0,
			HUD
		};
		constexpr const char* ToString[]{
			"World"
			, "HUD"
		};
	}

	namespace MeshAttribute {
		enum Enum {
			Position = 0,
			Normal,
			Color,
			UV,

			Count
		};
		constexpr ElementType::Enum ElementType[]{
			ElementType::Float3
			, ElementType::Float3
			, ElementType::Float4
			, ElementType::Float2
		};
		constexpr const char* ToString[]{
			"Position"
			, "Normal"
			, "Color"
			, "UV"
		};
	}
}