#pragma once

#include <SoftGL/PixelShader.h>

class PSTexturedNoLit : public PixelShader {
public:
	Texture* diffuseMap;

	PSTexturedNoLit() :diffuseMap(nullptr) {}

	float4 Execute(float4* input) {
		float4 diffuse = float4(0, 0, 0, 0);
		if (diffuseMap != nullptr) {
			diffuse = tex2D(diffuseMap, input[1].x(), input[1].y(), TextureFilter::Bilinear);
		}
		return diffuse;
	}
};
