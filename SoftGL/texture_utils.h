#ifndef texture_utils_h__
#define texture_utils_h__

#include <memory>

#include "StaticTexture.h"
#include "Bitmap.h"

struct texture_utils {
	template<typename T>
	static void fill(Texture* tex, T pixel_value) {
		if (tex == nullptr) {
			return;
		}

		if (tex->Desc().BytesPerPixel != sizeof(T)) {
			throw std::exception("Invalid pixel value specified");
		}

		auto data = (T*)tex->LockWrite();

		for (size_t i = 0; i < tex->Desc().Width * tex->Desc().Height; ++i) {
			data[i] = pixel_value;
		}
	}

	static Texture* LoadTexture(const std::string& path) {
		FILE *filePtr; //our file pointer
		BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
		BITMAPINFOHEADER bitmapInfoHeader;

		//open filename in read binary mode
		filePtr = fopen(path.c_str(), "rb");
		if (filePtr == NULL)
			return nullptr;

		//read the bitmap file header
		int read_cnt = fread(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr);

		if (read_cnt != sizeof(BITMAPFILEHEADER)) {
			printf("EPIC FAIL!\n");
		}


		//verify that this is a bmp file by check bitmap id
		if (bitmapFileHeader.bfType != 0x4D42) {
			fclose(filePtr);
			return nullptr;
		}

		//read the bitmap info header
		fread(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), filePtr);

		//move file point to the begging of bitmap data
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		auto width = bitmapInfoHeader.biWidth;
		auto height = bitmapInfoHeader.biHeight;
		auto bpp = bitmapInfoHeader.biBitCount / 8;

		auto mips = std::make_shared<DynamicMipChain>(width, height, bpp, -1);

		auto bytes_count = width * height * bpp;

		auto stride = width * bpp;

		auto padding = stride % 4 == 0 ? 0 : (4 - (stride % 4));

		uint8_t* ptr = (uint8_t*)mips->GetPointer();

		//read in the bitmap image data
		for (size_t y = 0; y < height; ++y) {
			auto dst_ptr = ptr + stride * (height - 1 - y);
			auto result = fread(dst_ptr, 1, stride, filePtr);
			if (padding != 0) {
				fseek(filePtr, padding, SEEK_CUR);
			}
		}

		fclose(filePtr);


		TextureDescription desc;
		desc.Width = width;
		desc.Height = height;
		desc.BytesPerPixel = bpp;
		desc.MipCount = mips->MipsCount();
		desc.Depth = 1;

		return new StaticTexture<decltype(mips)>(mips, desc);
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
