#ifndef Viewport_h__
#define Viewport_h__

#include "Types.h"

class Viewport
{
public:
    int32 width;
    int32 height;
	float minZ;
	float maxZ;

    Viewport(int32 _width, int32 _height, float _minZ, float _maxZ)
	{
		width = _width;
		height = _height;
		minZ = _minZ;
		maxZ = _maxZ;
	}
	~Viewport()
	{

	}
};
#endif // Viewport_h__
