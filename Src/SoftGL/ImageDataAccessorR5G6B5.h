#pragma once

#include <LMath/lmath.h>
#include <cstdint>

class ImageDataAccessorR5G6B5 {
public:

	using PixelDataType = std::uint16_t;
	static constexpr bool HasAlpha = true;

	static PixelDataType ConvertColor(const lm::float4& color) {
		PixelDataType r = (uint32_t)(color.x() * 0x1f);
		PixelDataType g = (uint32_t)(color.y() * 0x3f);
		PixelDataType b = (uint32_t)(color.z() * 0x1f);

		return  (r << 11) | (g << 5) | (b);
	}

	static lm::float4 ConvertColor(PixelDataType color) {
		float r = (float)(color >> 11) / 0x1f;
		float g = (float)((color >> 5) & 0x3f) / 0x3f;
		float b = (float)((color >> 0) & 0x1f) / 0x1f;
		return lm::float4(r, g, b, 1.0f);
	}
};