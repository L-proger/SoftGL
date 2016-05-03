#ifndef PixelShader_h__
#define PixelShader_h__

#include "IShader.h"
#include "Texture2D.h"
#include "Types.h"

#define CLIP -1231345391
class PixelShader : public IShader
{
public:
	virtual ~PixelShader()
	{

	}
	virtual Vector4D Execute(Vector4D* input) = 0;
	void sample2D(Texture2D* tex, Vector4D* color, float tx, float ty)
	{
		int x = (int)(tx * tex->fwidth);
		int y = (int)((1.0f - ty) * tex->fheight);
        uint8* ptr = reinterpret_cast<uint8*>(tex->getBuffer()->getDataPtr());
		ptr+=(y*tex->width + x)* tex->bpp;
		color->Z = ((float)ptr[0]) / 255.0f;
		color->Y = ((float)ptr[1]) / 255.0f;
		color->X = ((float)ptr[2]) / 255.0f;
		color->W = 1.0f;
	}
};
#endif // PixelShader_h__
