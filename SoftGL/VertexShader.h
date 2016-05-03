#ifndef VertexShader_h__
#define VertexShader_h__

#include "IShader.h"

class VertexShader : public IShader
{
public:
	virtual ~VertexShader()
	{

	}
	virtual int Execute(Vector4D** input, Vector4D* output) = 0;

};
#endif // VertexShader_h__