#ifndef PSDefault_h__
#define PSDefault_h__

#include "PixelShader.h"

class PSDefault : public PixelShader
{
public:
	float appTime;
	Vector4D diffuse;
	PSDefault()
	{

	}
	Vector4D Execute(Vector4D* input)
	{
	/*	float f = (sinf(appTime * 4.0f) * 0.5f + 0.5f);
		Vector4D color = diffuse * (sinf((input[0].Y / 2.0f)) * 0.5f + 0.5f)* (cosf((input[0].X / 2.0f)) * 0.5f * f + 0.5f);
		color.W = 0.6f;*/
		//return color;
		float val = sinf(appTime);
		return Vector4D(val,val,val,1);
		//return Vector4D(0,0,1, (sinf(appTime * 4.0f) * 0.5f + 0.5f));
	}
};
#endif // PSDefault_h__