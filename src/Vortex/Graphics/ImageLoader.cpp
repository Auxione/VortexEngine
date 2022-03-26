#include "Vortex/Graphics/ImageLoader.h"

#include <stb/stb_image.h>

namespace Vortex {
	namespace STB {
		//       1           grey
		//       2           grey, alpha
		//       3           red, green, blue
		//       4           red, green, blue, alpha
		ImageChannels::Enum Format[]{
			ImageChannels::Single //dummy placeholder for 0
			, ImageChannels::Single
			, ImageChannels::Double
			, ImageChannels::RGB
			, ImageChannels::RGBA
		};
	}

	bool LoadImage2D(const std::filesystem::path& image_path, ImageData& data) {
		auto file = image_path.string();
		const char* file_name = file.c_str();

		int w, h, c;
		UInt8* loaded_img_data = stbi_load(file_name, &w, &h, &c, 0);
		if (loaded_img_data != nullptr) {
			data.Size[0] = w;
			data.Size[1] = h;
			data.Channels = STB::Format[c];
			data.Pixels.insert(
				data.Pixels.begin(),
				loaded_img_data,
				loaded_img_data + w * h * c
			);

			stbi_image_free(loaded_img_data);
			VORTEX_LOG_INFO("[ImageLoader] Loaded from: %s.", file_name);
			VORTEX_LOG_DEBUG("[ImageLoader] Size: %zu Bytes, Resolution: %zux%zu, Channels: %s.", w * h * c * sizeof(UByte), data.Size[0], data.Size[1], ImageChannels::ToString[data.Channels])
			return true;
		}

		VORTEX_LOG_ERROR("[ImageLoader] Failed to load: %s.", file_name);
		return false;
	}

	bool LoadHDRImage2D(const std::filesystem::path& image_path, HDRImageData& data) {
		auto file = image_path.string();
		const char* file_name = file.c_str();

		int w, h, c;
		float* loaded_img_data = stbi_loadf(file_name, &w, &h, &c, 0);

		if (loaded_img_data != nullptr) {
			data.Size[0] = w;
			data.Size[1] = h;
			data.Channels = STB::Format[c];
			data.Pixels.insert(
				data.Pixels.begin(),
				loaded_img_data,
				loaded_img_data + w * h * c
			);

			stbi_image_free(loaded_img_data);
			VORTEX_LOG_INFO("[ImageLoader] Loaded from: %s.", file_name);
			VORTEX_LOG_DEBUG("[ImageLoader] Size: %zu Bytes, Resolution: %zux%zu, Channels: %s.", w * h * c * sizeof(float), data.Size[0], data.Size[1], ImageChannels::ToString[data.Channels])
			return true;
		}

		VORTEX_LOG_ERROR("[ImageLoader] Failed to load: %s.", file_name);
		return false;
	}
}