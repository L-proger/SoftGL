#ifndef Texture2D_h__
#define Texture2D_h__

#include "dynamic_buffer.h"
#include "LString.h"
#include "Bitmap.h"
#include <stdio.h>

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

		data = std::move(dynamic_buffer<uint8_t>(bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * bitmapInfoHeader.biBitCount / 8));

		//read in the bitmap image data
		fread(data.get_pointer(),bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * bitmapInfoHeader.biBitCount / 8, 1, filePtr);

		width = bitmapInfoHeader.biWidth;
		height = bitmapInfoHeader.biHeight;
		bpp = bitmapInfoHeader.biBitCount / 8;

		fclose(filePtr);
	}
	~Texture2D()
	{

	}
	buffer* getBuffer()
	{
		return &data;
	}
private:
	dynamic_buffer<uint8_t> data;
};
#endif // Texture2D_h__
