#ifndef InputLayout_h__
#define InputLayout_h__

#include "LString.h"
#include "RasterizerSettings.h"
#include "Debug.h"
#include <array>

enum class RegType {
	float1 = 0,
	float2 = 1,
	float3 = 2,
	float4 = 3
};

struct InputElement {
	char name[InputElementNameLength];
	uint8_t Offset;
	RegType type;
	uint8_t StreamId;

	InputElement() :name{0}, Offset(0), type(RegType::float1), StreamId(0){

	}
	InputElement(const char* _name, uint8_t offset, RegType t, uint8_t stream) {
		SGL_ASSERT(strlen(_name) < InputElementNameLength, "Too long name for InputElement");
		strcpy(name, _name);
		Offset = offset;
		type = t;
		StreamId = stream;
	}
};

class IInputLayout {
public:
	virtual size_t Size() = 0;
	virtual InputElement* GetElement(size_t id) = 0;
	int FindElement(const char* name) {
		auto size = Size();
		for (size_t i = 0; i < size; i++) {
			if (strcmp(GetElement(i)->name, name) == 0)
				return i;
		}
		return -1;
	}
};

template<size_t _Size>
class StaticInputLayout : public IInputLayout {
public:
	std::array<InputElement, _Size> elements;
	virtual size_t Size() override {
		return _Size;
	}
	virtual InputElement* GetElement(size_t id) override {
		return &elements[id];
	}
};
#endif // InputLayout_h__
