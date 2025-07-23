#pragma once

#include "Object.h"

class ShopBackGround : public Object
{
private:
	ShopBackGround(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static ShopBackGround* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

