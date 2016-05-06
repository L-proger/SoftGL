#ifndef Plane_h__
#define Plane_h__
#include "float3.h"
#include "float4.h"
#include <cmath>

class Plane {
public:
	float3 Normal;
	float D;
	Plane(float a, float b, float c, float d) {
		Normal.X = a;
		Normal.Y = b;
		Normal.Z = c;
		D = d;
	}
	void Set(float a, float b, float c, float d) {
		Normal.X = a;
		Normal.Y = b;
		Normal.Z = c;
		D = d;
	}
	Plane() {
		Normal.X = 0.0f;
		Normal.Y = 1.0f;
		Normal.Z = 0.0f;
		D = 0.0f;
	}

	Plane(float3& _normal, float d) {
		Normal = _normal;
		D = d;
	}

	Plane(float3& point, float3& normal) {
		Normal = normal;
		D = -(((normal.Y * point.Y) + (normal.X * point.X)) + (normal.Z * point.Z));
	}

	Plane(float4& val) {
		Normal.X = val.X;
		Normal.Y = val.Y;
		Normal.Z = val.Z;
		D = val.W;
	}
	Plane(float3& point1, float3& point2, float3& point3) {
		float num10 = point2.X - point1.X;
		float num9 = point2.Y - point1.Y;
		float num8 = point2.Z - point1.Z;
		float num7 = point3.X - point1.X;
		float num6 = point3.Y - point1.Y;
		float num5 = point3.Z - point1.Z;
		float num4 = (num9 * num5) - (num8 * num6);
		float num3 = (num8 * num7) - (num10 * num5);
		float num2 = (num10 * num6) - (num9 * num7);
		float num11 = ((num4 * num4) + (num3 * num3)) + (num2 * num2);
		float num = 1.0f / sqrtf(num11);
		Normal.X = num4 * num;
		Normal.Y = num3 * num;
		Normal.Z = num2 * num;
		D = -(((Normal.X * point1.X) + (Normal.Y * point1.Y)) + (Normal.Z * point1.Z));
	}
	bool operator ==(const Plane& other) const {
		return (other.Normal == this->Normal && other.D == this->D);
	}


	bool operator !=(const Plane& other) const {
		if (this->Normal == other.Normal) {
			return (this->D != other.D);
		}
		return true;
	}
};

#endif // Plane_h__
