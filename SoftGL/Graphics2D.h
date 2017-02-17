#ifndef Graphics2D_h__
#define Graphics2D_h__

#include "Texture.h"


struct TextureWriter {
	Texture* texture;

	size_t width() {
		return texture->Desc().Width;
	}
	size_t height() {
		return texture->Desc().Height;
	}
	template<typename Color>
	void setPixel(int32_t x, int32_t y, Color color) {
		if (x < 0 || y < 0) {
			return;
		}
		if (x >= width() || y >= height()) {
			return;
		}
		uint8_t* data = ((uint8_t*)texture->LockWrite()) + (y * width() + x) * sizeof(color);
		memcpy(data, &color, sizeof(color));
		
	}
};

template<typename PixelWriter, typename Color>
void clear(PixelWriter& writer, const Color& color) {
	for (int y = 0; y < writer.height(); ++y) {
		for (int x = 0; x < writer.width(); ++x) {
			writer.setPixel(x, y, color);
		}
	}
}

template<typename PixelWriter, typename Color>
void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1, PixelWriter& writer, const Color& color) {
		int32_t steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep) {
			std::swap(x0, y0);
			std::swap(x1, y1);
		}

		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		int32_t dx = x1 - x0;
		int32_t dy = abs(y1 - y0);

		auto err = dx / 2;
		int32_t ystep;

		if (y0 < y1) {
			ystep = 1;
		}
		else {
			ystep = -1;
		}

		for (; x0 <= x1; x0++) {
			if (steep) {
				writer.setPixel(y0, x0, color);
			}
			else {
				writer.setPixel(x0, y0, color);
			}
			err -= dy;
			if (err < 0) {
				y0 += ystep;
				err += dx;
			}
		}
}


#endif // Graphics2D_h__
