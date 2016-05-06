#ifndef Camera_h__
#define Camera_h__

#include "LMath.h"

class Camera
{
private:
	float3 position;
	float3 direction;
	float3 up;
	float3 right;
	float fov;
	float aspect;

	//0------1
	//
	//
	//2------3
	float3 corners[4];

	void ComputeCorners()
	{
		Vec3Normalize(&direction);
		right = Vec3Cross(float3(0,1,0), direction);
		up = Vec3Cross(direction, right);

		Vec3Normalize(&right);
		Vec3Normalize(&up);

		float farW = tanf(fov / 2.0f);
		float farH = farW / aspect;

		float3 farCenter = position + direction;

		corners[0] = farCenter + (up * farH) + (right * -farW);
		corners[1] = farCenter + (up * farH) + (right * farW);
		corners[2] = farCenter + (up * -farH) + (right * -farW);
		corners[3] = farCenter + (up * -farH) + (right * farW);

	}
public:
	Camera()
		:position(0,0,0), direction(0,0,1), up(0,1,0), fov(3.1415f / 4.0f), aspect(1.0f)
	{
		ComputeCorners();
	}
	float3* GetCorners()
	{
		return &corners[0];
	}
	void SetPosition(const float3& pos)
	{
		position = pos;
		ComputeCorners();
	}
	float3 GetPosition()
	{
		return position;
	}
	float3 GetDirection()
	{
		return direction;
	}
	void SetTargets(const float3& at)
	{
		direction = at - position;
		ComputeCorners();
	}
	void SetFov(float _fov)
	{
		fov = _fov;
		ComputeCorners();
	}
	void SetAspect(float _aspect)
	{
		aspect = _aspect;
		ComputeCorners();
	}
};
#endif // Camera_h__