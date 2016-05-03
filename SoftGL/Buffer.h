#ifndef Buffer_h__
#define Buffer_h__

#include <string.h>
#include <initializer_list>
#include <cstdint>

class buffer {
public:
	constexpr buffer()
	{
		
	}
	virtual ~buffer() = default;
	void write(void* src, int offset, int length){
		memcpy((uint8_t*)get_pointer() + offset, src, length);
	}
	void read(void* dst, size_t offset, int length){
		memcpy(dst, (uint8_t*)get_pointer() + offset, length);
	}
	virtual void* get_pointer() = 0;
	virtual size_t size() = 0;
};
#endif // Buffer_h__
