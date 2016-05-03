#ifndef Texture2D_h__
#define Texture2D_h__
#include "Buffer.h"
#include "LString.h"
#include "Bitmap.h"
#include <stdio.h>

class Texture2D
{
private:
	Buffer* data;
public:
	size_t width;
	size_t height;
	size_t bpp;
	float fwidth;
	float fheight;

	Texture2D(int _width, int _height, int _bpp)
	{
		width = _width;
		height = _height;
		bpp = _bpp;

		data = new Buffer(width * height * bpp);
	}
	Texture2D(String path)
	{
		FILE *filePtr; //our file pointer
		BITMAPFILEHEADER bitmapFileHeader; //our bitmap file header
		BITMAPINFOHEADER bitmapInfoHeader;

		//open filename in read binary mode
		filePtr = fopen(path.c_str(),"rb");
		if (filePtr == NULL)
			return;

		//read the bitmap file header
        int read_cnt = fread(&bitmapFileHeader, 1,sizeof(BITMAPFILEHEADER),filePtr);

        if(read_cnt != sizeof(BITMAPFILEHEADER))
        {
            printf("EPIC FAIL!\n");
        }

        printf("Size = %i\n", sizeof(BITMAPFILEHEADER));


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

		//allocate enough memory for the bitmap image data
		//if(data)
//			delete data;

		data = new Buffer(bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * bitmapInfoHeader.biBitCount / 8);

		//read in the bitmap image data
		fread(data->getDataPtr(),bitmapInfoHeader.biWidth * bitmapInfoHeader.biHeight * bitmapInfoHeader.biBitCount / 8, 1,filePtr);

		width = bitmapInfoHeader.biWidth;
		height = bitmapInfoHeader.biHeight;
		fwidth = (float)width;
		fheight = (float)height;
		bpp = bitmapInfoHeader.biBitCount / 8;

		fclose(filePtr);
	}
	~Texture2D()
	{

	}
	Buffer* getBuffer()
	{
		return data;
	}
};
#endif // Texture2D_h__
