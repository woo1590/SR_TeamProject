#pragma once

#include "Object.h"

class QuestTextObj : public Object
{
private:
	QuestTextObj(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {}

public:
	static QuestTextObj* Create(ObjectManager* owner);
	HRESULT Ready_Object();
};

