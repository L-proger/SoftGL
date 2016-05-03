#include <windows.h>
#include <functional>

namespace detail
{
	
}

template<typename...ArgTypes>
class event {
public:
	typedef std::function<void(ArgTypes...)> delegate;

	event& operator +=(delegate val) {

		return *this;
	}
private:

};

int main()
{
	event<int, float> e;
    return 0;
}

