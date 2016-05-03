#ifndef PSFont_h__
#define PSFont_h__

#include "PixelShader.h"

class PSFont : public PixelShader
{
public:
	float appTime;
	Vector4D diffuse;
	Texture2D* diffuseTex;

	PSFont()
	{

	}
	Vector4D Execute(Vector4D* input)
	{
		Vector4D out;
		sample2D(diffuseTex, &out, input[1].X, input[1].Y);

		out = Vector4D(1,1,1,1) - out;
		out.W = out.X;
		return out;
	}
};
#endif // PSFont_h__
