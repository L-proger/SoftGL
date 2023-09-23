#ifndef PSFont_h__
#define PSFont_h__

#include "PixelShader.h"

class PSFont : public PixelShader
{
public:
	float appTime;
	float4 diffuse;
	Texture2D* diffuseTex;

	PSFont()
	{

	}
	float4 Execute(float4* input)
	{
		float4 out;

		tex2D(diffuseTex, &out, input[1].X, input[1].Y);

		out = float4(1,1,1,1) - out;
		out.W = out.X;
		return out;
	}
};
#endif // PSFont_h__
