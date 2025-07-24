#pragma once

#include "Object.h"
#include "DialogRect.h"

class LoadingSpinner : public Object
{
private:
	LoadingSpinner(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static LoadingSpinner* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

private:
	vector<DialogRect*> dots;
	float animTime = 0.f;
};

