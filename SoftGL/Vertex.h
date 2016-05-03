#ifndef Vertex_h__
#define Vertex_h__

#include "LMath.h"

struct Vertex
{
	Vector4D Position;
	Vector2D Texcoord;
	Vertex()
	{
		
	}
	Vertex(const Vector4D& pos, const Vector2D& tc)
	{
		Position = pos;
		Texcoord = tc;
	}

	static int stride()
	{
		return sizeof(Vector4D) + sizeof(Vector2D);
	}
};
#endif // Vertex_h__
