#ifndef fresnel_h__
#define fresnel_h__

#include <cmath>

inline float Fresnel(float c, float ior) {
	float g = std::sqrt(ior*ior + c*c - 1);
	return 0.5f * std::pow(g - c, 2) / std::pow(g + c, 2) * (1 + std::pow(c*(g + c) - 1, 2) / std::pow(c*(g - c) + 1, 2));
}
#endif // fresnel_h__
