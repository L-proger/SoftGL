#ifndef mip_static_buffer_h__
#define mip_static_buffer_h__

#include <array>

struct MipUtils {
	static constexpr size_t SmallerMipSize(size_t size){
		return size > 1 ? size / 2 : size;
	}
};

template<size_t _BytesPerPixel, size_t _Width, size_t _Height, size_t _MaxMip = -1, size_t CurrentMip = 0>
struct StaticMipChain {
public:
	static constexpr uint32_t ChildWidth = MipUtils::SmallerMipSize(Width);
	static constexpr uint32_t ChildHeight = MipUtils::SmallerMipSize(Height);
	std::array<uint8_t, _Width * _Height * _BytesPerPixel> data;

	StaticMipChain<_BytesPerPixel, ChildWidth, ChildHeight, _MaxMip, CurrentMip + 1> child;
};

template<size_t _BytesPerPixel, size_t _MaxMip = -1, size_t CurrentMip = 0>
struct StaticMipChain<_BytesPerPixel, 1, 1, _MaxMip, CurrentMip> {
	
};


#endif // mip_static_buffer_h__
