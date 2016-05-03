#ifndef __Vector2D__
#define __Vector2D__
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


	typedef struct Vector2D
	{
	public:
		float X, Y;
		INLINE Vector2D(float x, float y)
		{
			this->X = x;
			this->Y = y;
		}
		INLINE Vector2D(void)
		{
			this->X = 0.0f;
			this->Y = 0.0f;
		}

		INLINE Vector2D(float val)
		{
			this->X = val;
			this->Y = val;
		}
		INLINE Vector2D& operator=(const Vector2D& other)
		{
			X = other.X;
			Y = other.Y;
			return *this;
		}
		//unary operators
		INLINE Vector2D operator -() const
		{
			return Vector2D(-X, -Y);
		}

		INLINE Vector2D operator +() const
		{
			return *this;
		}

		//binary operators
		INLINE Vector2D operator +(const Vector2D &other) const
		{
			return Vector2D(X + other.X, Y + other.Y);
		}
		INLINE Vector2D operator -(const Vector2D &other) const
		{
			return Vector2D(X - other.X, Y - other.Y);
		}
		INLINE Vector2D operator *(float other) const
		{
			return Vector2D(X * other, Y * other);
		}
		INLINE Vector2D operator /(float other) const
		{
			return Vector2D(X / other, Y / other);
		}

		INLINE operator float * ()
		{
			return &X;
		}
		INLINE operator const float * () const
		{
			return &X;
		}
		INLINE bool operator == (const Vector2D& vector) const	
		{
			return (X == vector.X && Y == vector.Y);
		}

		INLINE bool operator != (const Vector2D& vector) const
		{	
			return (X != vector.X || Y != vector.Y);
		}
		INLINE Vector2D& operator += (const Vector2D& Vector)
		{
			X += Vector.X;
			Y += Vector.Y;
			return *this;
		}
		INLINE Vector2D& operator -= (const Vector2D& Vector)
		{
			X -= Vector.X;
			Y -= Vector.Y;
			return *this;
		}
		INLINE Vector2D& operator *= (float Value)
		{
			X *= Value;
			Y *= Value;
			return *this;
		}
		INLINE Vector2D& operator /= (float Value)
		{
			//assert(Value && "Zero Value");
			*this *= (1.0f / Value);
			return *this;
		}
		INLINE Vector2D& operator += (float Value)
		{
			X += Value;
			Y += Value;
			return *this;
		}
		INLINE Vector2D& operator -= (float Value)
		{
			X -= Value;
			Y -= Value;
			return *this;
		}		
		INLINE Vector2D& operator /= (const Vector2D& vector)
		{
			X /= vector.X;
			Y /= vector.Y;
			return *this;
		}
		INLINE Vector2D& operator *= (const Vector2D& vector)
		{
			X *= vector.X;
			Y *= vector.Y;
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
		INLINE bool operator > (const Vector2D& v) const
		{
			return X > v.X && Y > v.Y;
		}
		INLINE bool operator < (const Vector2D& v) const
		{
			return X < v.X && Y < v.Y;
		}
	} Vector2D, *PVector2D, **LPVector2D;

	
#endif
