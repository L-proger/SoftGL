#ifndef Viewport_h__
#define Viewport_h__

#include <cstdint>

class Viewport {
public:
	int32_t width;
	int32_t height;
	float minZ;
	float maxZ;

	Viewport(int32_t _width, int32_t _height, float _minZ, float _maxZ) {
		width = _width;
		height = _height;
		minZ = _minZ;
		maxZ = _maxZ;
	}
	~Viewport() {

	}
};
#endif // Viewport_h__
