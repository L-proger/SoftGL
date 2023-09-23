#pragma once

#include <LMath/lmath.h>

struct Vertex {
	lm::float4 Position;
	lm::float3 Normal;
	lm::float2 UV0;

	Vertex() {

	}
	Vertex(const lm::float4& pos, const lm::float2& tc) {
		Position = pos;
		UV0 = tc;
	}
};
