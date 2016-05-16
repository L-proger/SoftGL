#ifndef config_h__
#define config_h__

#if (_MSC_VER)
#include <memory>
#endif

#if (_MSC_VER)
template<typename T>
using Pointer = std::shared_ptr<T>;
#else
template<typename T>
using Pointer = T*;
#endif


#endif // config_h__
