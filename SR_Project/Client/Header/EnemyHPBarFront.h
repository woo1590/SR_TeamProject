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

	void SetTarget(TransformComponent* target);
};

