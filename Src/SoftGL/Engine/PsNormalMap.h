#pragma once

#include <SoftGL/PixelShader.h>
#include <SoftGL/BRDF/lighting_core.h>

class PsNormalMap : public PixelShader {
public:
	float3 camPosition;
	float4 diffuse;
	Texture* diffuse_map;
	Texture* normal_map;
	Texture* ao_map;
	float ior = 1.5f;
	float M = 0.7f;

	PsNormalMap() :diffuse_map(nullptr), normal_map(nullptr), diffuse(0) {

	}
	float4 Execute(float4* input) {
		//return float4(1, 1, 0, 1);
		//return float4(input[1].x, input[1].y, 0, 1);

		float3 lightColor = float3(1.0f, 1.0f, 0.9f);

		float3 ambientColor = float3(0.1f, 0.1f, 0.25f);
		
		float4 normal_tbn = tex2D(normal_map, input[1].x(), input[1].y(), TextureFilter::Bilinear);
		auto tmp = normal_tbn.y();
		normal_tbn.y() = normal_tbn.z();
		normal_tbn.z() = tmp;

		float4 ao = tex2D(ao_map, input[1].x(), input[1].y(), TextureFilter::Bilinear);

		//return float4(normal_tbn.x, normal_tbn.y, normal_tbn.z, 1);

		float3 N = lm::normalize(normal_tbn.xyz() * 2.0f - 1.0f); //lm::normalize(input[2].xyz);
	
		float3 L = lm::normalize(float3(1, 1, 1));
		float3 V = lm::normalize(camPosition - input[3].xyz());
		float3 H = normalize(L + V);

		float NdotL = std::max<float>(0, lm::dot(N, L));
		float NdotV = std::max<float>(0, lm::dot(N, V));
		float LdotH = lm::dot(L, H);

		float F = Fresnel(LdotH, ior);

		float G = G_Cook_Torrance(N, H, V, L);

		float D = D_Beckmann(H, N, M);

		float r_microfacet = (F * G * D) / (4 * NdotL * NdotV + 1.0e-7) * MATH_PI* NdotL;

		float4 diffuse = tex2D(diffuse_map, input[1].x(), input[1].y(), TextureFilter::Bilinear);

		float3 result = float3(0, 0, 0);
		result = diffuse.xyz() * (lightColor * diffuse.xyz()) * NdotL + lightColor * r_microfacet + ambientColor * diffuse.xyz() * ao.x();

		result = lm::saturate(result);

		float4 retval;
		retval.xyz() = result;
		retval.w() = diffuse.w();
		return retval;
	}
};
