#ifndef StaticBuffer_h__
#define StaticBuffer_h__

#include "Buffer.h"

template<typename T, size_t Size>
struct static_buffer : public buffer {
	std::array<T, Size> data;
	
	static_buffer(const std::array<T, Size>& values):data(values){

		//static_assert(init.size() == Size, "Incorrect number of parameters");
	}

	virtual void* get_pointer() override {
		return &data[0];
	}

	virtual size_t size() override {
		return Size * sizeof(T);
	}
};

#endif // StaticBuffer_h__
