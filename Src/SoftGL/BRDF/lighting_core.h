#ifndef lighting_core_h__
#define lighting_core_h__

#include <LMath/lmath.h>
#include <algorithm>

#ifndef MATH_PI
#define MATH_PI 3.1415926535
#endif

using namespace lm;

float FresnelSchlickDielectric(float ior, float3 l, float3 h) {
	float f0 = std::pow((1.0f - ior) / (1.0f + ior), 2.0f);
	return f0 + (1.0f - f0) * std::pow(1.0f - dot(l, h), 5.0f);
}

float FresnelSchlickConductor(float ior, float absorbtion, float3 l, float3 h) {
	float n = ior;
	float k = absorbtion;
	return (std::pow(n - 1, 2) + 4.0f * n*std::pow(1.0f - dot(l, h), 5.0f) + k*k) / (std::pow(n + 1.0f, 2.0f) + k*k);
}

float G_implicit(float3 n, float3 h, float3 v, float3 l) {
	return (std::max<float>)(0, dot(n, l)) * (std::max<float>)(0, dot(n, v));
}

float G_Cook_Torrance(float3 n, float3 h, float3 v, float3 l) {
	float VdotH = dot(v, h) + 1e-5f;
	float NdotV = dot(n, v);
	float NdotH = dot(n, h);
	float NdotL = dot(n, l);

	float g1 = (2.0f * dot(n, h) * NdotV) / VdotH;
	float g2 = (2.0f * dot(n, h) * dot(n, l)) / VdotH;
	return (std::min)(1.0f, (std::min)(g1, g2));
}


float D_BlinnPhong(lm::float3 h, lm::float3 n, float power) {
	float normalization = ((power + 2.0f) / (2.0f * MATH_PI));
	//float normalization = 1;
	//float normalization = ((power + 8) / (8.0f * MATH_PI));
	//float normalization = (power + 2) / (4 * MATH_PI*(2 - exp(-power / 2)));
	//float normalization = ((power + 2)*(power + 4)) / (2 * MATH_PI*(pow(2, -power / 2) + power));
	return normalization * std::pow((std::max)(0.0f, lm::dot(n, h)), power);
}

float chiGGX(float v) {
	return v > 0 ? 1 : 0;
}

float GGX_Distribution(float3 n, float3 h, float alpha) {
	float NoH = dot(n, h);
	float alpha2 = alpha * alpha;
	float NoH2 = NoH * NoH;
	float den = NoH2 * alpha2 + (1 - NoH2);
	return (chiGGX(NoH) * alpha2) / (MATH_PI * den * den);
}

float GGX_PartialGeometryTerm(float3 v, float3 n, float3 h, float alpha) {
	float VoH2 = (std::max)(0.0f, (dot(v, h)));
	float chi = chiGGX(VoH2 / (std::max)(0.0f, dot(v, n)));
	VoH2 = VoH2 * VoH2;
	float tan2 = (1.0f - VoH2) / VoH2;
	return (chi * 2.0f) / (1.0f + std::sqrt(1.0f + alpha * alpha * tan2));
}

inline float D_Beckmann(lm::float3 h, lm::float3 n, float m) {
	float c = lm::dot(n, h);
	float T = c * c;
	float M = m*m;
	return std::exp((T - 1.0f) / (M*T)) / (M * T * T);
}

float PhongToBeckmann(float a) {
	return std::sqrt(2.0f / (a + 2.0f));
}

float BeckmannToPhong(float m) {
	return 2.0f / (m*m) - 2.0f;
}

float Fresnel(float c, float ior) {
	float g = std::sqrt(ior*ior + c*c - 1);
	return 0.5f * std::pow(g - c, 2.0f) / std::pow(g + c, 2.0f) * (1.0f + std::pow(c*(g + c) - 1.0f, 2.0f) / std::pow(c*(g - c) + 1.0f, 2.0f));
}
#endif // lighting_core_h__
