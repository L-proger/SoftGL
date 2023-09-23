#ifndef RegisterBlock_h__
#define RegisterBlock_h__

#include <LMath/lmath.h>
#include <array>

using namespace lm;

#define REG_COUNT 5

struct RegisterBlock {
	std::array<float4, REG_COUNT> reg;

	inline static void Sub(const RegisterBlock& b1, const RegisterBlock& b2, RegisterBlock& result) {
		for (int i = 0; i < REG_COUNT; i++) {
			result.reg[i] = b1.reg[i] - b2.reg[i];
		}
	}
	inline static void Add(const RegisterBlock& b1, const RegisterBlock& b2, RegisterBlock& result) {
		for (int i = 0; i < REG_COUNT; i++) {
			result.reg[i] = b1.reg[i] + b2.reg[i];
		}
	}
	inline static void Mul(const RegisterBlock& b1, float val, RegisterBlock& result) {
		for (int i = 0; i < REG_COUNT; i++) {
			result.reg[i] = b1.reg[i] * val;
		}
	}
	inline static void Div(const RegisterBlock& b1, float val, RegisterBlock& result) {
		for (int i = 0; i < REG_COUNT; i++) {
			result.reg[i] = b1.reg[i] / val;
		}
	}
	inline static void Mul(RegisterBlock& b1, float val) {
		for (int i = 0; i < REG_COUNT; i++) {
			b1.reg[i] *= val;
		}
	}
	inline static void Div(RegisterBlock& b1, float val) {
		for (int i = 0; i < REG_COUNT; i++) {
			b1.reg[i] /= val;
		}
	}
	inline static void Clone(const RegisterBlock& from, RegisterBlock& to) {
		for (int i = 0; i < REG_COUNT; i++) {
			to.reg[i] = from.reg[i];
		}
	}
};
#endif // RegisterBlock_h__
