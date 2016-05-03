#ifndef Primitive_h__
#define Primitive_h__

#include "Ray.h"

class Material
{
public:
	Vector4D Emittence;
	float roughness;
	float reflection;
	virtual ~Material()
	{

	}
	Material()
		:Emittence(0,0,0,0), reflection(1.0f),roughness(1.0f)
	{

	}
	virtual float getBrdf(Vector3D& view, Vector3D& r, Vector3D& n)
	{
		Vector3D half = view + r;
		Vec3Normalize(&half);
		return  powf(clampFloat(Vec3Dot(&half, &n),0.0f, 1.0f), 2.0f);
	}
	virtual Vector4D GetDiffuse(const Vector3D& point)
	{
		return Vector4D(1,1,1,1);
	}
};

class Checker : public Material
{
public:
	Vector4D GetDiffuse(const Vector3D& point)
	{
		return (fmodf(abs(point.Z + 1000.0f), 1.0f) > 0.5f || fmodf(abs(point.X + 1000.0f), 1.0f) > 0.5f)  ? Vector4D(1,1,1,1) : Vector4D(0.5f,0.5f,0.5f,0.5f);
	}
	float getBrdf(Vector3D& view, Vector3D& r, Vector3D& n)
	{
		return 1.0f;
	}
};


struct IntersectionInfo
{
	Vector3D Position;
	Vector3D Normal;
	float Distance;
	Vector4D Color;
	Material* material;

};


class Primitive
{
private:
	Vector3D origin;
public:
	Material* material;
	void SetOrigin(const Vector3D& pos)
	{
		origin = pos;
	}
	Vector3D GetOrigin()
	{
		return origin;
	}
	void SetMaterial(Material* mat)
	{
		material = mat;
	}
	Primitive()
		:origin(0,0,0)
	{
	}
	virtual ~Primitive()
	{

	}
	virtual bool Intersect(const Ray& ray, IntersectionInfo* info) = 0;

};
#endif // Primitive_h__