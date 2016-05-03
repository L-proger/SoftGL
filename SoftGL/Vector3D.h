#ifndef __VECTOR3D__
#define __VECTOR3D__
#include <math.h>


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


	typedef struct Vector3D
	{
	public:
		float X, Y, Z;

		INLINE Vector3D(float x, float y, float z)
		{
			this->X = x;
			this->Y = y;
			this->Z = z;
		}
		INLINE Vector3D(void)
		{
			this->X = 0.0f;
			this->Y = 0.0f;
			this->Z = 0.0f;
		}
		INLINE float Length() const
		{
			return sqrtf(this->X * this->X + this->Y * this->Y + this->Z * this->Z);
		}

		INLINE void Saturate()
		{
			if(X > 1.0f)
				X = 1.0f;
			else if(X < 0.0f)
				X = 0.0f;

			if(Y > 1.0f)
				Y = 1.0f;
			else if(Y < 0.0f)
				Y = 0.0f;

			if(Z > 1.0f)
				Z = 1.0f;
			else if(Z < 0.0f)
				Z = 0.0f;
		}


		INLINE void Normalize()
		{
			float num2 = Length();
			if (num2 != 0.0f)
			{
				float num = 1.0f / num2;
				this->X *= num;
				this->Y *= num;
				this->Z *= num;
			}
		}


		INLINE static Vector3D FromSpherical(float theta, float phi)
		{
			float fSinTheta = sin(theta);
			return Vector3D( fSinTheta * cosf(phi), fSinTheta * sinf(phi), cosf(theta) );
		}


		INLINE static Vector3D ToSpherical(const Vector3D& vec)
		{
			Vector3D normalized_v = vec;
			normalized_v.Normalize();
			return Vector3D(acosf(vec.Z), atan2f(vec.Y, vec.X), 1.0f);
		}


		INLINE static Vector3D reflect(const Vector3D& normal, const Vector3D& incident)
		{
			float cosine = normal.X * incident.X + normal.Y * incident.Y + normal.Z * incident.Z;
			return incident - normal * (2.0f * cosine);
		}

		INLINE Vector3D(float val)
		{
			this->X = val;
			this->Y = val;
			this->Z = val;
		}

		INLINE Vector3D& operator=(const Vector3D& other) 
		{
			X = other.X;
			Y = other.Y;
			Z = other.Z;
			return *this;
		}
		//unary operators
		INLINE Vector3D operator -() const
		{
			return Vector3D(-X, -Y, -Z);
		}

		INLINE Vector3D operator +() const
		{
			return *this;
		}

		//binary operators
		INLINE Vector3D operator +(const Vector3D &other) const
		{
			return Vector3D(X + other.X, Y + other.Y, Z + other.Z);
		}
		INLINE Vector3D operator -(const Vector3D &other) const
		{
			return Vector3D(X - other.X, Y - other.Y, Z - other.Z);
		}
		INLINE Vector3D operator *(float other) const
		{
			return Vector3D(X * other, Y * other, Z * other);
		}
		INLINE Vector3D operator /(float other) const
		{
			return Vector3D(X / other, Y / other, Z / other);
		}

		INLINE operator float * ()
		{
			return &X;
		}
		INLINE operator const float * () const
		{
			return &X;
		}
		INLINE bool operator == (const Vector3D& vector) const	
		{
			return (X == vector.X && Y == vector.Y && Z == vector.Z);
		}

		INLINE bool operator != (const Vector3D& vector) const
		{	
			return (X != vector.X || Y != vector.Y || Z != vector.Z);
		}
		INLINE Vector3D& operator += (const Vector3D& Vector)
		{
			X += Vector.X;
			Y += Vector.Y;
			Z += Vector.Z;
			return *this;
		}
		INLINE Vector3D& operator -= (const Vector3D& Vector)
		{
			X -= Vector.X;
			Y -= Vector.Y;
			Z -= Vector.Z;
			return *this;
		}
		INLINE Vector3D& operator *= (float Value)
		{
			X *= Value;
			Y *= Value;
			Z *= Value;
			return *this;
		}
		INLINE Vector3D& operator /= (float Value)
		{
			//assert(Value && "Zero Value");
			*this *= (1.0f / Value);
			return *this;
		}
		INLINE Vector3D& operator += (float Value)
		{
			X += Value;
			Y += Value;
			Z += Value;
			return *this;
		}
		INLINE Vector3D& operator -= (float Value)
		{
			X -= Value;
			Y -= Value;
			Z -= Value;
			return *this;
		}		
		INLINE Vector3D& operator /= (const Vector3D& vector)
		{
			X /= vector.X;
			Y /= vector.Y;
			Z /= vector.Z;
			return *this;
		}
		INLINE Vector3D& operator *= (const Vector3D& vector)
		{
			X *= vector.X;
			Y *= vector.Y;
			Z *= vector.Z;
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
		INLINE bool operator > (const Vector3D& v) const
		{
			return X > v.X && Y > v.Y && Z > v.Z;
		}
		INLINE bool operator < (const Vector3D& v) const
		{
			return X < v.X && Y < v.Y && Z < v.Z;
		}
	} Vector3D, *PVector3D, **LPVector3D;

#endif
