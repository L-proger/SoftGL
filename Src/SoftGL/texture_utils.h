#ifndef texture_utils_h__
#define texture_utils_h__

#include <memory>

#include "StaticTexture.h"
#include "Bitmap.h"
//#include <intrin.h>
#include <LMath/lmath.h>

struct texture_utils {

	template<typename SrcAccessor, typename DstAccessor>
	static bool copy(Texture* src, Texture* dst) {
		auto srcDesc = src->Desc();
		auto dstDesc = dst->Desc();

		if (srcDesc.Width != dstDesc.Width || (srcDesc.Height != dstDesc.Height)) {
			return false;
		}


		auto* srcPtr = reinterpret_cast<const typename SrcAccessor::PixelDataType*>(src->LockRead());
		auto* dstPtr = reinterpret_cast<typename DstAccessor::PixelDataType*>(dst->LockWrite());

		for (size_t y = 0; y < srcDesc.Height; ++y) {
			for (size_t x = 0; x < srcDesc.Width; ++x) {
				lm::float4 srcDataf = SrcAccessor::ConvertColor(srcPtr[x]);
				auto dstColor = DstAccessor::ConvertColor(srcDataf);
				dstPtr[x] = dstColor;
			}
			srcPtr += srcDesc.Width;
			dstPtr += srcDesc.Width;
		}

		return true;
	}


	template<typename T>
	static bool fill(Texture* tex, T pixel_value) {
		if (tex == nullptr) {
			return false;
		}

		if (tex->Desc().BytesPerPixel != sizeof(T)) {
			//throw std::exception("Invalid pixel value specified");
			return false;
		}

		auto data = (T*)tex->LockWrite();

		auto memsize = tex->Desc().Width * tex->Desc().Height * sizeof(pixel_value);

		if((8 % (sizeof(T))) == 0 && (memsize % 8) == 0)
		{
			alignas(8) uint64_t block_value;
			T* blockPtr = (T*)&block_value;
			for (size_t i = 0; i < (8 / sizeof(T)); ++i) {
				blockPtr[i] = pixel_value;
			}

			auto blockMem = (uint64_t*)data;
			for(size_t i = 0; i < (memsize / sizeof(block_value)); ++i)
			{
				blockMem[i] = block_value;
			}
		}

		/*__m256i avxBlock;

		if ((sizeof(avxBlock) % (sizeof(T))) == 0 && (memsize % sizeof(avxBlock)) == 0) {
			T* blockPtr = (T*)&avxBlock;
			for (size_t i = 0; i < (sizeof(avxBlock) / sizeof(T)); ++i) {
				blockPtr[i] = pixel_value;
			}

			auto blockMem = (decltype(avxBlock)*)data;
			if(((size_t)blockMem % sizeof(avxBlock)) != 0)
			{
				throw std::exception("Memory is not SSE aligned");
			}
			const auto cnt = (memsize / sizeof(avxBlock));
			const auto last = blockMem + cnt;
			while(blockMem != last){
		
				_mm256_store_si256(blockMem++, avxBlock);
				//_mm_store_si128(blockMem++, avxBlock);
			}
		}*/
		 

		//generic assign
		/*for (size_t i = 0; i < tex->Desc().Width * tex->Desc().Height; ++i) {
			data[i] = pixel_value;
		}*/
		return true;
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
