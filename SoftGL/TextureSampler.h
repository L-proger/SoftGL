#ifndef TEXTURESAMPLER_H
#define TEXTURESAMPLER_H

#include "Inline.h"
#include "Texture2D.h"

static INLINE uint32 sample2D(Texture2D* tex, float tX, float tY)
{
	if(tex == nullptr)
	{
		return 0;
	}
    tX = clampFloat(tX, 0.0f, 1.0f);
    tY = clampFloat(tY, 0.0f, 1.0f);

    tY = 1.0f - tY;

    int x = static_cast<int>((tX * (tex->fwidth - 1.0f)));
    int y = static_cast<int>((tY * (tex->fheight - 1.0f)));
    uint8* ptr = reinterpret_cast<uint8*>(tex->getBuffer()->get_pointer());


    float pX = tX * tex->fwidth;
    float pY = tY * tex->fheight;

    uint32 frX = (uint32)((pX - (float)((int)pX))* 255.0f);
    uint32 frY = (uint32)((pY - (float)((int)pY))* 255.0f);

    uint32* ptr2 = reinterpret_cast<uint32*>(tex->getBuffer()->get_pointer());

    uint32 color;
  //  if(tex->bpp < 4)
  //  {
        uint32 tl = 0xff000000;
   /*     uint32 tr = 0xff000000;
        uint32 rl = 0xff000000;
        uint32 rr = 0xff000000;*/

        memcpy(&tl, ptr+ (y*tex->width + x)* tex->bpp, tex->bpp);
        //memcpy(&tr, ptr+ (y*tex->width + x + 1)* tex->bpp, 3);
    //	memcpy(&rl, ptr+ ((y + 1)*tex->width + x)* tex->bpp, 3);
        //memcpy(&rr, ptr+ ((y + 1)*tex->width + x + 1)* tex->bpp, 3);

        color = tl;// blendrgb ( blendrgb ( tl, tr, frX), blendrgb ( rl, rr, frX), frY);

   /* }
    else
    {

        int32 x1 = x;
        int32 x2 = x + 1;
        int32 x3 = x;
        int32 x4 = x + 1;

        int32 y1 = y;
        int32 y2 = y;
        int32 y3 = (y + 1);
        int32 y4 = (y + 1);

        x1 = clampInt(x1, 0, tex->width - 1);
        x2 = clampInt(x2, 0, tex->width - 1);
        x3 = clampInt(x3, 0, tex->width - 1);
        x4 = clampInt(x4, 0, tex->width - 1);


        y1 = clampInt(y1, 0, tex->height - 1);
        y2 = clampInt(y2, 0, tex->height - 1);
        y3 = clampInt(y3, 0, tex->height - 1);
        y4 = clampInt(y4, 0, tex->height - 1);




        uint32 tl = ptr2[y1 * tex->width + x1];
        uint32 tr = ptr2[y2 * tex->width + x2];
        uint32 rl = ptr2[y3 * tex->width + x3];
        uint32 rr = ptr2[y4 * tex->width + x4];

        color = blendrgb ( blendrgb ( tl, tr, frX), blendrgb ( rl, rr, frX), frY);
    }*/

    return color;
}

#endif // TEXTURESAMPLER_H
