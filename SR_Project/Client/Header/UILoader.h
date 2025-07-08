#pragma once

BEGIN(Engine)
class InventoryManager;
class UIManager;
class ObjectManager;
END

class Player;

class UILoader
{
public:
	void LoadUI(ObjectManager* objMgr);

	void Update(float dt);

	void BuildWorldMapUI(ObjectManager* objMgr);
};