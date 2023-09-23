#pragma once

#include "PixelShader.h"
#include <SoftGL/BRDF/lighting_core.h>

class PSDefaultColor : public PixelShader
{
public:
	float4 diffuse;

	PSDefaultColor(): diffuse(1.0f, 1.0f, 1.0f, 1.0f){

	}
	float4 Execute(float4* input) {
		return diffuse;
	}
};
