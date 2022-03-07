#pragma once
#include "Vortex/Graphics/GraphicsEnum.h"

#include <glad/glad.h>

namespace Vortex::OpenGL45 {
	constexpr static GLint Topology[]{
		GL_TRIANGLES            // TriangleList
		, GL_TRIANGLE_STRIP        // TriangleStrip
		, GL_TRIANGLE_FAN        // TriangleFan
		, GL_LINES                // LineList
		, GL_LINE_STRIP            // LineStrip
		, GL_POINTS                // PointList
	};

	namespace PixelFormat {
		constexpr static GLint Internal[]{
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

		constexpr static GLenum Format[]{
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

		constexpr static GLenum Type[]{
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
	}

	constexpr static GLint TextureWrapType[]{
		GL_REPEAT                // Repeat
		, GL_MIRRORED_REPEAT    // RepeatMirrored
		, GL_CLAMP_TO_BORDER    // ClampToBorder
		, GL_CLAMP_TO_EDGE        // ClampToEdge
	};

	constexpr static GLint TextureLODFilter[]{
		GL_LINEAR                // Linear
		, GL_NEAREST            // Nearest
	};

	constexpr static GLint ShaderType[]{
		GL_VERTEX_SHADER        // Vertex
		, GL_FRAGMENT_SHADER    // Fragment
		, GL_GEOMETRY_SHADER    // Geometry
		, GL_COMPUTE_SHADER        // Compute
	};

	constexpr static GLint BufferUsage[]{
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

	constexpr static GLenum States[]{
		GL_CULL_FACE      // FaceCulling
		, GL_MULTISAMPLE      // Multisampling
		, GL_SCISSOR_TEST      // ScissorTest
		, GL_STENCIL_TEST       // StencilTest
	};
	constexpr static GLenum FaceCulling[]{
		GL_BACK    //Back
		, GL_FRONT    //Front
		, GL_FRONT_AND_BACK    //FrontAndBack
	};

	constexpr static GLenum DepthTest[]{
		GL_NEVER, //Never
		GL_LESS, //Less
		GL_LEQUAL, //LessEqual
		GL_EQUAL, //Equal
		GL_GEQUAL, //GreaterEqual
		GL_GREATER, //Greater
		GL_ALWAYS //Always
	};

	constexpr static GLenum FrameBufferAttachment[]{
		GL_COLOR_ATTACHMENT0,        // Color
		GL_DEPTH_ATTACHMENT,        // Depth
		GL_STENCIL_ATTACHMENT,        // Stencil
		GL_DEPTH_STENCIL_ATTACHMENT    // DepthStencil
	};

	constexpr static GLenum ElementType[]{
		GL_FLOAT,    	//Float1
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

		GL_UNSIGNED_INT,		//UInt1
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

		0            //SamplerTypeCount
	};
}