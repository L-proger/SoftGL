#ifndef texture_utils_h__
#define texture_utils_h__

struct texture_utils {
	template<typename T>
	static void fill(Texture2D* tex, T pixel_value) {
		if (tex == nullptr) {
			return;
		}

		if (tex->bpp != sizeof(T)) {
			throw std::exception("Invalid pixel value specified");
		}

		auto data = (T*)tex->getBuffer()->GetPointer();

		for (size_t i = 0; i < tex->width * tex->height; ++i) {
			data[i] = pixel_value;
		}
	}


	/*auto bb = rasterizer.GetBackBuffer();
	uint32_t* pixels = (uint32_t*)bb->getBuffer()->getDataPtr();
	for (size_t y = 0; y < bb->height; ++y) {
	for (size_t x = 0; x < bb->width; ++x) {
	uint32_t xVal = (x * 0xff) / bb->width;
	uint32_t yVal = (y * 0xff) / bb->height;
	pixels[y * bb->width + x] = ((xVal & 0xff) << 16) | ((yVal & 0xff) << 8);
	}
	}*/
};
#endif // texture_utils_h__
