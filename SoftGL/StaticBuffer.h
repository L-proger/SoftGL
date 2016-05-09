#ifndef StaticBuffer_h__
#define StaticBuffer_h__

#include "Buffer.h"

template<typename T, size_t _Size>
struct static_buffer : public Buffer {
	std::array<T, _Size> data;
	static_buffer() {

		//static_assert(init.size() == Size, "Incorrect number of parameters");
	}
	static_buffer(const std::array<T, _Size>& values):data(values){

		//static_assert(init.size() == Size, "Incorrect number of parameters");
	}

	virtual void* GetPointer() override {
		return &data[0];
	}

	virtual size_t Size() override {
		return _Size * sizeof(T);
	}

	virtual size_t ItemSize() override {
		return sizeof(T);
	}
};

#endif // StaticBuffer_h__
