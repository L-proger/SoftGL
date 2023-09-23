#ifndef DisplayMode_h__
#define DisplayMode_h__

#include <cstdint>

struct DisplayMode {
	uint32_t width;
	uint32_t height;
	uint32_t refreshRate;
	uint32_t bpp;
};

#endif // DisplayMode_h__
