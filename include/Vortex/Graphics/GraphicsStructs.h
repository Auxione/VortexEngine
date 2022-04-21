#pragma once

#include "Vortex/Common/HandleMap.h"
#include "Vortex/Common/HashedString.h"

#include "Vortex/Math/BasicVector.h"

#include "Vortex/Graphics/AbstractData.h"

namespace Vortex::Graphics {
	using Vector2HalfInt = Vortex::Math::BasicVector<UInt16, 2>;
	using Vector3HalfInt = Vortex::Math::BasicVector<UInt16, 3>;

	using HandleType = Vortex::BasicHandle<Vortex::UInt16, 10, 4, 2>;
	using Handle = HandleType::UnderlyingType;
	using AdditionalData = Graphics::AbstractData<4>;

	class Renderer;

	union Resolution {
		Int32 Data[3];
		struct {
			Int32 Width;
			Int32 Height;
			Int32 RefreshRate;
		};
	};

	struct Window {
		String Title;
		Resolution Resolution;
		//MonitorHandle Monitor;
		Handle DefaultViewHandle;

		AdditionalData AdditionalData;
	};

	struct Texture {
		Graphics::PixelFormat::Enum PixelFormat;

		Graphics::TextureLODFilter::Enum MinificationFilter;
		Graphics::TextureLODFilter::Enum MagnificationFilter;
		Graphics::TextureWrap::Enum WrapS;
		Graphics::TextureWrap::Enum WrapT;
		Graphics::TextureWrap::Enum WrapR;

		Vector3HalfInt Size;
		UInt16 LayerCount;
		UInt8 BitsPerPixel;
		bool IsCubemap;
		bool Mipmapped;

		SizeType DataSize;

		AdditionalData AdditionalData;
	};

	struct Buffer {
		Graphics::BufferUsage::Enum BufferUsage;
		Graphics::BufferLayout Layout;
		bool Mutable;
		SizeType Size;

		AdditionalData AdditionalData;
	};

	struct Shader {
		std::unordered_map<HashedString, int> Locations;
		std::unordered_map<HashedString, Graphics::ElementType::Enum> Types;
		std::unordered_map<HashedString, int> BindingPositions;
		ShaderTags::Enum Tags;

		AdditionalData AdditionalData;
	};

	struct FrameBuffer {
		Vector2HalfInt Size;
		std::vector<Handle> TextureHandles;
		AdditionalData AdditionalData;
	};

	struct Mesh {
		Handle IndexBufferHandle;
		std::vector<Handle> BufferHandles;

		SizeType IndexCount;
		SizeType IndexCapacity;

		Graphics::Topology::Enum Topology;

		AdditionalData AdditionalData;

#ifdef VORTEX_DEBUG
		std::vector<SizeType> d_BufferSizes;
#endif
	};

	struct DrawSurface {
		Math::RectangleInt Area;
		Math::Color ClearColor;
		float ClearDepth;
		UInt16 ClearStencil;
	};

	struct View {
		Math::RectangleInt Viewport;
		Math::Matrix4 ProjectionMatrix;
		Math::Matrix4 ViewMatrix;
		Graphics::DepthTesting::Enum DepthTest;

		Handle FramebufferHandle;
	};

	struct ComputeShader;
	using OnComputeShaderBindFn = void (*)(const Renderer&, const ComputeShader&);
	struct ComputeShader {
		Handle ShaderHandle;

		OnComputeShaderBindFn OnBind;
		Graphics::AbstractData<256> Data;
	};

	struct Material;
	using OnMaterialBindFn = void (*)(const Renderer&, const Material&);
	struct Material {
		Handle ShaderHandle;
		OnMaterialBindFn OnBind;
		Graphics::Blending::Enum Blending;

		Graphics::AbstractData<256> Data;
	};

	struct PostProcess;
	using OnPostProcessBindFn = void (*)(const PostProcess&);
	struct PostProcess {
		OnPostProcessBindFn OnBind;

		Graphics::AbstractData<256> Data;
	};

	using Map = HandleType::Map<
		Window,

		Texture,
		Buffer,
		Shader,
		FrameBuffer,

		ComputeShader,
		Mesh,
		Material,
		DrawSurface,
		View,

		PostProcess
	>;
}