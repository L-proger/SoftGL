#ifndef Vertex_h__
#define Vertex_h__

#include "LMath.h"

struct Vertex
{
	float4 Position;
	float2 Texcoord;
	Vertex()
	{
		
	}
	Vertex(const float4& pos, const float2& tc)
	{
		Position = pos;
		Texcoord = tc;
	}

	static int stride()
	{
		return sizeof(float4) + sizeof(float2);
	}
};
#endif // Vertex_h__
