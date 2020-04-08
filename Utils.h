#pragma once
namespace Utils {
  template<typename T>
	bool constexpr isPowerOfTwo(T x) noexcept {
		return (x != 0) && ((x & (x - 1)) == 0);
	}

	//courtesy of Graham Palmer, https://stackoverflow.com/a/53935759	
	template<typename T>
	constexpr T nextPowerOfTwo(T value, size_t pow = 0) noexcept {		
		return (value >> pow) ? nextPowerOfTwo(value, pow + 1) : T(1) << pow;
	}
}
