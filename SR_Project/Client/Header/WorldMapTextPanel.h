#pragma once

#include "Object.h"

class WorldMapTextPanel : public Object
{
private:
	WorldMapTextPanel(ObjectManager* owner)
		:Object(owner,ObjectType::UI) {}

public:
	static WorldMapTextPanel* Create(ObjectManager* owner);
	HRESULT Ready_Object();
	void Update(float dt) override;

	void SetText(const wstring& text) { displayText = text; }

private:
	wstring displayText; 
	bool isSelected = false;
	static WorldMapTextPanel* selectedPanel;
};

