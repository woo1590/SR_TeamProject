#pragma once

#include "Object.h"

enum class StageSelect {Stage0, Stage1, Stage2};

class MapNode_Front : public Object
{
private:
	MapNode_Front(ObjectManager* owner) : Object(owner, ObjectType::UI) {}

public:
	static MapNode_Front* Create(ObjectManager* owner);
	HRESULT Ready_Object();

	void SetLoadID(LOADID _id) { loadID = _id; }
	LOADID GetLoadID() const { return loadID; }

	void SetStage(StageSelect _stage) { stage = _stage; }
	StageSelect GetStage() const { return stage; }

private:
	LOADID loadID{};
	StageSelect stage = StageSelect::Stage0;
};

