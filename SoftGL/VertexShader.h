#ifndef VertexShader_h__
#define VertexShader_h__

#include "IShader.h"



class VertexShader : public IShader
{
public:
	virtual ~VertexShader()
	{

	}
	virtual int Execute(float4** input, float4* output) = 0;

};
#endif // VertexShader_h__