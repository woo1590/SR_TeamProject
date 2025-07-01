#pragma once

#include "Object.h"
#include "Player.h"

class UIDebugObj : public Object
{
private:
	UIDebugObj(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static UIDebugObj* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
	void SetPlayer(Player* _player) { player = _player; }

private:
	Player* player = nullptr;
	float accTime = 0.f;
	int frameCount = 0;
	float fps = 0.f;
};

