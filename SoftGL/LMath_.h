#ifndef LMath_h__
#define LMath_h__

#include "Matrix3x3.h"
#include "Matrix4x4.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Quaternion.h"
//#include "Plane.h"
#include <math.h>
#include <cstdint>
#include <algorithm>

//#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <time.h>


template <typename T>
static inline T minimum(T a, T b) {
	return a < b ? a : b;
}

template <typename T>
static inline T minimum(T a, T b, T c) {
	if (a < b)
		return a < c ? a : c;
	else
		return b < c ? b : c;
}

template <typename T>
static inline T maximum(T a, T b) {
	return a > b ? a : b;
}

template <typename T>
static inline T maximum(T a, T b, T c) {
	if (a > b)
		return a > c ? a : c;
	else
		return b > c ? b : c;
}




#define min_f minimum<float>
#define min_i minimum<int>

#define max_f maximum<float>
#define max_i maximum<int>

/*
static inline int fround(float val)
{
return static_cast<int>(floor(val + 0.5f));
}*/

static int fround(float x) {
	return (int)std::round(x);
	/*int t;
	#ifdef __GNUC__
	asm("fistpl %0":"=m"(t):"t"(x):"st");
	#else
	__asm
	{
	fld  x
	fistp t
	}
	#endif
	return t;*/
}

static inline float clampFloat(float val, float min, float max) {
	if (val < min)return min;
	if (val > max)return max;
	return val;

}

static inline float random(float low, float high) {
	float temp;

	if (low > high) {
		temp = low;
		low = high;
		high = temp;
	}

	temp = (rand() / (static_cast<float>(RAND_MAX) + 1.0f)) * (high - low) + low;
	return temp;
}

static inline int clampInt(int val, int min, int max) {
	if (val < min)return min;
	if (val > max)return max;
	return val;

}

static inline uint32_t blendrgb(uint32_t d, uint32_t s, uint32_t a) {
	const uint32_t dstrb = d & 0xFF00FF;
	const uint32_t dstg = d & 0xFF00;
	const uint32_t srcrb = s & 0xFF00FF;
	const uint32_t srcg = s & 0xFF00;

	uint32_t drb = srcrb - dstrb;
	uint32_t dg = srcg - dstg;

	drb *= a;
	dg *= a;
	drb >>= 8;
	dg >>= 8;

	uint32_t rb = (drb + dstrb) & 0xFF00FF;
	uint32_t g = (dg + dstg) & 0xFF00;

	return rb | g;
}

static inline bool IsEvenNum(int num) {
	return !((num % 2)>0);
}

static inline void Vec2TransformCoordinate(Vector2D& coord, Matrix3x3& transform) {
	Vector2D vector;
	vector.X = ((coord.X * transform.m11) + (coord.Y * transform.m21)) + transform.m31;
	vector.Y = ((coord.X * transform.m12) + (coord.Y * transform.m22)) + transform.m32;
	//vector.W = 1.0f / ((((coord.X * transform.M14) + (coord.Y * transform.M24)) + (coord.Z * transform.M34)) + transform.M44);
	//vector.W = 1.0f;
	coord.X = vector.X;
	coord.Y = vector.Y;
}
static inline Matrix3x3* Mat3x3Identity(Matrix3x3* dst) {
	std::fill(std::begin(dst->data), std::end(dst->data), 0.0f);
	dst->m11 = 1.0f;
	dst->m22 = 1.0f;
	dst->m33 = 1.0f;
	return dst;
}
static inline void Mat3x3Translation(const Vector2D& amount, Matrix3x3& matrix) {
	matrix.m11 = 1.0f;
	matrix.m12 = 0.0f;
	matrix.m13 = 0.0f;

	matrix.m21 = 0.0f;
	matrix.m22 = 1.0f;
	matrix.m23 = 0.0f;

	matrix.m31 = amount.X;
	matrix.m32 = amount.Y;
	matrix.m33 = 1.0f;
}

static inline void Mat3x3Rotation(float angle, Matrix3x3& matrix) {
	matrix.m11 = cosf(angle);
	matrix.m12 = sinf(angle);
	matrix.m13 = 0.0f;

	matrix.m21 = -sinf(angle);
	matrix.m22 = cosf(angle);
	matrix.m23 = 0.0f;

	matrix.m31 = 0.0f;
	matrix.m32 = 0.0f;
	matrix.m33 = 1.0f;
}

enum Axis {
	X, Y, Z, W
};


static void Mat4x4Invert(const Matrix4x4* matrix, Matrix4x4* result);
static void QuatRotationYawPitchRoll(float yaw, float pitch, float roll, Quaternion& result);
static void QuatRotationMatrix(const Matrix4x4& matrix, Quaternion& dst);

//----- Vector3D-----
static inline float Vec3Length(Vector3D* src) {
	return sqrtf(src->X * src->X + src->Y * src->Y + src->Z * src->Z);
}

static inline void Vec3Normalize(Vector3D* src) {
	float num2 = Vec3Length(src);
	if (num2 != 0.0f) {
		float num = 1.0f / num2;
		src->X *= num;
		src->Y *= num;
		src->Z *= num;
	}
}

static inline void Vec3Normalize(Vector3D* vector, Vector3D* result) {
	float num2 = Vec3Length(vector);
	if (num2 != 0.0f) {
		float num = 1.0f / num2;
		result->X = vector->X * num;
		result->Y = vector->Y * num;
		result->Z = vector->Z * num;
	} else {
		result->X = result->Y = result->Z = 0.0f;
	}
}


static inline void Vec3Add(Vector3D& src, Vector3D& val) {
	src.X += val.X;
	src.Y += val.Y;
	src.Z += val.Z;
}

static inline void Vec3Reflect(Vector3D& vector, Vector3D& normal, Vector3D& result) {

	float num2 = ((vector.Y * normal.Y) + (vector.X * normal.X)) + (vector.Z * normal.Z);
	float num = num2 * 2.0f;
	result.X = vector.X - (normal.X * num);
	result.Y = vector.Y - (normal.Y * num);
	result.Z = vector.Z - (normal.Z * num);
}

static inline void Vec3Reflect(Vector3D& vector, Vector3D& normal) {

	float num2 = ((vector.Y * normal.Y) + (vector.X * normal.X)) + (vector.Z * normal.Z);
	float num = num2 * 2.0f;
	vector.X = vector.X - (normal.X * num);
	vector.Y = vector.Y - (normal.Y * num);
	vector.Z = vector.Z - (normal.Z * num);
}

static inline void Vec3Transform(Vector3D* vector, Matrix4x4* transformation, Vector4D* result) {
	result->X = (((vector->Y * transformation->M21) + (vector->X * transformation->M11)) + (vector->Z * transformation->M31)) + transformation->M41;
	result->Y = (((vector->Y * transformation->M22) + (vector->X * transformation->M12)) + (vector->Z * transformation->M32)) + transformation->M42;
	result->Z = (((vector->Y * transformation->M23) + (vector->X * transformation->M13)) + (vector->Z * transformation->M33)) + transformation->M43;
	result->W = (((vector->Y * transformation->M24) + (transformation->M14 * vector->X)) + (vector->Z * transformation->M34)) + transformation->M44;
}

static inline float Vec3Dot(const Vector3D* left, const Vector3D* right) {
	return (((left->Y * right->Y) + (left->X * right->X)) + (left->Z * right->Z));
}

static inline Vector3D Vec3TransformCoordinate(const Vector3D& coordinate, const Matrix4x4& transformation) {
	Vector3D vector;
	Vector4D vector2;
	vector2.X = (((transformation.M21 * coordinate.Y) + (transformation.M11 * coordinate.X)) + (transformation.M31 * coordinate.Z)) + transformation.M41;
	vector2.Y = (((transformation.M22 * coordinate.Y) + (transformation.M12 * coordinate.X)) + (transformation.M32 * coordinate.Z)) + transformation.M42;
	vector2.Z = (((transformation.M23 * coordinate.Y) + (transformation.M13 * coordinate.X)) + (transformation.M33 * coordinate.Z)) + transformation.M43;
	vector2.W = (((transformation.M24 * coordinate.Y) + (transformation.M14 * coordinate.X)) + (transformation.M34 * coordinate.Z)) + transformation.M44;

	vector2.W = 1.0f / vector2.W;

	vector.X = vector2.X * vector2.W;
	vector.Y = vector2.Y * vector2.W;
	vector.Z = vector2.Z * vector2.W;
	return vector;
}

static inline void Vec4Transform(Vector4D& coordinate, const Matrix4x4& transformation) {
	Vector4D vector2;
	vector2.X = (((transformation.M21 * coordinate.Y) + (transformation.M11 * coordinate.X)) + (transformation.M31 * coordinate.Z)) + transformation.M41;
	vector2.Y = (((transformation.M22 * coordinate.Y) + (transformation.M12 * coordinate.X)) + (transformation.M32 * coordinate.Z)) + transformation.M42;
	vector2.Z = (((transformation.M23 * coordinate.Y) + (transformation.M13 * coordinate.X)) + (transformation.M33 * coordinate.Z)) + transformation.M43;
	vector2.W = (((transformation.M24 * coordinate.Y) + (transformation.M14 * coordinate.X)) + (transformation.M34 * coordinate.Z)) + transformation.M44;
	coordinate = vector2;
}

static inline void Vec3TransformCoordinate(Vector3D& coordinate, const Matrix4x4& transformation) {
	Vector4D vector2;
	vector2.X = (((transformation.M21 * coordinate.Y) + (transformation.M11 * coordinate.X)) + (transformation.M31 * coordinate.Z)) + transformation.M41;
	vector2.Y = (((transformation.M22 * coordinate.Y) + (transformation.M12 * coordinate.X)) + (transformation.M32 * coordinate.Z)) + transformation.M42;
	vector2.Z = (((transformation.M23 * coordinate.Y) + (transformation.M13 * coordinate.X)) + (transformation.M33 * coordinate.Z)) + transformation.M43;
	float num = (float)(1.0 / ((((transformation.M24 * coordinate.Y) + (transformation.M14 * coordinate.X)) + (transformation.M34 * coordinate.Z)) + transformation.M44));
	vector2.W = num;
	coordinate.X = vector2.X * num;
	coordinate.Y = vector2.Y * num;
	coordinate.Z = vector2.Z * num;
}


static inline Vector3D Vec3TransformNoW(const Vector3D& coordinate, const Matrix4x4& transformation) {
	Vector3D vector;
	vector.X = (((transformation.M21 * coordinate.Y) + (transformation.M11 * coordinate.X)) + (transformation.M31 * coordinate.Z)) + transformation.M41;
	vector.Y = (((transformation.M22 * coordinate.Y) + (transformation.M12 * coordinate.X)) + (transformation.M32 * coordinate.Z)) + transformation.M42;
	vector.Z = (((transformation.M23 * coordinate.Y) + (transformation.M13 * coordinate.X)) + (transformation.M33 * coordinate.Z)) + transformation.M43;
	return vector;
}


static inline void Vec3Minimize(Vector3D& value1, Vector3D& value2, Vector3D& result) {
	float z;
	float y;
	float x;
	if (value1.X < value2.X) {
		x = value1.X;
	} else {
		x = value2.X;
	}
	result.X = x;
	if (value1.Y < value2.Y) {
		y = value1.Y;
	} else {
		y = value2.Y;
	}
	result.Y = y;
	if (value1.Z < value2.Z) {
		z = value1.Z;
	} else {
		z = value2.Z;
	}
	result.Z = z;
}

static inline void Vec3Minimize(Vector4D& value1, Vector3D& value2, Vector3D& result) {
	float z;
	float y;
	float x;
	if (value1.X < value2.X) {
		x = value1.X;
	} else {
		x = value2.X;
	}
	result.X = x;
	if (value1.Y < value2.Y) {
		y = value1.Y;
	} else {
		y = value2.Y;
	}
	result.Y = y;
	if (value1.Z < value2.Z) {
		z = value1.Z;
	} else {
		z = value2.Z;
	}
	result.Z = z;
}



static inline void Vec3Maximize(Vector3D& value1, Vector3D& value2, Vector3D& result) {
	float z;
	float y;
	float x;
	if (value1.X > value2.X) {
		x = value1.X;
	} else {
		x = value2.X;
	}
	result.X = x;
	if (value1.Y > value2.Y) {
		y = value1.Y;
	} else {
		y = value2.Y;
	}
	result.Y = y;
	if (value1.Z > value2.Z) {
		z = value1.Z;
	} else {
		z = value2.Z;
	}
	result.Z = z;
}

static inline void Vec3Maximize(Vector4D& value1, Vector3D& value2, Vector3D& result) {
	float z;
	float y;
	float x;
	if (value1.X > value2.X) {
		x = value1.X;
	} else {
		x = value2.X;
	}
	result.X = x;
	if (value1.Y > value2.Y) {
		y = value1.Y;
	} else {
		y = value2.Y;
	}
	result.Y = y;
	if (value1.Z > value2.Z) {
		z = value1.Z;
	} else {
		z = value2.Z;
	}
	result.Z = z;
}



static inline void Vec3TransformCoordinate(const Vector3D& coordinate, const Matrix4x4& transformation, Vector3D& result) {
	Vector4D vector2;
	vector2.X = (((coordinate.Y * transformation.M21) + (coordinate.X * transformation.M11)) + (coordinate.Z * transformation.M31)) + transformation.M41;
	vector2.Y = (((coordinate.Y * transformation.M22) + (coordinate.X * transformation.M12)) + (coordinate.Z * transformation.M32)) + transformation.M42;
	vector2.Z = (((coordinate.Y * transformation.M23) + (coordinate.X * transformation.M13)) + (coordinate.Z * transformation.M33)) + transformation.M43;
	float num = (float)(1.0 / ((((transformation.M24 * coordinate.Y) + (transformation.M14 * coordinate.X)) + (coordinate.Z * transformation.M34)) + transformation.M44));
	vector2.W = num;
	result.X = vector2.X * num;
	result.Y = vector2.Y * num;
	result.Z = vector2.Z * num;
}

static inline void Vec3Cross(const Vector3D& left, const Vector3D& right, Vector3D& result) {
	result.X = (left.Y * right.Z) - (left.Z * right.Y);
	result.Y = (left.Z * right.X) - (left.X * right.Z);
	result.Z = (left.X * right.Y) - (left.Y * right.X);
}

static inline Vector3D Vec3Cross(const Vector3D& vector1, const Vector3D& vector2) {
	Vector3D vector;
	vector.X = (vector1.Y * vector2.Z) - (vector1.Z * vector2.Y);
	vector.Y = (vector1.Z * vector2.X) - (vector1.X * vector2.Z);
	vector.Z = (vector1.X * vector2.Y) - (vector1.Y * vector2.X);
	return vector;
}




static inline Vector3D Vec3Lerp(const Vector3D& start, const Vector3D& end, float amount) {
	Vector3D vector;
	vector.X = ((end.X - start.X) * amount) + start.X;
	vector.Y = ((end.Y - start.Y) * amount) + start.Y;
	vector.Z = ((end.Z - start.Z) * amount) + start.Z;
	return vector;
}
static inline void Vec3Lerp(const Vector3D& start, const Vector3D& end, float amount, Vector3D& result) {
	result.X = ((end.X - start.X) * amount) + start.X;
	result.Y = ((end.Y - start.Y) * amount) + start.Y;
	result.Z = ((end.Z - start.Z) * amount) + start.Z;
}

static inline void Vec3Move(Vector3D& src, const Vector3D& direction, float amount) {
	src.X += direction.X * amount;
	src.Y += direction.Y * amount;
	src.Z += direction.Z * amount;
}
static inline void Vec3Mul(Vector3D& vector, float val) {
	vector.X *= val;
	vector.Y *= val;
	vector.Z *= val;
}

static inline void Vec3TransformCoordinate(Vector3D& coord, Matrix4x4& transform) {
	Vector4D vector;
	vector.X = (((coord.X * transform.M11) + (coord.Y * transform.M21)) + (coord.Z * transform.M31)) + transform.M41;
	vector.Y = (((coord.X * transform.M12) + (coord.Y * transform.M22)) + (coord.Z * transform.M32)) + transform.M42;
	vector.Z = (((coord.X * transform.M13) + (coord.Y * transform.M23)) + (coord.Z * transform.M33)) + transform.M43;
	vector.W = 1.0f / ((((coord.X * transform.M14) + (coord.Y * transform.M24)) + (coord.Z * transform.M34)) + transform.M44);
	//vector.W = 1.0f;
	coord.X = vector.X * vector.W;
	coord.Y = vector.Y * vector.W;
	coord.Z = vector.Z * vector.W;
}
static inline float Vec3Distance(const Vector3D& left, const Vector3D& right) {
	float x = right.X - left.X;
	float y = right.Y - left.Y;
	float z = right.Z - left.Z;

	return (float)sqrtf(x * x + y * y + z * z);
}

static inline float Vec3DistanceSquared(const Vector3D& left, const Vector3D& right) {
	float x = right.X - left.X;
	float y = right.Y - left.Y;
	float z = right.Z - left.Z;

	return x * x + y * y + z * z;
}

static inline float Vec3DistanceSquared(const Vector3D& left, const Vector4D& right) {
	float x = right.X - left.X;
	float y = right.Y - left.Y;
	float z = right.Z - left.Z;

	return x * x + y * y + z * z;
}

/*inline Vector3D Vec3Unproject(const Vector3D& vector, float x, float y, float width, float height, float minZ, float maxZ, const Matrix4x4& worldViewProjection)
{
Vector3D coordinate;
Matrix4x4 transformation;
Mat4x4Invert(&worldViewProjection, &transformation);
transformation = Matrix4x4.Invert(worldViewProjection);

coordinate.X = (float)((((vector.X - x) / ((double)width)) * 2.0) - 1.0);
coordinate.Y = (float)-((((vector.Y - y) / ((double)height)) * 2.0) - 1.0);
coordinate.Z = (vector.Z - minZ) / (maxZ - minZ);
TransformCoordinate(ref coordinate, ref transformation, out coordinate);
return coordinate;
}*/
//-----Vector4D-----
static inline void Vec4Move(Vector3D& source, Vector3D& direction, float amount) {

	source.X += direction.X * amount;
	source.Y += direction.Y * amount;
	source.Z += direction.Z * amount;
}
static inline void Vec4Minimize(Vector4D& value1, Vector4D& value2, Vector4D& result) {
	float z;
	float y;
	float x;
	float w;
	if (value1.X < value2.X) {
		x = value1.X;
	} else {
		x = value2.X;
	}
	result.X = x;
	if (value1.Y < value2.Y) {
		y = value1.Y;
	} else {
		y = value2.Y;
	}
	result.Y = y;
	if (value1.Z < value2.Z) {
		z = value1.Z;
	} else {
		z = value2.Z;
	}
	result.Z = z;
	if (value1.W < value2.W) {
		w = value1.W;
	} else {
		w = value2.W;
	}
	result.W = w;
}
static inline void Vec4Maximize(Vector4D& value1, Vector4D& value2, Vector4D& result) {
	float z;
	float y;
	float x;
	float w;
	if (value1.X > value2.X) {
		x = value1.X;
	} else {
		x = value2.X;
	}
	result.X = x;
	if (value1.Y > value2.Y) {
		y = value1.Y;
	} else {
		y = value2.Y;
	}
	result.Y = y;
	if (value1.Z > value2.Z) {
		z = value1.Z;
	} else {
		z = value2.Z;
	}
	result.Z = z;

	if (value1.W > value2.W) {
		w = value1.W;
	} else {
		w = value2.W;
	}
	result.W = w;
}
//-----Matrix4x4-----
static inline Matrix4x4* Mat4x4Identity(Matrix4x4* dst) {
	std::fill(std::begin(dst->data), std::end(dst->data), 0.0f);
	dst->M11 = dst->M22 = dst->M33 = dst->M44 = 1.0f;
	return dst;
}

static inline void Mat4x4Ortho(float width, float height, float zNear, float zFar, Matrix4x4& result) {
	result.M12 = result.M13 = result.M14 =
		result.M21 = result.M23 = result.M24 =
		result.M31 = result.M32 = result.M34 =
		result.M41 = result.M42 = 0.0f;
	result.M44 = 1.0f;

	result.M11 = 2.0f / width;
	result.M22 = 2.0f / height;
	result.M33 = 1.0f / (zFar - zNear);
	result.M43 = zNear / (zNear - zFar);
}


static inline Matrix4x4 Mat4x4Invert(const Matrix4x4* matrix) {
	Matrix4x4 matrix2;
	float num5 = matrix->M11;
	float num4 = matrix->M12;
	float num3 = matrix->M13;
	float num2 = matrix->M14;
	float num9 = matrix->M21;
	float num8 = matrix->M22;
	float num7 = matrix->M23;
	float num6 = matrix->M24;
	float num17 = matrix->M31;
	float num16 = matrix->M32;
	float num15 = matrix->M33;
	float num14 = matrix->M34;
	float num13 = matrix->M41;
	float num12 = matrix->M42;
	float num11 = matrix->M43;
	float num10 = matrix->M44;
	float num23 = (num15 * num10) - (num14 * num11);
	float num22 = (num16 * num10) - (num14 * num12);
	float num21 = (num16 * num11) - (num15 * num12);
	float num20 = (num17 * num10) - (num14 * num13);
	float num19 = (num17 * num11) - (num15 * num13);
	float num18 = (num17 * num12) - (num16 * num13);
	float num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
	float num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
	float num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
	float num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
	float num = 1.0f / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));
	matrix2.M11 = num39 * num;
	matrix2.M21 = num38 * num;
	matrix2.M31 = num37 * num;
	matrix2.M41 = num36 * num;
	matrix2.M12 = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
	matrix2.M22 = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
	matrix2.M32 = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
	matrix2.M42 = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;
	float num35 = (num7 * num10) - (num6 * num11);
	float num34 = (num8 * num10) - (num6 * num12);
	float num33 = (num8 * num11) - (num7 * num12);
	float num32 = (num9 * num10) - (num6 * num13);
	float num31 = (num9 * num11) - (num7 * num13);
	float num30 = (num9 * num12) - (num8 * num13);
	matrix2.M13 = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
	matrix2.M23 = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
	matrix2.M33 = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
	matrix2.M43 = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;
	float num29 = (num7 * num14) - (num6 * num15);
	float num28 = (num8 * num14) - (num6 * num16);
	float num27 = (num8 * num15) - (num7 * num16);
	float num26 = (num9 * num14) - (num6 * num17);
	float num25 = (num9 * num15) - (num7 * num17);
	float num24 = (num9 * num16) - (num8 * num17);
	matrix2.M14 = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
	matrix2.M24 = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
	matrix2.M34 = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
	matrix2.M44 = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;
	return matrix2;
}

inline void Mat4x4Invert(const Matrix4x4* matrix, Matrix4x4* result) {
	float num5 = matrix->M11;
	float num4 = matrix->M12;
	float num3 = matrix->M13;
	float num2 = matrix->M14;
	float num9 = matrix->M21;
	float num8 = matrix->M22;
	float num7 = matrix->M23;
	float num6 = matrix->M24;
	float num17 = matrix->M31;
	float num16 = matrix->M32;
	float num15 = matrix->M33;
	float num14 = matrix->M34;
	float num13 = matrix->M41;
	float num12 = matrix->M42;
	float num11 = matrix->M43;
	float num10 = matrix->M44;
	float num23 = (num15 * num10) - (num14 * num11);
	float num22 = (num16 * num10) - (num14 * num12);
	float num21 = (num16 * num11) - (num15 * num12);
	float num20 = (num17 * num10) - (num14 * num13);
	float num19 = (num17 * num11) - (num15 * num13);
	float num18 = (num17 * num12) - (num16 * num13);
	float num39 = ((num8 * num23) - (num7 * num22)) + (num6 * num21);
	float num38 = -(((num9 * num23) - (num7 * num20)) + (num6 * num19));
	float num37 = ((num9 * num22) - (num8 * num20)) + (num6 * num18);
	float num36 = -(((num9 * num21) - (num8 * num19)) + (num7 * num18));
	float num = 1.0f / ((((num5 * num39) + (num4 * num38)) + (num3 * num37)) + (num2 * num36));
	result->M11 = num39 * num;
	result->M21 = num38 * num;
	result->M31 = num37 * num;
	result->M41 = num36 * num;
	result->M12 = -(((num4 * num23) - (num3 * num22)) + (num2 * num21)) * num;
	result->M22 = (((num5 * num23) - (num3 * num20)) + (num2 * num19)) * num;
	result->M32 = -(((num5 * num22) - (num4 * num20)) + (num2 * num18)) * num;
	result->M42 = (((num5 * num21) - (num4 * num19)) + (num3 * num18)) * num;
	float num35 = (num7 * num10) - (num6 * num11);
	float num34 = (num8 * num10) - (num6 * num12);
	float num33 = (num8 * num11) - (num7 * num12);
	float num32 = (num9 * num10) - (num6 * num13);
	float num31 = (num9 * num11) - (num7 * num13);
	float num30 = (num9 * num12) - (num8 * num13);
	result->M13 = (((num4 * num35) - (num3 * num34)) + (num2 * num33)) * num;
	result->M23 = -(((num5 * num35) - (num3 * num32)) + (num2 * num31)) * num;
	result->M33 = (((num5 * num34) - (num4 * num32)) + (num2 * num30)) * num;
	result->M43 = -(((num5 * num33) - (num4 * num31)) + (num3 * num30)) * num;
	float num29 = (num7 * num14) - (num6 * num15);
	float num28 = (num8 * num14) - (num6 * num16);
	float num27 = (num8 * num15) - (num7 * num16);
	float num26 = (num9 * num14) - (num6 * num17);
	float num25 = (num9 * num15) - (num7 * num17);
	float num24 = (num9 * num16) - (num8 * num17);
	result->M14 = -(((num4 * num29) - (num3 * num28)) + (num2 * num27)) * num;
	result->M24 = (((num5 * num29) - (num3 * num26)) + (num2 * num25)) * num;
	result->M34 = -(((num5 * num28) - (num4 * num26)) + (num2 * num24)) * num;
	result->M44 = (((num5 * num27) - (num4 * num25)) + (num3 * num24)) * num;
}

static inline Matrix4x4* Mat4x4RotationAxis(Vector3D* axis, float angle, Matrix4x4* pResult) {
	float y = axis->Y;
	float x = axis->X;
	float z = axis->Z;
	if (((float)(((x * x) + (y * y)) + (z * z))) != 1.0f) {
		Vec3Normalize(axis);
		// axis.Normalize();
	}
	float num4 = axis->X;
	float num3 = axis->Y;
	float num2 = axis->Z;
	float num = cosf(angle);
	float num5 = sinf(angle);
	float num23 = num4;
	float num17 = (num23 * num23);
	float num22 = num3;
	float num16 = (num22 * num22);
	float num21 = num2;
	float num15 = (num21 * num21);
	float num20 = num3 * num4;
	float num19 = num2 * num4;
	float num18 = num2 * num3;
	pResult->M11 = (((1.0f - num17) * num)) + num17;
	float num14 = num20;
	float num13 = num14 - (num * num14);
	float num12 = num5 * num2;
	pResult->M12 = (num12 + num13);
	float num11 = num19;
	float num10 = num11 - (num * num11);
	float num9 = num5 * num3;
	pResult->M13 = (num10 - num9);
	pResult->M14 = 0.0f;
	pResult->M21 = (num13 - num12);
	pResult->M22 = (((1.0f - num16) * num)) + num16;
	float num8 = num18;
	float num7 = num8 - (num * num8);
	float num6 = num5 * num4;
	pResult->M23 = (float)(num6 + num7);
	pResult->M24 = 0.0f;
	pResult->M31 = (float)(num9 + num10);
	pResult->M32 = (float)(num7 - num6);
	pResult->M33 = ((float)((1.0f - num15) * num)) + num15;
	pResult->M34 = 0.0f;
	pResult->M41 = 0.0f;
	pResult->M42 = 0.0f;
	pResult->M43 = 0.0f;
	pResult->M44 = 1.0f;

	return pResult;
}


static inline Matrix4x4 Mat4x4RotationX(float angle) {
	Matrix4x4 matrix;
	float num2 = cosf(angle);
	float num = sinf(angle);
	matrix.M11 = 1.0f;
	matrix.M12 = 0.0f;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = num2;
	matrix.M23 = num;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = -num;
	matrix.M33 = num2;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
	return matrix;
}

static inline void Mat4x4RotationX(float angle, Matrix4x4& matrix) {
	float num2 = cosf(angle);
	float num = sinf(angle);
	matrix.M11 = 1.0f;
	matrix.M12 = 0.0f;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = num2;
	matrix.M23 = num;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = -num;
	matrix.M33 = num2;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
}

static inline Matrix4x4 Mat4x4RotationY(float angle) {
	Matrix4x4 matrix;
	float num2 = cosf(angle);
	float num = sinf(angle);
	matrix.M11 = num2;
	matrix.M12 = 0.0f;
	matrix.M13 = -num;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = 1.0f;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = num;
	matrix.M32 = 0.0f;
	matrix.M33 = num2;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
	return matrix;
}
static inline void Mat4x4RotationY(float angle, Matrix4x4& matrix) {
	float num2 = cosf(angle);
	float num = sinf(angle);
	matrix.M11 = num2;
	matrix.M12 = 0.0f;
	matrix.M13 = -num;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = 1.0f;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = num;
	matrix.M32 = 0.0f;
	matrix.M33 = num2;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
}

static inline Matrix4x4 Mat4x4RotationZ(float angle) {
	Matrix4x4 matrix;
	float num2 = cosf(angle);
	float num = sinf(angle);
	matrix.M11 = num2;
	matrix.M12 = num;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = -num;
	matrix.M22 = num2;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = 0.0f;
	matrix.M33 = 1.0f;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
	return matrix;
}
static inline void Mat4x4RotationZ(float angle, Matrix4x4& matrix) {
	float num2 = cosf(angle);
	float num = sinf(angle);
	matrix.M11 = num2;
	matrix.M12 = num;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = -num;
	matrix.M22 = num2;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = 0.0f;
	matrix.M33 = 1.0f;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
}

static inline void Mat4x4RotationQuaternion(const Quaternion& rotation, Matrix4x4& result) {
	float x = rotation.X;
	float num9 = (x * x);
	float y = rotation.Y;
	float num8 = (y * y);
	float z = rotation.Z;
	float num7 = (z * z);
	float num6 = rotation.Y * rotation.X;
	float num5 = rotation.W * rotation.Z;
	float num4 = rotation.Z * rotation.X;
	float num3 = rotation.W * rotation.Y;
	float num2 = rotation.Z * rotation.Y;
	float num = rotation.W * rotation.X;
	result.M11 = (1.0f - ((num7 + num8) * 2.0f));
	result.M12 = ((num5 + num6) * 2.0f);
	result.M13 = ((num4 - num3) * 2.0f);
	result.M14 = 0.0f;
	result.M21 = ((num6 - num5) * 2.0f);
	result.M22 = (1.0f - ((num7 + num9) * 2.0f));
	result.M23 = ((num + num2) * 2.0f);
	result.M24 = 0.0f;
	result.M31 = ((num3 + num4) * 2.0f);
	result.M32 = ((num2 - num) * 2.0f);
	result.M33 = (1.0f - ((num8 + num9) * 2.0f));
	result.M34 = 0.0f;
	result.M41 = 0.0f;
	result.M42 = 0.0f;
	result.M43 = 0.0f;
	result.M44 = 1.0f;
}
static inline void Mat4x4RotationYawPitchRoll(float yaw, float pitch, float roll, Matrix4x4& out) {
	Quaternion quaternion;
	QuatRotationYawPitchRoll(yaw, pitch, roll, quaternion);
	Mat4x4RotationQuaternion(quaternion, out);
}
static inline void Mat4x4Scaling(float xScale, float yScale, float zScale, Matrix4x4& out) {
	out.M11 = xScale;
	out.M12 = 0.0f;
	out.M13 = 0.0f;
	out.M14 = 0.0f;
	out.M21 = 0.0f;
	out.M22 = yScale;
	out.M23 = 0.0f;
	out.M24 = 0.0f;
	out.M31 = 0.0f;
	out.M32 = 0.0f;
	out.M33 = zScale;
	out.M34 = 0.0f;
	out.M41 = 0.0f;
	out.M42 = 0.0f;
	out.M43 = 0.0f;
	out.M44 = 1.0f;
}
static inline void Mat4x4Scaling(Vector3D& amt, Matrix4x4& matrix) {
	matrix.M11 = amt.X;
	matrix.M12 = 0.0f;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = amt.Y;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = 0.0f;
	matrix.M33 = amt.Z;
	matrix.M34 = 0.0f;
	matrix.M41 = 0.0f;
	matrix.M42 = 0.0f;
	matrix.M43 = 0.0f;
	matrix.M44 = 1.0f;
}

static inline void Mat4x4Translation(const Vector3D& amount, Matrix4x4& matrix) {
	matrix.M11 = 1.0f;
	matrix.M12 = 0.0f;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = 1.0f;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = 0.0f;
	matrix.M33 = 1.0f;
	matrix.M34 = 0.0f;
	matrix.M41 = amount.X;
	matrix.M42 = amount.Y;
	matrix.M43 = amount.Z;
	matrix.M44 = 1.0f;
}
static inline void Mat4x4Translation(float _x, float _y, float _z, Matrix4x4& matrix) {
	matrix.M11 = 1.0f;
	matrix.M12 = 0.0f;
	matrix.M13 = 0.0f;
	matrix.M14 = 0.0f;
	matrix.M21 = 0.0f;
	matrix.M22 = 1.0f;
	matrix.M23 = 0.0f;
	matrix.M24 = 0.0f;
	matrix.M31 = 0.0f;
	matrix.M32 = 0.0f;
	matrix.M33 = 1.0f;
	matrix.M34 = 0.0f;
	matrix.M41 = _x;
	matrix.M42 = _y;
	matrix.M43 = _z;
	matrix.M44 = 1.0f;
}
static inline void Mat4x4Transpose(const Matrix4x4& src, Matrix4x4& dst) {
	Matrix4x4 tmp = src;
	dst.M11 = tmp.M11;
	dst.M12 = tmp.M21;
	dst.M13 = tmp.M31;
	dst.M14 = tmp.M41;
	dst.M21 = tmp.M12;
	dst.M22 = tmp.M22;
	dst.M23 = tmp.M32;
	dst.M24 = tmp.M42;
	dst.M31 = tmp.M13;
	dst.M32 = tmp.M23;
	dst.M33 = tmp.M33;
	dst.M34 = tmp.M43;
	dst.M41 = tmp.M14;
	dst.M42 = tmp.M24;
	dst.M43 = tmp.M34;
	dst.M44 = tmp.M44;
}
static inline void Mat4x4Lerp(const Matrix4x4& start, const Matrix4x4& end, Matrix4x4& result, float amount) {
	result.M11 = ((end.M11 - start.M11) * amount) + start.M11;
	result.M12 = ((end.M12 - start.M12) * amount) + start.M12;
	result.M13 = ((end.M13 - start.M13) * amount) + start.M13;
	result.M14 = ((end.M14 - start.M14) * amount) + start.M14;
	result.M21 = ((end.M21 - start.M21) * amount) + start.M21;
	result.M22 = ((end.M22 - start.M22) * amount) + start.M22;
	result.M23 = ((end.M23 - start.M23) * amount) + start.M23;
	result.M24 = ((end.M24 - start.M24) * amount) + start.M24;
	result.M31 = ((end.M31 - start.M31) * amount) + start.M31;
	result.M32 = ((end.M32 - start.M32) * amount) + start.M32;
	result.M33 = ((end.M33 - start.M33) * amount) + start.M33;
	result.M34 = ((end.M34 - start.M34) * amount) + start.M34;
	result.M41 = ((end.M41 - start.M41) * amount) + start.M41;
	result.M42 = ((end.M42 - start.M42) * amount) + start.M42;
	result.M43 = ((end.M43 - start.M43) * amount) + start.M43;
	result.M44 = ((end.M44 - start.M44) * amount) + start.M44;
}

/*
static inline void Mat4x4Billboard(const Vector3D& objectPosition, const Vector3D& cameraPosition, const Vector3D& cameraUpVector, const Vector3D& cameraForwardVector, Matrix4x4& matrix) {
	Vector3D eyeDir = objectPosition - cameraPosition;
	Vector3D right = eyeDir;
	Vector3D result;
	Vector3D Vector3D;

	float x = right.X;
	float y = right.Y;
	float z = right.Z;

	float num = (((x * x) + (y * y)) + (z * z));
	if (num < 9.99999974737875f) {
		right = -(cameraForwardVector);
	} else {
		right = (Vector3D)(eyeDir * ((1.0f / sqrtf(num))));
	}
	Vec3Cross(cameraUpVector, right, result);
	Vec3Normalize(&result);
	Vec3Cross(right, result, Vector3D);

	matrix.M11 = Vector3D.X;
	matrix.M12 = Vector3D.Y;
	matrix.M13 = Vector3D.Z;
	matrix.M14 = 0.0f;
	matrix.M21 = result.X;
	matrix.M22 = result.Y;
	matrix.M23 = result.Z;
	matrix.M24 = 0.0f;
	matrix.M31 = right.X;
	matrix.M32 = right.Y;
	matrix.M33 = right.Z;
	matrix.M34 = 0.0f;
	matrix.M41 = objectPosition.X;
	matrix.M42 = objectPosition.Y;
	matrix.M43 = objectPosition.Z;
	matrix.M44 = 1.0f;
}*/
static inline void Mat4x4LookAtLH(const Vector3D& cameraPosition, const Vector3D& cameraTarget, const Vector3D& cameraUpVector, Matrix4x4& matrix) {

	Vector3D camDir = cameraTarget - cameraPosition;
	Vec3Normalize(&camDir);


	// Vector3D right = Vector3D.Normalize(Vector3D.Cross(cameraUpVector, camDir));

	Vector3D right;
	Vec3Cross(cameraUpVector, camDir, right);
	Vec3Normalize(&right);


	Vector3D up;
	Vec3Cross(right, camDir, up);

	matrix.M11 = right.X;
	matrix.M12 = up.X;
	matrix.M13 = camDir.X;
	matrix.M14 = 0.0f;
	matrix.M21 = right.Y;
	matrix.M22 = up.Y;
	matrix.M23 = camDir.Y;
	matrix.M24 = 0.0f;
	matrix.M31 = right.Z;
	matrix.M32 = up.Z;
	matrix.M33 = camDir.Z;
	matrix.M34 = 0.0f;
	matrix.M41 = -Vec3Dot(&right, &cameraPosition);
	matrix.M42 = -Vec3Dot(&up, &cameraPosition);
	matrix.M43 = -Vec3Dot(&camDir, &cameraPosition);
	matrix.M44 = 1.0f;
}
static inline void Mat4x4Decompose(Matrix4x4& matrix, Vector3D& position, Vector3D& scale, Quaternion& rot) {
	//compute scaling
	Vector3D Row0(matrix.M11, matrix.M12, matrix.M13);
	Vector3D Row1(matrix.M21, matrix.M22, matrix.M23);
	Vector3D Row2(matrix.M31, matrix.M32, matrix.M33);
	scale.X = Vec3Length(&Row0);
	scale.Y = Vec3Length(&Row1);
	scale.Z = Vec3Length(&Row2);

	//translation
	position.X = matrix.M41;
	position.Y = matrix.M42;
	position.Z = matrix.M43;

	//rotation
	QuatRotationMatrix(matrix, rot);

}
static inline void Mat4x4PerspectiveFOV(float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance, Matrix4x4& matrix) {

	/*if ((fieldOfView <= 0.0f) || (fieldOfView >= (2.0f * 3.1415926535f)))
	{
	throw new ArgumentOutOfRangeException("FieldOfView is not correct!");
	}
	if (nearPlaneDistance <= 0.0f)
	{
	throw new ArgumentOutOfRangeException("Near plane distance must be greater than zero");
	}
	if (farPlaneDistance <= 0.0f)
	{
	throw new ArgumentOutOfRangeException("Far plane distance must be greater than zero");
	}
	if (nearPlaneDistance >= farPlaneDistance)
	{
	throw new ArgumentOutOfRangeException("Distance to the far plane must be greater than distance to the near plane!");
	}*/
	float num = 1.0f / (tanf(fieldOfView * 0.5f));
	float num9 = num / aspectRatio;
	matrix.M11 = num9;
	matrix.M12 = matrix.M13 = matrix.M14 = 0.0f;
	matrix.M22 = num;
	matrix.M21 = matrix.M23 = matrix.M24 = 0.0f;
	matrix.M31 = matrix.M32 = 0.0f;
	matrix.M33 = farPlaneDistance / (farPlaneDistance - nearPlaneDistance);
	matrix.M34 = 1.0f;
	matrix.M41 = matrix.M42 = matrix.M44 = 0.0f;
	matrix.M43 = (-nearPlaneDistance * farPlaneDistance) / (farPlaneDistance - nearPlaneDistance);
}
//-----Quaternion-----
static inline void QuatIdentity(Quaternion& pDst) {
	pDst.X = pDst.Y = pDst.Z = 0.0f;
	pDst.W = 1.0f;
}
static inline void QuatLerp(const Quaternion& quaternion1, const Quaternion& quaternion2, float amount, Quaternion& result) {
	float num = amount;
	float num2 = 1.0f - num;
	float num5 = (((quaternion1.X * quaternion2.X) + (quaternion1.Y * quaternion2.Y)) + (quaternion1.Z * quaternion2.Z)) + (quaternion1.W * quaternion2.W);
	if (num5 >= 0.0f) {
		result.X = (num2 * quaternion1.X) + (num * quaternion2.X);
		result.Y = (num2 * quaternion1.Y) + (num * quaternion2.Y);
		result.Z = (num2 * quaternion1.Z) + (num * quaternion2.Z);
		result.W = (num2 * quaternion1.W) + (num * quaternion2.W);
	} else {
		result.X = (num2 * quaternion1.X) - (num * quaternion2.X);
		result.Y = (num2 * quaternion1.Y) - (num * quaternion2.Y);
		result.Z = (num2 * quaternion1.Z) - (num * quaternion2.Z);
		result.W = (num2 * quaternion1.W) - (num * quaternion2.W);
	}
	float num4 = (((result.X * result.X) + (result.Y * result.Y)) + (result.Z * result.Z)) + (result.W * result.W);
	float num3 = 1.0f / sqrtf(num4);
	result.X *= num3;
	result.Y *= num3;
	result.Z *= num3;
	result.W *= num3;
}
inline void QuatRotationYawPitchRoll(float yaw, float pitch, float roll, Quaternion& result) {
	float num13 = roll * 0.5f;
	float num2 = sinf(num13);
	float num = cosf(num13);
	float num12 = pitch * 0.5f;
	float num11 = sinf(num12);
	float num10 = cosf(num12);
	float num9 = yaw * 0.5f;
	float num8 = sinf(num9);
	float num7 = cosf(num9);
	float num6 = num7 * num11;
	float num5 = num8 * num10;
	result.X = ((num2 * num5) + (num * num6));
	result.Y = ((num * num5) - (num2 * num6));
	float num4 = num7 * num10;
	float num3 = num8 * num11;
	result.Z = ((num2 * num4) - (num * num3));
	result.W = ((num2 * num3) + (num * num4));
}

static inline float QuatLength(const Quaternion& src) {
	return sqrtf((src.X * src.X) + (src.Y * src.Y) + (src.Z * src.Z) + (src.W * src.W));
}
static inline void QuatNormalize(Quaternion& src) {
	float num = (1.0f / QuatLength(src));
	src.X *= num;
	src.Y *= num;
	src.Z *= num;
	src.W *= num;
}
static inline void QuatSetAxisAngle(const Vector3D& axis, float theta, Quaternion& dst) {
	dst.X = sinf(theta / 2.0f) * axis.X;
	dst.Y = sinf(theta / 2.0f) * axis.Y;
	dst.Z = sinf(theta / 2.0f) * axis.Z;
	dst.W = cosf(theta / 2.0f);
}
inline void QuatRotationMatrix(const Matrix4x4& matrix, Quaternion& dst) {
	float num8 = (matrix.M11 + matrix.M22) + matrix.M33;
	if (num8 > 0.0f) {
		float num = sqrtf(num8 + 1.0f);
		dst.W = num * 0.5f;
		num = 0.5f / num;
		dst.X = (matrix.M23 - matrix.M32) * num;
		dst.Y = (matrix.M31 - matrix.M13) * num;
		dst.Z = (matrix.M12 - matrix.M21) * num;
	}
	if ((matrix.M11 >= matrix.M22) && (matrix.M11 >= matrix.M33)) {
		float num7 = sqrtf((((1.0f + matrix.M11) - matrix.M22) - matrix.M33));
		float num4 = 0.5f / num7;
		dst.X = 0.5f * num7;
		dst.Y = (matrix.M12 + matrix.M21) * num4;
		dst.Z = (matrix.M13 + matrix.M31) * num4;
		dst.W = (matrix.M23 - matrix.M32) * num4;
	}
	if (matrix.M22 > matrix.M33) {
		float num6 = sqrtf((((1.0f + matrix.M22) - matrix.M11) - matrix.M33));
		float num3 = 0.5f / num6;
		dst.X = (matrix.M21 + matrix.M12) * num3;
		dst.Y = 0.5f * num6;
		dst.Z = (matrix.M32 + matrix.M23) * num3;
		dst.W = (matrix.M31 - matrix.M13) * num3;
	}
	float num5 = sqrtf((((1.0f + matrix.M33) - matrix.M11) - matrix.M22));
	float num2 = 0.5f / num5;
	dst.X = (matrix.M31 + matrix.M13) * num2;
	dst.Y = (matrix.M32 + matrix.M23) * num2;
	dst.Z = 0.5f * num5;
	dst.W = (matrix.M12 - matrix.M21) * num2;
}
static inline void QuatInvert(Quaternion& src) {
	float num2 = (((src.X * src.X) + (src.Y * src.Y)) + (src.Z * src.Z)) + (src.W * src.W);
	float num = 1.0f / num2;
	src.X = -src.X * num;
	src.Y = -src.Y * num;
	src.Z = -src.Z * num;
	src.W = src.W * num;
}
//-----Plane-----

/*
static inline bool PlaneIntersectLine(const Plane& plane, const Vector3D& start, const Vector3D& dir, float& dist) {
	dist = 0;
	float num2 = ((plane.Normal.X * dir.X) + (plane.Normal.Y * dir.Y)) + (plane.Normal.Z * dir.Z);
	if (fabs(num2) < 1E-05f) {
		return false;
	}
	float num3 = ((plane.Normal.X * start.X) + (plane.Normal.Y * start.Y)) + (plane.Normal.Z * start.Z);
	float num = (-plane.D - num3) / num2;
	if (num < 0.0f) {
		if (num < -1E-05f) {
			return false;
		}
		num = 0.0f;
	}
	dist = num;
	return true;
}
static inline void PlaneNormalize(Plane& plane) {
	float num2 = ((plane.Normal.X * plane.Normal.X) + (plane.Normal.Y * plane.Normal.Y)) + (plane.Normal.Z * plane.Normal.Z);
	if (fabs(num2 - 1.0f) >= 1.192093E-07f) {
		float num = 1.0f / sqrtf(num2);
		plane.Normal.X *= num;
		plane.Normal.Y *= num;
		plane.Normal.Z *= num;
		plane.D *= num;
	}
}
static inline void PlaneTransform(Plane& plane, const Matrix4x4& matrix) {
	Matrix4x4 matrix2 = matrix;
	Mat4x4Invert(&matrix2);
	float x = plane.Normal.X;
	float y = plane.Normal.Y;
	float z = plane.Normal.Z;
	float d = plane.D;
	plane.Normal.X = (((x * matrix2.M11) + (y * matrix2.M12)) + (z * matrix2.M13)) + (d * matrix2.M14);
	plane.Normal.Y = (((x * matrix2.M21) + (y * matrix2.M22)) + (z * matrix2.M23)) + (d * matrix2.M24);
	plane.Normal.Z = (((x * matrix2.M31) + (y * matrix2.M32)) + (z * matrix2.M33)) + (d * matrix2.M34);
	plane.D = (((x * matrix2.M41) + (y * matrix2.M42)) + (z * matrix2.M43)) + (d * matrix2.M44);
}
static inline void PlaneTransform(const Plane& plane, const Matrix4x4& matrix, Plane& result) {
	Matrix4x4 matrix2 = matrix;
	Mat4x4Invert(&matrix2);
	float x = plane.Normal.X;
	float y = plane.Normal.Y;
	float z = plane.Normal.Z;
	float d = plane.D;
	result.Normal.X = (((x * matrix2.M11) + (y * matrix2.M12)) + (z * matrix2.M13)) + (d * matrix2.M14);
	result.Normal.Y = (((x * matrix2.M21) + (y * matrix2.M22)) + (z * matrix2.M23)) + (d * matrix2.M24);
	result.Normal.Z = (((x * matrix2.M31) + (y * matrix2.M32)) + (z * matrix2.M33)) + (d * matrix2.M34);
	result.D = (((x * matrix2.M41) + (y * matrix2.M42)) + (z * matrix2.M43)) + (d * matrix2.M44);
}

*/
static inline void Get4x4Offsets(int destTextureWidth, int destTextureHeight, Vector4D* output) {
	float tu = 1.0f / destTextureWidth;
	float tv = 1.0f / destTextureHeight;

	int index = 0;
	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			output[index].X = (x - 1.5f) * tu;
			output[index].Y = (y - 1.5f) * tv;

			index++;
		}
	}
}
static inline void Get3x3Offsets(int destTextureWidth, int destTextureHeight, Vector4D* output) {
	float sampleOffset = 1.0f;
	float tu = 1.0f / destTextureWidth * sampleOffset;
	float tv = 1.0f / destTextureHeight * sampleOffset;

	int index = 0;
	for (int x = 0; x < 3; x++) {
		for (int y = 0; y < 3; y++) {
			output[index].X = (x - 1.5f) * tu;
			output[index].Y = (y - 1.5f) * tv;

			index++;
		}
	}
}
static inline void Get2x2Offsets(int destTextureWidth, int destTextureHeight, Vector4D* output) {
	float sampleOffset = 1.0f;
	float tu = 1.0f / destTextureWidth * sampleOffset;
	float tv = 1.0f / destTextureHeight * sampleOffset;

	//Vector4D[] offsets = new Vector4D[4];
	int index = 0;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			output[index].X = (x - 0.5f) * tu;
			output[index].Y = (y - 0.5f) * tv;

			index++;
		}
	}
}
static inline void GetBlurWeightedOffsets(float numSamples, int destTextureDimensionLength, Vector4D* output) {
	float tuv = 1.0f / destTextureDimensionLength;

	// Each weight vector holds an offset and a weight.
	//   Vector4D[] weightedOffsets = new Vector4D[(int)numSamples];

	float offset = -(numSamples - 1.0f) / 2.0f;
	for (int i = 0; i < numSamples; i++) {
		output[i].X = offset * tuv;
		offset++;

		/*
		* To map 'i' into [-1,1]: (i - (numSamples - 1.0f)/2.0f)/(numSamples - 1.0f)/2.0f
		* Which is equivalent to: 2.0f * i/(numSamples - 1.0f) - 1.0f
		*/
		float x = 2.0f * i / (numSamples - 1.0f) - 1.0f;
		/*
		* A simple quadratic curve distrubtion. Note that this leads to a generally more pronounced
		* effect than the commonly used gaussian distribution function.
		*/
		output[i].Y = -0.25f * x * x + 0.225f;
	}
}
#endif // LMath_h__
