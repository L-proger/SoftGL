#ifndef VSDefault_h__
#define VSDefault_h__

#include "VertexShader.h"

class VSDefault : public VertexShader
{
public:
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;

	VSDefault(){
		//declare shader inputs
        inputs.push_back(String("POSITION"));
        inputs.push_back(String("TEXCOORD"));
		inputs.push_back(String("NORMAL"));
	}
	int Execute(float4** input, float4* output)
	{
		float4x4 wv = lm::mul(mView, mWorld);
		float4x4 wvp = lm::mul(mProj, wv);

		float4 position = *input[0];
		position.w() = 1.0f;

		float3x3 world;
		world[0] = mWorld[0].xyz();
		world[1] = mWorld[1].xyz();
		world[2] = mWorld[2].xyz();
		

		output[0] = lm::mul(wvp, position);
		output[1] = *input[1];
		output[2].xyz() = lm::mul(world, (*input[2]).xyz());
		output[3].xyz() = lm::mul(mWorld, position).xyz();
		return 4;
	}
};
#endif // VSDefault_h__
