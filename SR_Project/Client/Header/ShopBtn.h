#pragma once

#include "Object.h"

class ShopBtn : public Object
{
private:
	ShopBtn(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static ShopBtn* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

