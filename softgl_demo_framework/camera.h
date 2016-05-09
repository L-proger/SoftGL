#ifndef camera_h__
#define camera_h__

#include "lmath.h"
#include "gameobject.h"

class Camera {
public:
	Camera(Game_object* object):
		_fov(3.1415f/4.0f), _zNear(0.3f), _zFar(100.0f), _aspect(4.0f/3.0f), _object(object){
		UpdateProjection();
	}

	lm::float4x4 world_to_camera_matrix(){
		auto tf = _object->transform.get_global_transform();
		return lm::matrix4x4_inverse(tf, false);
	}
	Game_object* GameObject()
	{
		return _object;
	}
	float4x4 GetProjection() const{
		return _projection;
	}
	float GetFov() const{
		return _fov;
	}
	void SetFov(float fov) {
		_fov = fov;
		UpdateProjection();
	}
	float GetAspect() const {
		return _aspect;
	}
	void SetAspect(float aspect) {
		_aspect = aspect;
		UpdateProjection();
	}
	float GetZNear() const {
		return _zNear;
	}
	void SetZNear(float zNear) {
		_zNear = zNear;
		UpdateProjection();
	}
	float GetZFar() const {
		return _zFar;
	}
	void SetZFar(float zFar) {
		_zFar = zFar;
		UpdateProjection();
	}
private:
	void UpdateProjection(){
		_projection = lm::matrix4x4_perspective(_fov, _aspect, _zNear, _zFar);
	}
	float _fov;
	float _zNear;
	float _zFar;
	float _aspect;
	float4x4 _projection;
	Game_object* _object;
};
#endif // camera_h__
