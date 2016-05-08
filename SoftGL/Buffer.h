#ifndef Buffer_h__
#define Buffer_h__

#include <string.h>
#include <cstdint>
#include "Debug.h"

class buffer {
public:
	constexpr buffer()
	{
		
	}
	virtual ~buffer() = default;
	void write(void* src, size_t offset, size_t length){
		SGL_ASSERT(src != nullptr, "Invalid pointer");
		SGL_ASSERT((offset + length) <= size(), "Invalid pointer");

		memcpy((uint8_t*)get_pointer() + offset, src, length);
	}
	void read(void* dst, size_t offset, int length){
		memcpy(dst, (uint8_t*)get_pointer() + offset, length);
	}
	virtual void* get_pointer() = 0;
	virtual size_t size() = 0;
	virtual size_t item_size()
	{
		return 1;
	}
};
#endif // Buffer_h__
