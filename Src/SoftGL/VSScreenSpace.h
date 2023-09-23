#ifndef VSScreenSpace_h__
#define VSScreenSpace_h__


#include "VertexShader.h"

class VSScreenSpace : public VertexShader
{
public:
	//Matrix4x4 mWorld;
	//Matrix4x4 mView;
	//Matrix4x4 mProj;
	float width;
	float height;

	float pX;
	float pY;

	float screenWidth;
	float screenHeight;


	VSScreenSpace()
	{
		//declare shader inputs
		inputs.push_back(STR("POSITION"));
		inputs.push_back(STR("COLOR"));
	}
	int Execute(float4** input, float4* output)
	{
		//Matrix4x4 wv = mWorld * mView;
		//Matrix4x4 wvp = wv * mProj;
		float xScale = width / screenWidth;
		float yScale = height / screenHeight;

		float4 position = (*input)[0];
		position.W = 1.0f;

		position.X *= width;
		position.Y *= height;

		position.X += pX;
		position.Y += pY;

		position.X = position.X / screenWidth * 2.0f - 1.0f;
		position.Y = position.Y / screenHeight * 2.0f - 1.0f;

		//Output.Position.xy = Output.Position.xy/float2(bbWidth, -bbHeight)*2.0f + float2(-1.0f,1.0f);


		output[0] = position;
		//Vec4Transform(position, wvp);
	    //output[0] = position;

		output[1] = (*input)[1];

		return 2;
	}
};
#endif // VSScreenSpace_h__
