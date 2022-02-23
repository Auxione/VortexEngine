#pragma once
#include "Vortex/Graphics/RenderBackend.h"
#include "Vortex/Containers/Vector.h"
#include "Vortex/Memory/Memory.h"

#include <filesystem>

namespace Vortex {
	namespace ImageChannels {
		enum Enum {
			Single = 0,
			Double,
			RGB,
			RGBA,
		};
		constexpr const char* ToString[]{
			"Single"
			, "Double"
			, "RGB"
			, "RGBA"
		};
		constexpr int Count[]{
			1
			, 2
			, 3
			, 4
		};
	}

	struct ImageData {
		Vector<UInt8> Data;
		SizeType Size[2];
		ImageChannels::Enum Channels;
	};
	bool LoadImage2D(const std::filesystem::path& image_path, ImageData& data);

	struct HDRImageData {
		Vector<float> Data;
		SizeType Size[2];
		ImageChannels::Enum Channels;
	};
	bool LoadHDRImage2D(const std::filesystem::path& image_path, HDRImageData& data);
}