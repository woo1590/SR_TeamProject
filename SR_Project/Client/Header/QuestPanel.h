#pragma once

#include "Object.h"

class QuestPanel : public Object
{
private:
	QuestPanel(ObjectManager* owner)
		:Object(owner, ObjectType::UI){}

public:
	static QuestPanel* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void Update(float dt) override;
};

