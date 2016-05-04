#ifndef PSDefault_h__
#define PSDefault_h__

#include "PixelShader.h"

class PSDefault : public PixelShader
{
public:
	Vector4D diffuse;
	Texture2D* diffuse_map;

	PSDefault():diffuse_map(nullptr), diffuse(0)
	{

	}
	Vector4D Execute(Vector4D* input)
	{
		Vector4D result = Vector4D(0, 0, 0, 0);
		if(diffuse_map != nullptr)
		{
			tex2D(diffuse_map, &result, input[1].X, input[1].Y);
		}
		return result;
	}
};
#endif // PSDefault_h__