#pragma once

#include "mip_utils.h"
#include "Texture.h"
#include <vector>
#include <array>

template<size_t _BytesPerPixel, size_t _Width, size_t _Height, size_t _MaxMip = -1, size_t CurrentMip = 0, bool _lastMip = ((_Width == 1) && (_Height == 1)) || (CurrentMip == _MaxMip)>
struct MipChain : MipChain<_BytesPerPixel, MipUtils::SmallerMipSize(_Width), MipUtils::SmallerMipSize(_Height), _MaxMip, CurrentMip + 1> {
	static constexpr auto Width = _Width;
	static constexpr auto Height = _Height;
	static constexpr auto BytesPerPixel = _BytesPerPixel;
	static_assert(MipUtils::IsValidMipSize(_Width), "Invalid MIP width, must be power of 2");
	static_assert(MipUtils::IsValidMipSize(_Height), "Invalid MIP height, must be power of 2");
	std::array<uint8_t, _Width * _Height * _BytesPerPixel> data;
};

template<size_t _BytesPerPixel, size_t _Width, size_t _Height, size_t _MaxMip, size_t CurrentMip>
struct MipChain<_BytesPerPixel, _Width, _Height, _MaxMip, CurrentMip, true> {
	void* GetPointer() {
		return &data[0];
	}
	const void* GetPointer() const {
		return &data[0];
	}

	static constexpr size_t MipsCount = CurrentMip + 1;
	static constexpr auto Width = _Width;
	static constexpr auto Height = _Height;
	static constexpr auto BytesPerPixel = _BytesPerPixel;
	std::array<uint8_t, _Width * _Height * _BytesPerPixel> data;
};


struct DynamicMipChain {
	DynamicMipChain(size_t width, size_t height, size_t bytesPerPixel, size_t maxMip){
		size_t pixelsCount = 0;
		_mipsCount = 0;
		while(_mipsCount <= maxMip && (width > 0) && (height > 0)){
			pixelsCount += width * height;
			width >>= 1;
			height >>= 1;
			_mipsCount++;
		}
		_data.resize(pixelsCount * bytesPerPixel);
	}
	void* GetPointer() {
		return &_data[0];
	}
	const void* GetPointer() const {
		return &_data[0];
	}
	size_t Size() const{
		return _data.size();
	}
	size_t MipsCount() const {
		return _mipsCount;
	}
private:
	size_t _mipsCount;
	std::vector<uint8_t> _data;
};


template<typename _Pointer, bool _IsConst = std::is_const<typename std::remove_pointer<_Pointer>::type>::value>
struct PointerUtils {
	using raw_pointer = decltype(&(*std::declval<_Pointer>()));
	static raw_pointer GetWritable(_Pointer ptr) { return (raw_pointer)ptr->GetPointer(); }
};

template<typename _Pointer>
struct PointerUtils<_Pointer, true> {
	using raw_pointer = decltype(&(*std::declval<_Pointer>()));
	static typename std::remove_cv<std::remove_pointer<raw_pointer>>::type* GetWritable(_Pointer ptr) { return nullptr; }
};


template<typename _DataPointer>
class StaticTexture : public Texture {
public:
	StaticTexture(_DataPointer data, const TextureDescription& description) : Texture(description), _data(data) {}
	virtual void* LockWrite() override { return PointerUtils<_DataPointer>::GetWritable(_data); }
	virtual const void* LockRead() override { return _data->GetPointer(); }
private:
	_DataPointer _data;
};
