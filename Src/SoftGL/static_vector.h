#ifndef static_vector_h__
#define static_vector_h__

#include <cstdint>
#include "Debug.h"

template<typename T, size_t Capacity>
class Static_vector {
public:
	Static_vector():_size(0){

	}
	static constexpr size_t capacity(){
		return Capacity;
	}
	bool full() const {
		return _size == Capacity;
	}
	bool empty() const {
		return _size == 0;
	}
	void clear(){
		_size = 0;
	}
	bool push_back(const T& value){
		if(full()){
			assert(false && "Can't push");
			return false;
		}
		_buffer[_size++] = value;
		return true;
	}
	void pop_back(){
		if(empty()){
			return;
		}
		--_size;
	}
	void remove(size_t id){
		if(id >= _size){
			return;
		}

		_buffer[id] = _buffer[_size-1];
		--_size;
	}
	T& operator[](size_t id){
		SGL_ASSERT(id < _size, "Invalid index");
		return _buffer[id];
	}
	const T& operator[](size_t id) const{
		SGL_ASSERT(id < _size, "Invalid index");
		return _buffer[id];
	}
	size_t size() const {
		return _size;
	}
private:
	T _buffer[Capacity];
	size_t _size;
};
#endif // static_vector_h__
