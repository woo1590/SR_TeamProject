#pragma once

#include "Object.h"

class UIObj : public Object
{
private:
	explicit UIObj(ObjectManager* owner, ObjectType objType);
	virtual ~UIObj() = default;

public:
	static UIObj* Create(ObjectManager* owner, ObjectType objType);
	HRESULT Ready_Object() override;
	void Update(float dt) override;
	void Late_Update(float dt) override;

private:
	virtual void Free() override;
};

