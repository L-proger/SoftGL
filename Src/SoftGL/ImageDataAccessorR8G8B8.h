
#pragma once

#include <LMath/lmath.h>
#include <cstdint>

class ImageDataAccessorR8G8B8 {
public:

	using PixelDataType = std::uint32_t;
	static constexpr bool HasAlpha = true;

	static uint32_t ConvertColor(const lm::float4& color) {
		uint32_t r = (uint32_t)(color.x() * 255.0f);
		uint32_t g = (uint32_t)(color.y() * 255.0f);
		uint32_t b = (uint32_t)(color.z() * 255.0f);
		uint32_t a = (uint32_t)(color.w() * 255.0f);

		return (a << 24) | (r << 16) | (g << 8) | (b);
	}

	static lm::float4 ConvertColor(uint32_t color) {
		float d = 1.0f / 255.0f;

		float a = (float)((color >> 24) & 0xff) * d;
		float r = (float)((color >> 16) & 0xff) * d;
		float g = (float)((color >> 8) & 0xff) * d;
		float b = (float)((color >> 0) & 0xff) * d;
		return lm::float4(r, g, b, a);
	}
};