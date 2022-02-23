#pragma once
#include <string>

namespace Vortex::MathToString {
	constexpr static SizeType BufferSize = 512;

	inline void Float1(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%f]", data[0]);
		str.append(buf);
	}
	inline void Float2(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%f, %f]", data[0], data[1]);
		str.append(buf);
	}
	inline void Float3(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%f, %f, %f]", data[0], data[1], data[2]);
		str.append(buf);
	}
	inline void Float4(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%f, %f, %f, %f]", data[0], data[1], data[2], data[3]);
		str.append(buf);
	}

	inline void Float2x2(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "|%f, %f |\n|%f, %f |",
				  data[0], data[2],
				  data[1], data[3]
		);
		str.append(buf);
	}
	inline void Float3x3(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "|%f, %f, %f |\n|%f, %f, %f |\n|%f, %f, %f |",
				  data[0], data[3], data[6],
				  data[1], data[4], data[7],
				  data[2], data[5], data[8]
		);
		str.append(buf);
	}
	inline void Float4x4(String& str, const float* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "|%f, %f, %f, %f |\n|%f, %f, %f, %f |\n|%f, %f, %f, %f |\n|%f, %f, %f, %f |",
				  data[0], data[4], data[8], data[12],
				  data[1], data[5], data[9], data[13],
				  data[2], data[6], data[10], data[14],
				  data[3], data[7], data[11], data[15]
		);
		str.append(buf);
	}

	inline void Int2(String& str, const std::int32_t* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%d, %d]", data[0], data[1]);
		str.append(buf);
	}
	inline void Int3(String& str, const std::int32_t* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%d, %d, %d]", data[0], data[1], data[2]);
		str.append(buf);
	}
	inline void Int4(String& str, const std::int32_t* data) {
		char buf[BufferSize];
		sprintf_s(buf, BufferSize, "[%d, %d, %d, %d]", data[0], data[1], data[2], data[3]);
		str.append(buf);
	}
}