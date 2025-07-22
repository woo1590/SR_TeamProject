#pragma once

#include "Object.h"

class ShopEmerald : public Object
{
private:
	ShopEmerald(ObjectManager* owner): Object(owner, ObjectType::UI) {}

public:
	static ShopEmerald* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

