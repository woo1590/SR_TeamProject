#pragma once

#include "Object.h"

class Atri : public Object
{
private:
	Atri(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static Atri* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void SetEmotion(Emotion emotion);
};