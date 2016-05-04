#ifndef __VECTOR3D__
#define __VECTOR3D__

#include <cmath>

typedef struct Vector3D {
public:
	float X, Y, Z;

	inline Vector3D(float x, float y, float z) {
		this->X = x;
		this->Y = y;
		this->Z = z;
	}
	inline Vector3D(void) {
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}
	inline float Length() const {
		return sqrtf(this->X * this->X + this->Y * this->Y + this->Z * this->Z);
	}

	inline void Saturate() {
		if (X > 1.0f)
			X = 1.0f;
		else if (X < 0.0f)
			X = 0.0f;

		if (Y > 1.0f)
			Y = 1.0f;
		else if (Y < 0.0f)
			Y = 0.0f;

		if (Z > 1.0f)
			Z = 1.0f;
		else if (Z < 0.0f)
			Z = 0.0f;
	}


	inline void Normalize() {
		float num2 = Length();
		if (num2 != 0.0f) {
			float num = 1.0f / num2;
			this->X *= num;
			this->Y *= num;
			this->Z *= num;
		}
	}


	inline static Vector3D FromSpherical(float theta, float phi) {
		float fSinTheta = sin(theta);
		return Vector3D(fSinTheta * cosf(phi), fSinTheta * sinf(phi), cosf(theta));
	}


	inline static Vector3D ToSpherical(const Vector3D& vec) {
		Vector3D normalized_v = vec;
		normalized_v.Normalize();
		return Vector3D(acosf(vec.Z), atan2f(vec.Y, vec.X), 1.0f);
	}


	inline static Vector3D reflect(const Vector3D& normal, const Vector3D& incident) {
		float cosine = normal.X * incident.X + normal.Y * incident.Y + normal.Z * incident.Z;
		return incident - normal * (2.0f * cosine);
	}

	inline Vector3D(float val) {
		this->X = val;
		this->Y = val;
		this->Z = val;
	}

	inline Vector3D& operator=(const Vector3D& other) {
		X = other.X;
		Y = other.Y;
		Z = other.Z;
		return *this;
	}
	//unary operators
	inline Vector3D operator -() const {
		return Vector3D(-X, -Y, -Z);
	}

	inline Vector3D operator +() const {
		return *this;
	}

	//binary operators
	inline Vector3D operator +(const Vector3D &other) const {
		return Vector3D(X + other.X, Y + other.Y, Z + other.Z);
	}
	inline Vector3D operator -(const Vector3D &other) const {
		return Vector3D(X - other.X, Y - other.Y, Z - other.Z);
	}
	inline Vector3D operator *(float other) const {
		return Vector3D(X * other, Y * other, Z * other);
	}
	inline Vector3D operator /(float other) const {
		return Vector3D(X / other, Y / other, Z / other);
	}

	inline operator float * () {
		return &X;
	}
	inline operator const float * () const {
		return &X;
	}
	inline bool operator == (const Vector3D& vector) const {
		return (X == vector.X && Y == vector.Y && Z == vector.Z);
	}

	inline bool operator != (const Vector3D& vector) const {
		return (X != vector.X || Y != vector.Y || Z != vector.Z);
	}
	inline Vector3D& operator += (const Vector3D& Vector) {
		X += Vector.X;
		Y += Vector.Y;
		Z += Vector.Z;
		return *this;
	}
	inline Vector3D& operator -= (const Vector3D& Vector) {
		X -= Vector.X;
		Y -= Vector.Y;
		Z -= Vector.Z;
		return *this;
	}
	inline Vector3D& operator *= (float Value) {
		X *= Value;
		Y *= Value;
		Z *= Value;
		return *this;
	}
	inline Vector3D& operator /= (float Value) {
		//assert(Value && "Zero Value");
		*this *= (1.0f / Value);
		return *this;
	}
	inline Vector3D& operator += (float Value) {
		X += Value;
		Y += Value;
		Z += Value;
		return *this;
	}
	inline Vector3D& operator -= (float Value) {
		X -= Value;
		Y -= Value;
		Z -= Value;
		return *this;
	}
	inline Vector3D& operator /= (const Vector3D& vector) {
		X /= vector.X;
		Y /= vector.Y;
		Z /= vector.Z;
		return *this;
	}
	inline Vector3D& operator *= (const Vector3D& vector) {
		X *= vector.X;
		Y *= vector.Y;
		Z *= vector.Z;
		return *this;
	}

	inline float& operator [](int index) {
		//assert(index < 3 && "Out Of Range");
		return *(index + (&X));
	}
	inline float operator [] (int index) const {
		//assert(index < 2 && "Out Of Range");
		return *(index + (&X));
	}
	inline bool operator > (const Vector3D& v) const {
		return X > v.X && Y > v.Y && Z > v.Z;
	}
	inline bool operator < (const Vector3D& v) const {
		return X < v.X && Y < v.Y && Z < v.Z;
	}
} Vector3D, *PVector3D, **LPVector3D;

#endif
