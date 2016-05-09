#ifndef camera_controller_h__
#define camera_controller_h__

#include "camera.h"
#include "Input.h"
#include "lmath.h"

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

		_angle_x += mouse_speed * mice[1]->get_dx();
		_angle_y += mouse_speed * mice[1]->get_dy();

		_angle_y = clamp(_angle_y, -pi / 2.0f, pi / 2.0f);

		auto rv = lm::Quaternion_f::angle_axis(_angle_y, lm::float3(1, 0, 0));
		auto rh = lm::Quaternion_f::angle_axis(_angle_x, lm::float3(0, 1, 0));

		_camera->GameObject()->transform.set_local_rotation(lm::mul(rh, rv));

		auto keyboard = Input::Instance()->keyboards()[1];
		auto& tf = _camera->GameObject()->transform;


		if (keyboard->get_key_state(0x57)) {
			tf.set_localPosition(tf.get_localPosition() + tf.forward() * (deltaTime * move_speed));
		}

		if (keyboard->get_key_state(0x53)) {
			tf.set_localPosition(tf.get_localPosition() - tf.forward() * (deltaTime * move_speed));
		}

		if (keyboard->get_key_state(0x41)) {
			tf.set_localPosition(tf.get_localPosition() - tf.right() * (deltaTime * move_speed));
		}

		if (keyboard->get_key_state(0x44)) {
			tf.set_localPosition(tf.get_localPosition() + tf.right() * (deltaTime * move_speed));
		}
	}
private:
	Camera* _camera;
	float _angle_x;
	float _angle_y;
};
#endif // camera_controller_h__
