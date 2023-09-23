#ifndef Ray_h__
#define Ray_h__

#include "LMath.h"

class Ray
{
public:
	float3 Origin;
	float3 Direction;
	Ray()
		:Origin(0,0,0), Direction(0,0,0)
	{

	}
	Ray(const float3& orig, const float3& dir)
	{
		Origin = orig;
		Direction = dir;
	}
};
#endif // Ray_h__