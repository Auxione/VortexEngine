#pragma once
#include "Vortex/Memory/Memory.h"

namespace Vortex::Graphics {
	namespace HardwareLimit {
		enum Enum {
			MaxTextureSize,
			MaxTextureLayers,
			MaxTextureSamplers,

			MaxFrameBufferWidth,
			MaxFrameBufferHeight,
			MaxFrameBufferColorAttachments,
			MaxDrawBuffers,

			MaxVertexAttributes,

			MaxComputeWorkGroupX,
			MaxComputeWorkGroupY,
			MaxComputeWorkGroupZ,

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

			, "MaxComputeWorkGroupX"
			, "MaxComputeWorkGroupY"
			, "MaxComputeWorkGroupZ"
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
			, "TriangleStrip"
			, "TriangleFan"

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
	namespace ShaderTags {
		enum Enum {
			Undefined = 0,
			Lit = 1 << 1,
			Instanced = 1 << 2
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
			Color0,
			Color1,
			Color2,
			Color3,
			Color4,
			Color5,
			Color6,
			Color7,

			ColorAttachmentCount,

			Depth,
			Stencil,
			DepthStencil,

			Count
		};

		constexpr static PixelFormat::Enum Format[]{
			PixelFormat::RGBA_F32        //Color0
			, PixelFormat::RGBA_F32        //Color1
			, PixelFormat::RGBA_F32        //Color2
			, PixelFormat::RGBA_F32        //Color3
			, PixelFormat::RGBA_F32        //Color4
			, PixelFormat::RGBA_F32        //Color5
			, PixelFormat::RGBA_F32        //Color6
			, PixelFormat::RGBA_F32        //Color7

			, PixelFormat::Count        //ColorAttachmentCount

			, PixelFormat::Depth_F32    //Depth
			, PixelFormat::Stencil_UI8    //Stencil
			, PixelFormat::DepthStencil    //DepthStencil

			, PixelFormat::Count        //Count
		};
		constexpr static const char* ToString[]{
			"Color0"
			, "Color1"
			, "Color2"
			, "Color3"
			, "Color4"
			, "Color5"
			, "Color6"
			, "Color7"

			, "ColorAttachmentCount"

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

			Image1D,
			Image2D,
			Image3D,

			ImageTypeCount,

			UniformBlock,
			StorageBlock,

			Count
		};

		constexpr static const UInt16 Size[]{
			sizeof(float) * 1      //Float1
			, sizeof(float) * 2    //Float2
			, sizeof(float) * 3    //Float3
			, sizeof(float) * 4    //Float4

			, sizeof(float) * 2 * 2    //Matrix2
			, sizeof(float) * 3 * 3    //Matrix3
			, sizeof(float) * 4 * 4    //Matrix4

			, 0        //FloatingTypeCount

			, sizeof(Int32) * 1    //Int1
			, sizeof(Int32) * 2    //Int2
			, sizeof(Int32) * 3    //Int3
			, sizeof(Int32) * 4    //Int4

			, sizeof(UInt32) * 1    //UInt1
			, sizeof(UInt32) * 2    //UInt2
			, sizeof(UInt32) * 3    //UInt3
			, sizeof(UInt32) * 4    //UInt4

			, sizeof(Byte) * 1    //Byte1
			, sizeof(Byte) * 2    //Byte2
			, sizeof(Byte) * 3    //Byte3
			, sizeof(Byte) * 4    //Byte4

			, sizeof(UByte) * 1    //UByte1
			, sizeof(UByte) * 2    //UByte2
			, sizeof(UByte) * 3    //UByte3
			, sizeof(UByte) * 4    //UByte4

			, sizeof(Int16) * 1        //Short1
			, sizeof(Int16) * 2        //Short2
			, sizeof(Int16) * 3        //Short3
			, sizeof(Int16) * 4        //Short4

			, sizeof(UInt16) * 1        //UShort1
			, sizeof(UInt16) * 2        //UShort2
			, sizeof(UInt16) * 3        //UShort3
			, sizeof(UInt16) * 4        //UShort4

			, 0        //IntegerTypeCount
		};

		constexpr static const UInt16 ComponentCount[]{
			1      //Float1
			, 2    //Float2
			, 3    //Float3
			, 4    //Float4

			, 2 * 2    //Matrix2
			, 3 * 3    //Matrix3
			, 4 * 4    //Matrix4

			, 0    //FloatingTypeCount

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

			, 1    //Short1
			, 2    //Short2
			, 3    //Short3
			, 4    //Short4

			, 1    //UShort1
			, 2    //UShort2
			, 3    //UShort3
			, 4    //UShort4

			, 0    //IntegerTypeCount
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

			, "Image1D"
			, "Image2D"
			, "Image3D"

			, "SamplerTypeCount"

			, "UniformBlock"
			, "StorageBlock"
		};
	}

	namespace ViewLayer {
		enum Enum {
			World = 0,
			HUD,
			PostProcess,

			Count
		};
		constexpr const char* ToString[]{
			"World"
			, "HUD"
			, "PostProcess"
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