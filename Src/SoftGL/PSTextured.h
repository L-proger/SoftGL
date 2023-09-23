#ifndef PSTextured_h__
#define PSTextured_h__

#include "PixelShader.h"

class PSTextured : public PixelShader
{
public:
	float appTime;
	float4 diffuse;
	Texture2D* diffuseTex;

	PSTextured()
	{

	}
	float4 Execute(float4* input)
	{
	/*	float f = (sinf(appTime * 4.0f) * 0.5f + 0.5f);
		float4 color = diffuse * (sinf((input[0].Y / 2.0f)) * 0.5f + 0.5f)* (cosf((input[0].X / 2.0f)) * 0.5f * f + 0.5f);
		color.W = 0.6f;*/
		//return color;
		//return diffuse;
		float4 out;
		sample2D(diffuseTex, &out, input[1].X, input[1].Y);

		//out = float4(1,1,1,1) - out;
		out.W = 0.5f;
		//return float4(1,1,1,1);
		return out;



		//return float4(1,1,0,1);
	}
};
#endif // PSTextured_h__
