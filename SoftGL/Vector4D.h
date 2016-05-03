#ifndef __VECTOR4D__
#define __VECTOR4D__
//#include "Math.h"


#undef INLINE
#ifdef _WIN32
#ifdef _MSC_VER
#define INLINE __forceinline
#endif
#else
#ifdef __GNUC__ 
#define INLINE __attribute__((always_inline))
#endif
#endif

#ifndef INLINE
#define INLINE inline
#endif


	struct Vector4D;
	struct Vector4D;

	typedef struct Vector4D
	{
	public:
		float X, Y, Z, W;
		INLINE Vector4D(float x, float y, float z, float w)
		{
			this->X = x;
			this->Y = y;
			this->Z = z;
			this->W = w;
		}
		INLINE Vector4D(void)
		{
			this->X = 0.0f;
			this->Y = 0.0f;
			this->Z = 0.0f;
			this->W = 0.0f;
		}

		INLINE void Saturate()
		{
			if(X > 1.0f)
				X = 1.0f;
			if(Y > 1.0f)
				Y = 1.0f;
			if(Z > 1.0f)
				Z = 1.0f;
			if(W > 1.0f)
				W = 1.0f;

			if(X < 0.0f)
				X = 0.0f;
			if(Y < 0.0f)
				Y = 0.0f;
			if(Z < 0.0f)
				Z = 0.0f;
			if(W < 0.0f)
				W = 0.0f;
		}

		INLINE Vector4D(float val)
		{
			this->X = val;
			this->Y = val;
			this->Z = val;
			this->W = val;
		}
		//unary operators
		INLINE Vector4D operator -() const
		{
			return Vector4D(-X, -Y, -Z, -W);
		}

		INLINE Vector4D operator +() const
		{
			return *this;
		}
		INLINE Vector4D& operator=(const Vector4D& other) 
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			W = other.W;
			return *this;
		}
		//binary operators
		INLINE Vector4D operator +(const Vector4D &other) const
		{
			return Vector4D(X + other.X, Y + other.Y, Z + other.Z, W + other.W);
		}

		INLINE Vector4D operator *(const Vector4D &other) const
		{
			return Vector4D(X * other.X, Y * other.Y, Z * other.Z, W * other.W);
		}


		INLINE Vector4D operator -(const Vector4D &other) const
		{
			return Vector4D(X - other.X, Y - other.Y, Z - other.Z, W - other.W);
		}
		INLINE Vector4D operator *(float other) const
		{
			return Vector4D(X * other, Y * other, Z * other, W * other);
		}
		INLINE Vector4D operator /(float other) const
		{
			return Vector4D(X / other, Y / other, Z / other, W / other);
		}

		INLINE operator float * ()
		{
			return &X;
		}
		INLINE operator const float * () const
		{
			return &X;
		}
		INLINE bool operator == (const Vector4D& vector) const	
		{
			return (X == vector.X && Y == vector.Y && Z == vector.Z && W == vector.W);
		}

		INLINE bool operator != (const Vector4D& vector) const
		{	
			return (X != vector.X || Y != vector.Y || Z != vector.Z || W != vector.W);
		}
		INLINE void Increment(const Vector4D& val)
		{
			X += val.X;
			Y += val.Y;
			Z += val.Z;
			W += val.W;
		}
		INLINE Vector4D& operator += (const Vector4D& Vector)
		{
			X += Vector.X;
			Y += Vector.Y;
			Z += Vector.Z;
			W += Vector.W;
			return *this;
		}
		INLINE Vector4D& operator -= (const Vector4D& Vector)
		{
			X -= Vector.X;
			Y -= Vector.Y;
			Z -= Vector.Z;
			W -= Vector.W;
			return *this;
		}
		INLINE Vector4D& operator *= (float Value)
		{
			X *= Value;
			Y *= Value;
			Z *= Value;
			W *= Value;
			return *this;
		}
		INLINE Vector4D& operator /= (float Value)
		{
			//assert(Value && "Zero Value");
			*this *= (1.0f / Value);
			return *this;
		}
		INLINE Vector4D& operator += (float Value)
		{
			X += Value;
			Y += Value;
			Z += Value;
			W += Value;
			return *this;
		}
		INLINE Vector4D& operator -= (float Value)
		{
			X -= Value;
			Y -= Value;
			Z -= Value;
			W -= Value;
			return *this;
		}		
		INLINE Vector4D& operator /= (const Vector4D& vector)
		{
			X /= vector.X;
			Y /= vector.Y;
			Z /= vector.Z;
			W /= vector.W;
			return *this;
		}
		INLINE Vector4D& operator *= (const Vector4D& vector)
		{
			X *= vector.X;
			Y *= vector.Y;
			Z *= vector.Z;
			W *= vector.W;
			return *this;
		}

		INLINE float& operator [](int index)
		{
			//assert(index < 3 && "Out Of Range");
			return *(index + (&X));
		}
		INLINE float operator [] (int index) const
		{
			//assert(index < 2 && "Out Of Range");
			return * (index + (&X));
		}
		INLINE bool operator > (const Vector4D& v) const
		{
			return X > v.X && Y > v.Y && Z > v.Z && W > v.W;
		}
		INLINE bool operator < (const Vector4D& v) const
		{
			return X < v.X && Y < v.Y && Z < v.Z && W < v.W;
		}


	}Vector4D, *PVector4D, **LPVector4D;


#endif
