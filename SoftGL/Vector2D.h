#ifndef __Vector2D__
#define __Vector2D__


typedef struct Vector2D {
public:
	float X, Y;
	inline Vector2D(float x, float y) {
		this->X = x;
		this->Y = y;
	}
	inline Vector2D(void) {
		this->X = 0.0f;
		this->Y = 0.0f;
	}

	inline Vector2D(float val) {
		this->X = val;
		this->Y = val;
	}
	inline Vector2D& operator=(const Vector2D& other) {
		X = other.X;
		Y = other.Y;
		return *this;
	}
	//unary operators
	inline Vector2D operator -() const {
		return Vector2D(-X, -Y);
	}

	inline Vector2D operator +() const {
		return *this;
	}

	//binary operators
	inline Vector2D operator +(const Vector2D &other) const {
		return Vector2D(X + other.X, Y + other.Y);
	}
	inline Vector2D operator -(const Vector2D &other) const {
		return Vector2D(X - other.X, Y - other.Y);
	}
	inline Vector2D operator *(float other) const {
		return Vector2D(X * other, Y * other);
	}
	inline Vector2D operator /(float other) const {
		return Vector2D(X / other, Y / other);
	}

	inline operator float * () {
		return &X;
	}
	inline operator const float * () const {
		return &X;
	}
	inline bool operator == (const Vector2D& vector) const {
		return (X == vector.X && Y == vector.Y);
	}

	inline bool operator != (const Vector2D& vector) const {
		return (X != vector.X || Y != vector.Y);
	}
	inline Vector2D& operator += (const Vector2D& Vector) {
		X += Vector.X;
		Y += Vector.Y;
		return *this;
	}
	inline Vector2D& operator -= (const Vector2D& Vector) {
		X -= Vector.X;
		Y -= Vector.Y;
		return *this;
	}
	inline Vector2D& operator *= (float Value) {
		X *= Value;
		Y *= Value;
		return *this;
	}
	inline Vector2D& operator /= (float Value) {
		//assert(Value && "Zero Value");
		*this *= (1.0f / Value);
		return *this;
	}
	inline Vector2D& operator += (float Value) {
		X += Value;
		Y += Value;
		return *this;
	}
	inline Vector2D& operator -= (float Value) {
		X -= Value;
		Y -= Value;
		return *this;
	}
	inline Vector2D& operator /= (const Vector2D& vector) {
		X /= vector.X;
		Y /= vector.Y;
		return *this;
	}
	inline Vector2D& operator *= (const Vector2D& vector) {
		X *= vector.X;
		Y *= vector.Y;
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
	inline bool operator > (const Vector2D& v) const {
		return X > v.X && Y > v.Y;
	}
	inline bool operator < (const Vector2D& v) const {
		return X < v.X && Y < v.Y;
	}
} Vector2D, *PVector2D, **LPVector2D;


#endif
