#ifndef Vertex_h__
#define Vertex_h__

#include "lmath.h"

using namespace lm;

#pragma pack(push, 1)
struct Vertex {
	float4 Position;
	float3 Normal;
	float2 UV0;

	Vertex() {

	}
	Vertex(const float4& pos, const float2& tc) {
		Position = pos;
		UV0 = tc;
	}
};
#pragma pack(pop)
#endif // Vertex_h__
