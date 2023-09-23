#pragma once

#include "camera.h"
#include <LFramework/Input/Input.h>
#include <LMath/lmath.h>

class CameraController{
public:
	float mouse_speed;
	float move_speed = 3.0f;
	static constexpr float pi = 3.14159265358979f;

	CameraController(Camera* camera) : mouse_speed(0.01f), _camera(camera) {
		_angle_x = _angle_y = 0.0f;
	}

	void Tick(float deltaTime)  {
		auto mice = Input::Instance()->mice();
		auto mouse = mice[1];

		if(mouse->GetKeyState(1)){
			_angle_x += mouse_speed * mouse->GetDx();
			_angle_y += mouse_speed * mouse->GetDy();
			_angle_y = clamp(_angle_y, -pi / 2.0f, pi / 2.0f);	
			auto rv = lm::Quaternion_f::angleAxis(_angle_y, lm::float3(1, 0, 0));
			auto rh = lm::Quaternion_f::angleAxis(_angle_x, lm::float3(0, 1, 0));
			_camera->GameObject()->transform.set_local_rotation(lm::mul(rh, rv));
		}
	
		auto keyboard = Input::Instance()->keyboards()[0];
		auto& tf = _camera->GameObject()->transform;

		if (keyboard->GetKeyState(0x57)) {
			tf.SetLocalPosition(tf.GetLocalPosition() + tf.Forward() * (deltaTime * move_speed));
		}

		if (keyboard->GetKeyState(0x53)) {
			tf.SetLocalPosition(tf.GetLocalPosition() - tf.Forward() * (deltaTime * move_speed));
		}

		if (keyboard->GetKeyState(0x41)) {
			tf.SetLocalPosition(tf.GetLocalPosition() - tf.right() * (deltaTime * move_speed));
		}

		if (keyboard->GetKeyState(0x44)) {
			tf.SetLocalPosition(tf.GetLocalPosition() + tf.right() * (deltaTime * move_speed));
		}
	}
private:
	Camera* _camera;
	float _angle_x;
	float _angle_y;
};
