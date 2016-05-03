#ifndef InputLayout_h__
#define InputLayout_h__

#include "LString.h"

enum REG_TYPE
{
	RT_FLOAT1 = 0,
	RT_FLOAT2 = 1,
	RT_FLOAT3 = 2,
	RT_FLOAT4 = 3
};

struct InputElement
{
	String Name;
	int Offset;
	REG_TYPE RegType;
	int StreamId;

	InputElement()
	{

	}
	InputElement(const String& name, int offset, REG_TYPE t, int stream)
	{
		Name = name;
		Offset = offset;
		RegType = t;
		StreamId = stream;
	}
};

class InputLayout
{
public:
	int RegCount;
	InputElement* RegInfo;

	InputLayout(InputElement* elements, int count)
	{
		RegCount = count;
		RegInfo = new InputElement[count];
		memcpy(&RegInfo[0], &elements[0], sizeof(InputElement) * count);
	}

	int FindElement(const String& name)
	{
		for(int i = 0; i < RegCount; i++){
			if(RegInfo[i].Name == name)
				return i;
		}
		return -1;
	}
};
#endif // InputLayout_h__