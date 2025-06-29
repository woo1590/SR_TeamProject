#pragma once

#include "Object.h"
#include "playerInfoComponent.h"

class Player : public Object
{
public:
	Player(ObjectManager* owner, ObjectType objType)
		:Object(owner, objType) {
	}

	static Player* Create(ObjectManager* owner, ObjectType objType)
	{
		Player* instance = new Player(owner,objType);
		
		if (FAILED(instance->Ready_Object()))
		{
			Safe_Release(instance);
			instance = nullptr;
		}

		return instance;
	}

	HRESULT Ready_Object()
	{
		auto info = AddComponent<PlayerInfoComponent>();
		
		return S_OK;
	}
};

