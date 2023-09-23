#ifndef Buffer_h__
#define Buffer_h__

#include <string.h>
#include <cstdint>
#include "Debug.h"

class Buffer {
public:
	constexpr Buffer()
	{
		
	}
	virtual ~Buffer() = default;
	void Write(void* src, size_t offset, size_t length){
		SGL_ASSERT(src != nullptr, "Invalid pointer");
		SGL_ASSERT((offset + length) <= Size(), "Invalid pointer");

		memcpy((uint8_t*)GetPointer() + offset, src, length);
	}
	void Read(void* dst, size_t offset, int length){
		memcpy(dst, (uint8_t*)GetPointer() + offset, length);
	}
	virtual void* GetPointer() = 0;
	virtual size_t Size() = 0;
	virtual size_t ItemSize()
	{
		return 1;
	}
};
#endif // Buffer_h__
