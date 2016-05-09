#ifndef PSDefault_h__
#define PSDefault_h__

#include "PixelShader.h"

class PSDefault : public PixelShader
{
public:
	float4 diffuse;
	Texture2D* diffuse_map;

	PSDefault():diffuse_map(nullptr), diffuse(0){

	}
	float4 Execute(float4* input) {
		//return float4(1, 1, 0, 1);
		//return float4(input[1].x, input[1].y, 0, 1);
		

		float3 N = lm::normalize(input[2].xyz);
		float3 L = lm::normalize(float3(4, 1, 3));

		float NdotL = std::max<float>(0, lm::dot(N, L));

		float3 result = float3(0, 0, 0);
		float4 diffuse = float4(0, 0, 0, 0);
		float3 ambient = float3(0.1f, 0.1f, 0.2f);

		

		if(diffuse_map != nullptr){
			tex2D(diffuse_map, &diffuse, input[1].x, input[1].y, TextureFilter::Bilinear);
		}

		result = diffuse.xyz * NdotL;
		result += ambient * diffuse.xyz;

		float4 retval;
		retval.xyz = result;
		retval.w = diffuse.w;
		return retval;
	}
};
#endif // PSDefault_h__