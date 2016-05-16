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
