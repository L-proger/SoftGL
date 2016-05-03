#ifndef VSDefault_h__
#define VSDefault_h__

#include "VertexShader.h"

class VSDefault : public VertexShader
{
public:
	Matrix4x4 mWorld;
	Matrix4x4 mView;
	Matrix4x4 mProj;

	VSDefault()
	{
		//declare shader inputs
        inputs.push_back(String("POSITION"));
        inputs.push_back(String("TEXCOORD"));
	}
	int Execute(Vector4D** input, Vector4D* output)
	{
		Matrix4x4 wv = mWorld * mView;
		Matrix4x4 wvp = wv * mProj;

		Vector4D position = (*input)[0];
		position.W = 1.0f;

		Vec4Transform(position, wvp);
		output[0] = position;

		output[1] = (*input)[1];

		return 2;
	}
};
#endif // VSDefault_h__
