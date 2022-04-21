#pragma once
#include "Vortex/Graphics/GraphicsEnum.h"

#include <glad/glad.h>

namespace Vortex::Graphics {
	namespace Topology {
		constexpr static GLint ToGLType[]{
			GL_TRIANGLES            // TriangleList
			, GL_TRIANGLE_STRIP        // TriangleStrip
			, GL_TRIANGLE_FAN        // TriangleFan
			, GL_LINES                // LineList
			, GL_LINE_STRIP            // LineStrip
			, GL_POINTS                // PointList
		};
	}

	namespace PixelFormat {
		constexpr static GLint ToGLInternalType[]{
			GL_R8                    // Red_UI8
			, GL_R8                    // Alpha_UI8
			, GL_RGB8                // RGB_UI8
			, GL_RGBA8            // RGBA_UI8
			, GL_STENCIL_INDEX8        // Stencil_UI8
			, GL_R32UI                // Red_UI32
			, GL_R32UI                // Alpha_UI32
			, GL_DEPTH_COMPONENT32F    // Depth_F32
			, GL_R32F                // Red_F32
			, GL_R32F                // Alpha_F32
			, GL_RGB32F                // RGB_F32
			, GL_RGBA32F            // RGBA_F32
			, GL_DEPTH24_STENCIL8    // DepthStencil
		};

		constexpr static GLenum ToGLFormatType[]{
			GL_RED                    // Red_UI8
			, GL_RED                // Alpha_UI8
			, GL_RGB                // RGB_UI8
			, GL_RGBA                // RGBA_UI8
			, GL_STENCIL_INDEX        // Stencil_UI8
			, GL_RGBA_INTEGER        // Red_UI32
			, GL_RGBA_INTEGER        // Alpha_UI32
			, GL_DEPTH_COMPONENT    // Depth_F32
			, GL_RED                // Red_F32
			, GL_RED                // Alpha_F32
			, GL_RGB                // RGB_F32
			, GL_RGBA                // RGBA_F32
			, GL_DEPTH_STENCIL        // DepthStencil
		};

		constexpr static GLenum ToGLType[]{
			GL_UNSIGNED_BYTE        // Red_UI8
			, GL_UNSIGNED_BYTE        // Alpha_UI8
			, GL_UNSIGNED_BYTE        // RGB_UI8
			, GL_UNSIGNED_BYTE        // RGBA_UI8
			, GL_UNSIGNED_BYTE        // Stencil_UI8
			, GL_UNSIGNED_INT        // Red_UI32
			, GL_UNSIGNED_INT        // Alpha_UI32
			, GL_FLOAT                // Depth_F32
			, GL_FLOAT                // Red_F32
			, GL_FLOAT                // Alpha_F32
			, GL_FLOAT                // RGB_F32
			, GL_FLOAT                // RGBA_F32
			, GL_UNSIGNED_INT_24_8    // DepthStencil
		};

		constexpr static GLenum ToGLImageTexture[]{
			GL_R8UI          // Red_UI8
			, GL_R8UI        // Alpha_UI8
			, GL_RGB8UI      // RGB_UI8
			, GL_RGBA8UI     // RGBA_UI8
			, GL_R8UI        // Stencil_UI8
			, GL_R32UI       // Red_UI32
			, GL_R32UI       // Alpha_UI32
			, GL_R32F        // Depth_F32
			, GL_R32F        // Red_F32
			, GL_R32F        // Alpha_F32
			, GL_RGB32F      // RGB_F32
			, GL_RGBA32F     // RGBA_F32
			, GL_NONE         // DepthStencil
		};

		constexpr static GLenum ToGLFrameBufferAttachment[]{
			GL_COLOR_ATTACHMENT0, // Red_UI8
			GL_COLOR_ATTACHMENT0, // Alpha_UI8
			GL_COLOR_ATTACHMENT0, // RGB_UI8
			GL_COLOR_ATTACHMENT0, // RGBA_UI8

			GL_STENCIL_ATTACHMENT, // Stencil_UI8

			GL_COLOR_ATTACHMENT0, // Red_UI32
			GL_COLOR_ATTACHMENT0, // Alpha_UI32

			GL_DEPTH_ATTACHMENT, // Depth_F32

			GL_COLOR_ATTACHMENT0, // Red_F32
			GL_COLOR_ATTACHMENT0, // Alpha_F32
			GL_COLOR_ATTACHMENT0, // RGB_F32
			GL_COLOR_ATTACHMENT0, // RGBA_F32

			GL_DEPTH_STENCIL_ATTACHMENT // DepthStencil
		};
	}

	namespace TextureWrap {
		constexpr static GLint ToGLType[]{
			GL_REPEAT                // Repeat
			, GL_MIRRORED_REPEAT    // RepeatMirrored
			, GL_CLAMP_TO_BORDER    // ClampToBorder
			, GL_CLAMP_TO_EDGE        // ClampToEdge
		};
	}

	namespace TextureLODFilter {
		constexpr static GLint ToGLType[]{
			GL_LINEAR                // Linear
			, GL_NEAREST            // Nearest
		};
	}

	namespace ShaderType {
		constexpr static GLint ToGLType[]{
			GL_VERTEX_SHADER        // Vertex
			, GL_FRAGMENT_SHADER    // Fragment
			, GL_GEOMETRY_SHADER    // Geometry
			, GL_COMPUTE_SHADER        // Compute
		};
	}

	namespace BufferUsage {
		constexpr static GLint ToGLType[]{
			GL_STATIC_DRAW            // StaticDraw
			, GL_STATIC_READ        // StaticRead
			, GL_STATIC_COPY        // StaticCopy
			, GL_DYNAMIC_DRAW        // DynamicDraw
			, GL_DYNAMIC_READ        // DynamicRead
			, GL_DYNAMIC_COPY        // DynamicCopy
			, GL_STREAM_DRAW        // StreamDraw
			, GL_STREAM_READ        // StreamRead
			, GL_STREAM_COPY        // StreamCopy
		};
	}

	namespace DepthTesting {
		constexpr static GLenum ToGLType[]{
			GL_NEVER, //Never
			GL_LESS, //Less
			GL_LEQUAL, //LessEqual
			GL_EQUAL, //Equal
			GL_GEQUAL, //GreaterEqual
			GL_GREATER, //Greater
			GL_ALWAYS //Always
		};
	}

	namespace ElementType {
		constexpr static GLenum ToGLUniformType[]{
			GL_FLOAT,        //Float1
			GL_FLOAT_VEC2,  //Float2
			GL_FLOAT_VEC3,  //Float3
			GL_FLOAT_VEC4,  //Float4

			GL_FLOAT_MAT2,    //Matrix2
			GL_FLOAT_MAT3,    //Matrix3
			GL_FLOAT_MAT4,    //Matrix4

			0,            //FloatingTypeCount

			GL_INT,        //Int1
			GL_INT_VEC2,   //Int2
			GL_INT_VEC3,   //Int3
			GL_INT_VEC4,   //Int4

			GL_UNSIGNED_INT,        //UInt1
			GL_UNSIGNED_INT_VEC2,   //UInt2
			GL_UNSIGNED_INT_VEC3,   //UInt3
			GL_UNSIGNED_INT_VEC4,   //UInt4

			GL_BYTE,        //Byte1
			GL_BYTE,        //Byte2
			GL_BYTE,        //Byte3
			GL_BYTE,        //Byte4

			GL_UNSIGNED_BYTE,    //UByte1
			GL_UNSIGNED_BYTE,    //UByte2
			GL_UNSIGNED_BYTE,    //UByte3
			GL_UNSIGNED_BYTE,    //UByte4

			GL_SHORT,        //Short1
			GL_SHORT,        //Short2
			GL_SHORT,        //Short3
			GL_SHORT,        //Short4

			GL_UNSIGNED_SHORT,    //UShort1
			GL_UNSIGNED_SHORT,    //UShort2
			GL_UNSIGNED_SHORT,    //UShort3
			GL_UNSIGNED_SHORT,    //UShort4

			0,            //IntegerTypeCount

			GL_SAMPLER_1D,    //Sampler1D
			GL_SAMPLER_2D,    //Sampler2D
			GL_SAMPLER_3D,    //Sampler3D

			0,            //SamplerTypeCount

			GL_IMAGE_2D,    //Image1D
			GL_IMAGE_2D,    //Image2D
			GL_IMAGE_2D,    //Image3D

			0            //ImageTypeCount
		};

		constexpr static GLenum ToGLBufferType[]{
			GL_FLOAT,        //Float1
			GL_FLOAT,  //Float2
			GL_FLOAT,  //Float3
			GL_FLOAT,  //Float4

			GL_FLOAT,    //Matrix2
			GL_FLOAT,    //Matrix3
			GL_FLOAT,    //Matrix4

			0,            //FloatingTypeCount

			GL_INT,        //Int1
			GL_INT,   //Int2
			GL_INT,   //Int3
			GL_INT,   //Int4

			GL_UNSIGNED_INT,        //UInt1
			GL_UNSIGNED_INT,   //UInt2
			GL_UNSIGNED_INT,   //UInt3
			GL_UNSIGNED_INT,   //UInt4

			GL_BYTE,        //Byte1
			GL_BYTE,        //Byte2
			GL_BYTE,        //Byte3
			GL_BYTE,        //Byte4

			GL_UNSIGNED_BYTE,    //UByte1
			GL_UNSIGNED_BYTE,    //UByte2
			GL_UNSIGNED_BYTE,    //UByte3
			GL_UNSIGNED_BYTE,    //UByte4

			GL_SHORT,        //Short1
			GL_SHORT,        //Short2
			GL_SHORT,        //Short3
			GL_SHORT,        //Short4

			GL_UNSIGNED_SHORT,    //UShort1
			GL_UNSIGNED_SHORT,    //UShort2
			GL_UNSIGNED_SHORT,    //UShort3
			GL_UNSIGNED_SHORT,    //UShort4

			0            //IntegerTypeCount
		};
	}
}