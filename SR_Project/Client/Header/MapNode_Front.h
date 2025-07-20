#pragma once

#include "Object.h"

class MapNode_Front : public Object
{
private:
	MapNode_Front(ObjectManager* owner)
		:Object(owner, ObjectType::UI) {
	}

public:
	static MapNode_Front* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void SetLoadID(LOADID _id) { loadID = _id; }
	LOADID GetLoadID() const { return loadID; }

private:
	LOADID loadID;
};

