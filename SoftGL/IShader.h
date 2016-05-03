#ifndef IShader_h__
#define IShader_h__

#include "LString.h"
#include <vector>
#include "LMath.h"
using namespace std;

class IShader
{
protected:
	typedef std::vector<String> RegisterList;
	RegisterList inputs;

public:
	int RegisterCount()
	{
		return inputs.size();
	}
	int FindRegister(const String& name)
	{
		int cnt = inputs.size();
		if(!cnt) return -1;

		RegisterList::iterator it = inputs.begin();
		for(int i = 0; i < cnt; i++)
		{
			if((*it) == name) return i;
			it++;
		}
		return -1;
	}

	IShader()
	{

	}
	virtual ~IShader()
	{

	}
};
#endif // IShader_h__