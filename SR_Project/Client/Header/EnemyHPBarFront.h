#pragma once

#include "Object.h"
#include "TransformComponent.h"

class EnemyHPBarFront : public Object
{
private:
	EnemyHPBarFront(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static EnemyHPBarFront* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

	void SetTarget(TransformComponent* target) { monsterTf = target; }

private:
	_vec2 WorldToScreen(const _vec3& worldPos, float& outZ);

private:
	TransformComponent* monsterTf = nullptr;;
};

