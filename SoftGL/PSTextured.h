#ifndef PSTextured_h__
#define PSTextured_h__

#include "PixelShader.h"

class PSTextured : public PixelShader
{
public:
	float appTime;
	Vector4D diffuse;
	Texture2D* diffuseTex;

	PSTextured()
	{

	}
	Vector4D Execute(Vector4D* input)
	{
	/*	float f = (sinf(appTime * 4.0f) * 0.5f + 0.5f);
		Vector4D color = diffuse * (sinf((input[0].Y / 2.0f)) * 0.5f + 0.5f)* (cosf((input[0].X / 2.0f)) * 0.5f * f + 0.5f);
		color.W = 0.6f;*/
		//return color;
		//return diffuse;
		Vector4D out;
		sample2D(diffuseTex, &out, input[1].X, input[1].Y);

		//out = Vector4D(1,1,1,1) - out;
		out.W = 0.5f;
		//return Vector4D(1,1,1,1);
		return out;



		//return Vector4D(1,1,0,1);
	}
};
#endif // PSTextured_h__
