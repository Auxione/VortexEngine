#pragma once

namespace Vortex::Math {
	struct Color {
		using UnderlyingType = float;
		constexpr static SizeType Length() { return 4; }

	public:
		constexpr void Set(float r, float g, float b, float a) {
			Data[0] = r;
			Data[1] = g;
			Data[2] = b;
			Data[3] = a;
		}
		constexpr float Grayscale() const { return (r + g + b) / 3.0f; }

	public:
		constexpr static Color FromHexColor(UInt32 hex_color) {
			return Color{
				static_cast<float>((hex_color >> 24) & 0xFF) / 255.0f
				, static_cast<float>((hex_color >> 16) & 0xFF) / 255.0f
				, static_cast<float>((hex_color >> 8) & 0xFF) / 255.0f
				, static_cast<float>(hex_color & 0xFF) / 255.0f
			};
		}

		constexpr static UInt32 ToHexColor(const Color& color) {
			UInt32 attribute = static_cast<UInt32>(color.r * 255.0f) << 24;
			attribute += static_cast<UInt32>(color.g * 255.0f) << 16;
			attribute += static_cast<UInt32>(color.b * 255.0f) << 8;
			attribute += static_cast<UInt32>(color.a * 255.0f);
			return attribute;
		}

	public:
		union {
			float Data[4];
			struct { float r; float g; float b; float a; };
		};
	};

	namespace Colors {
		enum Enum {
			Black = 0,
			Gray,
			White,

			Blue,
			Cyan,
			Green,
			Yellow,
			Red,
			Orange,
			Magenta,

			VortexCustom,
			Clear,

			Count
		};
		constexpr static Color Values[]{
			Color{0.0f, 0.0f, 0.0f, 1.0f}          // Black
			, Color{0.5f, 0.5f, 0.5f, 1.0f}        // Gray
			, Color{1.0f, 1.0f, 1.0f, 1.0f}        // White

			, Color{0.0f, 0.0f, 1.0f, 1.0f}        // Blue
			, Color{0.0f, 1.0f, 1.0f, 1.0f}        // Cyan
			, Color{0.0f, 1.0f, 0.0f, 1.0f}        // Green
			, Color{1.0f, 1.0f, 0.0f, 1.0f}        // Yellow
			, Color{1.0f, 0.0f, 0.0f, 1.0f}        // Red
			, Color{1.0f, 0.5f, 0.0f, 1.0f}        // Orange
			, Color{1.0f, 0.0f, 1.0f, 1.0f}        // Magenta

			, Color{
				0.58431372549019607843137254901961f
				, 0.68627450980392156862745098039216f
				, 0.75294117647058823529411764705882f
				, 1.0f
			}    // VortexCustom
			, Color{0.0f, 0.0f, 0.0f, 0.0f}        // Clear
		};
	}
}