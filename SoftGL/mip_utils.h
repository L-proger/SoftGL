#ifndef mip_utils_h__
#define mip_utils_h__

struct MipUtils {
	static constexpr size_t SmallerMipSize(size_t size) {
		return size > 1 ? size / 2 : size;
	}

	static constexpr bool IsValidMipSize(size_t size) {
		return size == 0 ? false : (size == 1 ? true : (size % 2 == 0));
	}
};

#endif // mip_utils_h__