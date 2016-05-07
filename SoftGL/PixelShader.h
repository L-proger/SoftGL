#ifndef PixelShader_h__
#define PixelShader_h__

#include "IShader.h"
#include "Texture2D.h"
#include <cstdint>
#include "lmath.h"

using namespace lm;

enum class TextureFilter
{
	Point,
	Bilinear,
	Trilinear,
	Anisotropic
};

#define CLIP -1231345391
class PixelShader : public IShader
{
public:
	virtual ~PixelShader()
	{

	}
	virtual float4 Execute(float4* input) = 0;
	void tex2D(Texture2D* tex, float4* color, float tx, float ty, TextureFilter filter)
	{
		auto tw = (float)(tex->width - 1);
		auto th = (float)(tex->height - 1);

	

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
		if(filter == TextureFilter::Bilinear)
		{
			float x = (tx * tw);
			float y = (ty * th);
			int px = (int)x; 
			int py = (int)y;

			float fx = x - px;
			float fy = y - py;
			float fx1 = 1.0f - fx;
			float fy1 = 1.0f - fy;

			int w1 = fx1 * fy1 * 256.0f;
			int w2 = fx  * fy1 * 256.0f;
			int w3 = fx1 * fy  * 256.0f;
			int w4 = fx  * fy  * 256.0f;

			auto mem = reinterpret_cast<uint8_t*>(tex->getBuffer()->get_pointer());

			uint8_t* p1 = mem + ((py + 0)*tex->width + (px + 0))* tex->bpp;
			uint8_t* p2 = mem + ((py + 0)*tex->width + (px + 1))* tex->bpp;
			uint8_t* p3 = mem + ((py + 1)*tex->width + (px + 0))* tex->bpp;
			uint8_t* p4 = mem + ((py + 1)*tex->width + (px + 1))* tex->bpp;

			int outr = p1[0] * w1 + p2[0] * w2 + p3[0] * w3 + p4[0] * w4;
			int outg = p1[1] * w1 + p2[1] * w2 + p3[1] * w3 + p4[1] * w4;
			int outb = p1[2] * w1 + p2[2] * w2 + p3[2] * w3 + p4[2] * w4;
			
			color->z = ((float)(outr >> 8)) / 255.0f;
			color->y = ((float)(outg >> 8)) / 255.0f;
			color->x = ((float)(outb >> 8)) / 255.0f;
			color->w = 1.0f;

		}else{
			tx += 0.5f / tw;
			ty += 0.5f / th;
			int x = (int)(tx * tw);
			int y = (int)(ty * th);
			uint8_t* ptr = reinterpret_cast<uint8_t*>(tex->getBuffer()->get_pointer());
			ptr += (y*tex->width + x)* tex->bpp;
			color->z = ((float)ptr[0]) / 255.0f;
			color->y = ((float)ptr[1]) / 255.0f;
			color->x = ((float)ptr[2]) / 255.0f;
			color->w = 1.0f;
		}
		
	}
};
#endif // PixelShader_h__
