#ifndef Texture2D_h__
#define Texture2D_h__

#include "dynamic_buffer.h"
#include "LString.h"
#include "Bitmap.h"
#include <stdio.h>

struct Color24{
	uint8_t b;
	uint8_t g;
	uint8_t r;
};

class Texture2D {
public:
	size_t width;
	size_t height;
	size_t bpp;

	Texture2D(size_t _width, size_t _height, int _bpp) : width(_width), height(_height), bpp(_bpp), data(_width * _height * _bpp){
	}

	Texture2D(String path){
		FILE *filePtr; //our file pointer
		BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
		BITMAPINFOHEADER bitmapInfoHeader;

		//open filename in read binary mode
		filePtr = fopen(path.c_str(),"rb");
		if (filePtr == NULL)
			return;

		//read the bitmap file header
        int read_cnt = fread(&bitmapFileHeader, 1,sizeof(BITMAPFILEHEADER),filePtr);

        if(read_cnt != sizeof(BITMAPFILEHEADER)) {
            printf("EPIC FAIL!\n");
        }


		//verify that this is a bmp file by check bitmap id
		if (bitmapFileHeader.bfType !=0x4D42)
		{
			fclose(filePtr);
			return;
		}

		//read the bitmap info header
        fread(&bitmapInfoHeader, 1,sizeof(BITMAPINFOHEADER),filePtr);

		//move file point to the begging of bitmap data
		fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

		width = bitmapInfoHeader.biWidth;
		height = bitmapInfoHeader.biHeight;
		bpp = bitmapInfoHeader.biBitCount / 8;

		auto bytes_count = width * height * bpp;
		data = std::move(dynamic_buffer<uint8_t>(bytes_count));

		auto stride = width * bpp;
		auto padding = stride % 4 == 0 ? 0 : (4 - (stride % 4));
		uint8_t* ptr = (uint8_t*)data.GetPointer();

		//read in the bitmap image data
		for(size_t y = 0; y < height; ++y){
			auto dst_ptr = ptr + stride * (height - 1 - y);
			auto result = fread(dst_ptr, 1, stride, filePtr);
			if(padding != 0){
				fseek(filePtr, padding, SEEK_CUR);
			}
		}

		fclose(filePtr);
	}
	~Texture2D()
	{

	}
	Buffer* getBuffer()
	{
		return &data;
	}
private:
	dynamic_buffer<uint8_t> data;
};
#endif // Texture2D_h__
