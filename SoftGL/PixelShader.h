#ifndef PixelShader_h__
#define PixelShader_h__

#include "IShader.h"
#include "Texture2D.h"
#include <cstdint>
#include "lmath.h"

using namespace lm;

#define CLIP -1231345391
class PixelShader : public IShader
{
public:
	virtual ~PixelShader()
	{

	}
	virtual float4 Execute(float4* input) = 0;
	void tex2D(Texture2D* tex, float4* color, float tx, float ty)
	{
		if(tx > 1.0f){
			tx = 1.0f;
		}
		if (tx < 0.0f) {
			tx = 0.0f;
		}
		if (ty > 1.0f) {
			ty = 1.0f;
		}
		if (ty < 0.0f) {
			ty = 0.0f;
		}
		int x = (int)(tx * (float)tex->width);
		int y = (int)((1.0f - ty) * (float)tex->height);
        uint8_t* ptr = reinterpret_cast<uint8_t*>(tex->getBuffer()->get_pointer());
		ptr+=(y*tex->width + x)* tex->bpp;
		color->z = ((float)ptr[0]) / 255.0f;
		color->y = ((float)ptr[1]) / 255.0f;
		color->x = ((float)ptr[2]) / 255.0f;
		color->w = 1.0f;
	}
};
#endif // PixelShader_h__
