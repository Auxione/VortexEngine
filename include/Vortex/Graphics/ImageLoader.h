#pragma once
#include "Vortex/Containers/Array.h"
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
		Array<UInt8> Pixels;
		UInt16 Size[2];
		ImageChannels::Enum Channels;
	};
	bool LoadImage2D(const std::filesystem::path& image_path, ImageData& data);

	struct HDRImageData {
		Array<float> Pixels;
		UInt16 Size[2];
		ImageChannels::Enum Channels;
	};
	bool LoadHDRImage2D(const std::filesystem::path& image_path, HDRImageData& data);
}