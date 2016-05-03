#ifndef BITMAP_H
#define BITMAP_H

#include "Types.h"

/*struct BITMAPFILEHEADER {
  int16 bfType;
  int32 bfSize;
  int16 Reserved1;
  int16 Reserved2;
  int32 bfOffBits;
};*/

#if defined(_MSC_VER)
#include <Windows.h>
#else
#pragma pack(push,2)
typedef struct tagBITMAPFILEHEADER {
    unsigned short    bfType;
    unsigned long   bfSize;
    unsigned short    bfReserved1;
    unsigned short    bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER, *PBITMAPFILEHEADER;
#pragma pack(pop)


#pragma pack(push,2)
typedef struct tagBITMAPINFOHEADER {
  unsigned long biSize;
  long  biWidth;
  long  biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned long biCompression;
  unsigned long biSizeImage;
  long  biXPelsPerMeter;
  long  biYPelsPerMeter;
  unsigned long biClrUsed;
  unsigned long biClrImportant;
}BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack(pop)
#endif

#endif // BITMAP_H
