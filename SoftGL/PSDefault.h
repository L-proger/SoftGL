#ifndef PSDefault_h__
#define PSDefault_h__

#include "PixelShader.h"

class PSDefault : public PixelShader
{
public:
	float4 diffuse;
	Texture2D* diffuse_map;

	PSDefault():diffuse_map(nullptr), diffuse(0)
	{

	}
	float4 Execute(float4* input)
	{
		float4 result = float4(0, 0, 0, 0);
		if(diffuse_map != nullptr)
		{
			tex2D(diffuse_map, &result, input[1].x, input[1].y);
		}
		return result;
	}
};
#endif // PSDefault_h__