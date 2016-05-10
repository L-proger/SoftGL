#ifndef ITexture2D_h__
#define ITexture2D_h__

#include "mip_buffer.h"

struct ITexture2D {
	virtual MipBuffer* GetTopMip() = 0;
	virtual size_t GetWidth() = 0;
	virtual size_t GetHeight() = 0;
	virtual size_t GetBpp() = 0;
	virtual void UpdateMips() = 0;
};
#endif // ITexture2D_h__
