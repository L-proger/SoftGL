#ifndef Texture_h__
#define Texture_h__

#include "dynamic_buffer.h"
#include "LString.h"
#include "Bitmap.h"
#include <stdio.h>
#include "mip_utils.h"

struct Color24{
	uint8_t b;
	uint8_t g;
	uint8_t r;

	void fromColor16(uint16_t value){
		b = (value & 0x1f) << 3;
		g = (value & 0x7e0) >> 3;
		r = (value & 0xf800) >> 8;
	}

	uint16_t toColor16(){
		uint16_t result = b >> 3;
		result |= ((uint16_t)g >> 2) << 5;
		result |= ((uint16_t)r >> 3) << 11;
		return result;
	}

	static Color24 lerp(Color24 left, Color24 right, uint16_t val, uint16_t maxVal){
		Color24 result;
		result.r = (int32_t)left.r + ((int32_t)(right.r - left.r) * (int32_t)val) / (int32_t)maxVal;
		result.g = (int32_t)left.g + ((int32_t)(right.g - left.g) * (int32_t)val) / (int32_t)maxVal;
		result.b = (int32_t)left.b + ((int32_t)(right.b - left.b) * (int32_t)val) / (int32_t)maxVal;

		return result;
	}

};

struct TextureDescription {
	size_t Width;
	size_t Height;
	size_t Depth;
	size_t BytesPerPixel;
	size_t MipCount;
};

struct Texture {
	Texture(const TextureDescription& description) : _description(description) {
		_depthStride = 0;
		size_t width = _description.Width;
		size_t height = _description.Height;
		for (size_t i = 0; i < _description.MipCount; ++i) {
			_depthStride += _description.BytesPerPixel * width * height;
			width = MipUtils::SmallerMipSize(width);
			height = MipUtils::SmallerMipSize(height);
		}
	}
	virtual void* LockWrite() = 0;
	virtual const void* LockRead() = 0;
	virtual ~Texture() = default;
	const TextureDescription& Desc() const {
		return _description;
	}
private:
	size_t _depthStride;
	TextureDescription _description;
};

#endif // Texture_h__
