#pragma once

#include "Object.h"

class EnemyHPBarBack : public Object
{
private:
	EnemyHPBarBack(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static EnemyHPBarBack* Create(ObjectManager* owner);
	HRESULT Ready_Object();

};