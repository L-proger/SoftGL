#ifndef PixelShader_h__
#define PixelShader_h__

#include "IShader.h"
#include "Texture.h"
#include <cstdint>
#include <LMath/lmath.h>

using namespace lm;

enum class TextureFilter {
	Point,
	Bilinear,
	Trilinear,
	Anisotropic
};

#define CLIP -1231345391
class PixelShader : public IShader {
public:
	virtual ~PixelShader() {

	}
	virtual float4 Execute(float4* input) = 0;
	float4 tex2D(Texture* tex, float tx, float ty, TextureFilter filter) {
		if (tex == nullptr) {
			return float4(0, 0, 0, 0);
		}
		auto width = tex->Desc().Width;
		auto height = tex->Desc().Height;
		auto bpp = tex->Desc().BytesPerPixel;

		auto tw = (float)(width - 1);
		auto th = (float)(height - 1);

		auto mips = tex->LockRead();

		if (tx > 1.0f) {
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
		if (filter == TextureFilter::Bilinear) {
			float x = (tx * tw);
			float y = (ty * th);
			int px = (int)x;
			int py = (int)y;

			float fx = x - px;
			float fy = y - py;
			float fx1 = 1.0f - fx;
			float fy1 = 1.0f - fy;

			int w1 = (int)(fx1 * fy1 * 256.0f);
			int w2 = (int)(fx  * fy1 * 256.0f);
			int w3 = (int)(fx1 * fy  * 256.0f);
			int w4 = (int)(fx  * fy  * 256.0f);

			auto mem = reinterpret_cast<const uint8_t*>(mips);

			auto p1 = mem + ((py + 0)*width + (px + 0))* bpp;
			auto p2 = mem + ((py + 0)*width + (px + 1))* bpp;
			auto p3 = mem + ((py + 1)*width + (px + 0))* bpp;
			auto p4 = mem + ((py + 1)*width + (px + 1))* bpp;

			int outr = p1[0] * w1 + p2[0] * w2 + p3[0] * w3 + p4[0] * w4;
			int outg = p1[1] * w1 + p2[1] * w2 + p3[1] * w3 + p4[1] * w4;
			int outb = p1[2] * w1 + p2[2] * w2 + p3[2] * w3 + p4[2] * w4;

			return float4(
				((float)(outr >> 8)) / 255.0f,
				((float)(outg >> 8)) / 255.0f,
				((float)(outb >> 8)) / 255.0f,
				1.0f);

		} else {
			tx += 0.5f / tw;
			ty += 0.5f / th;
			int x = (int)(tx * tw);
			int y = (int)(ty * th);
			auto ptr = reinterpret_cast<const uint8_t*>(mips);
			ptr += (y*width + x)* bpp;
			return float4(
				((float)ptr[0]) / 255.0f,
				((float)ptr[1]) / 255.0f,
				((float)ptr[2]) / 255.0f,
				1.0f);
		}
	}
};
#endif // PixelShader_h__
