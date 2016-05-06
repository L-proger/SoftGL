#ifndef VSDefault_h__
#define VSDefault_h__

#include "VertexShader.h"

class VSDefault : public VertexShader
{
public:
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;

	VSDefault()
	{
		//declare shader inputs
        inputs.push_back(String("POSITION"));
        inputs.push_back(String("TEXCOORD"));
	}
	int Execute(float4** input, float4* output)
	{
		float4x4 wv = lm::mul(mWorld, mView);
		float4x4 wvp = lm::mul(wv, mProj);

		float4 position = (*input)[0];
		position.w = 1.0f;

		position = lm::mul(wvp, position);
		output[0] = position;
		output[1] = (*input)[1];

		return 2;
	}
};
#endif // VSDefault_h__
