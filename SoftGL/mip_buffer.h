#ifndef mip_buffer_h__
#define mip_buffer_h__

#include "Buffer.h"

struct MipBuffer {
	virtual Buffer* GetData() = 0;
	virtual size_t GetWigth() = 0;
	virtual size_t GetHeight() = 0;
	virtual MipBuffer* GetChild() = 0;
};

#endif // mip_buffer_h__
