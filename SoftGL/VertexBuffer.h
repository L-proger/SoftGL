#ifndef VertexBuffer_h__
#define VertexBuffer_h__
#include "Buffer.h"

class VertexBuffer : public Buffer
{
private:
	int stride;
public:
	
	VertexBuffer(int _size, int _stride)
		: Buffer(_size), stride(_stride)
	{

	}
	int Stride()
	{
		return stride;
	}

};
#endif // VertexBuffer_h__