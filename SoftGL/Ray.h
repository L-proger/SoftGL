#ifndef Ray_h__
#define Ray_h__

#include "LMath.h"

class Ray
{
public:
	Vector3D Origin;
	Vector3D Direction;
	Ray()
		:Origin(0,0,0), Direction(0,0,0)
	{

	}
	Ray(const Vector3D& orig, const Vector3D& dir)
	{
		Origin = orig;
		Direction = dir;
	}
};
#endif // Ray_h__