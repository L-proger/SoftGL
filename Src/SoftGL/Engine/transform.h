#pragma once

#include <LMath/lmath.h>

using namespace lm;

class Game_object;

class Transform {
public:
	Transform* parent;
	lm::Quaternion_f _rotation;
	float3 _position;
	float3 _scale;

	Transform(Game_object* game_object)
		:parent(nullptr), _scale(1, 1, 1), _position(0, 0, 0), _rotation(0, 0, 0, 1), _game_object(game_object) {

	}

	std::wstring class_name() const {
		return L"Transform";
	}

	void SetLocalPosition(const float3& position) {
		_position = position;
	}
	float3 GetLocalPosition() {
		return _position;
	}

	float3 Forward() const {
		return get_global_transform()[2].xyz();
	}

	float3 right() const {
		return get_global_transform()[0].xyz();
	}

	float3 up() const {
		return get_global_transform()[1].xyz();
	}

	void set_local_rotation(const Quaternion_f& rotation) {
		_rotation = rotation;
	}

	Quaternion_f get_local_rotation() {
		return _rotation;
	}

	void set_local_scale(const float3& scale) {
		_scale = scale;
	}

	float3 get_local_scale() {
		return _scale;
	}

	float4x4 get_local_transform() const {
		float4x4 scaleMatrix = matrix4x4Scale(_scale);
		float4x4 rotationMatrix = matrix4x4RotationQuaternion(_rotation);
		float4x4 positionMatrix = matrix4x4Translation(_position);
		return lm::mul(lm::mul(scaleMatrix, rotationMatrix), positionMatrix);
	}

	float4x4 get_global_transform() const {
		if (parent == nullptr) {
			return get_local_transform();
		}
		return lm::mul(get_local_transform(), parent->get_global_transform());
	}

	float3 get_global_position() {
		auto tf = get_global_transform();
		return float3(tf[3][0], tf[3][1], tf[3][2]);
	}

	void RotateLocal(const Quaternion_f& rotation) {
		_rotation = lm::mul(_rotation, rotation);
	}
private:
	Game_object* _game_object;
};
