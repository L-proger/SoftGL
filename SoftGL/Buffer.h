#ifndef Buffer_h__
#define Buffer_h__

#include <string.h>

class Buffer
{
private:
	char* data;
	int size;
public:
	Buffer(int _size)
	{
		size = _size;
		if(size>0)
		{
			data = new char[size];
			memset(data,0,size);
		}
		else
			data = 0;
	}
	~Buffer()
	{
		if(data)
			delete[] data;
	}
	void Write(void* src, int offset, int length)
	{
		memcpy(data + offset, src, length);
	}
	void Read(void* dst, int offset, int length)
	{
		memcpy(dst, data + offset, length);
	}
	void* getDataPtr()
	{
		return data;
	}
	int Size()
	{
		return size;
	}
};
#endif // Buffer_h__
