#ifndef dynamic_buffer_h__
#define dynamic_buffer_h__

#include "Buffer.h"

template<typename T>
struct dynamic_buffer : public buffer{
public:
	dynamic_buffer() : _size(0), _data(nullptr) {

	}
	dynamic_buffer(size_t count) : _size(count * sizeof(T)){
		_data = new T[count];
	}
	~dynamic_buffer(){
		if (_data != nullptr) {
			delete[] _data;
		}
	}

	virtual void* get_pointer() override {
		return _data;
	}

	virtual size_t size() override {
		return _size;
	}

private:
	size_t _size;
	T* _data;
};
#endif // dynamic_buffer_h__
