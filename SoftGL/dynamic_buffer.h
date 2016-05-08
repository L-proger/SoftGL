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
	dynamic_buffer(const dynamic_buffer& v) {
		_size = v._size;
		_data = new T[_size / sizeof(T)];
		memcpy(&_data[0], &v._data[0], _size);
	}
	dynamic_buffer(const dynamic_buffer&& v) {
		_size = v._size;
		_data = v._data;

		v._size = 0;
		v._data = nullptr;
	}

	dynamic_buffer& operator=(const dynamic_buffer& v) {
		_size = v._size;
		_data = new T[_size / sizeof(T)];
		memcpy(&_data[0], &v._data[0], _size);
		return *this;
	}

	dynamic_buffer& operator=(dynamic_buffer&& v) {
		_size = v._size;
		_data = v._data;

		v._size = 0;
		v._data = nullptr;
		return *this;
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

	virtual size_t item_size() override {
		return sizeof(T);
	}

private:
	size_t _size;
	T* _data;
};
#endif // dynamic_buffer_h__
