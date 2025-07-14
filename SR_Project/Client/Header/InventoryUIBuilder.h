#pragma once

BEGIN(Engine)

class ObjectManager;
class InventoryManager;

END

class  InventoryUIBuilder
{
public:
	static void BuildInventoryUI(ObjectManager* objMgr, InventoryManager* invMgr);

private:
	static void BuildGearAndItemSlots(ObjectManager* objMgr, InventoryManager* invMgr);
	static void BuildQuickSlots(ObjectManager* objMgr, InventoryManager* invMgr); 
	static void BuildInventorySlots(ObjectManager* objMgr, InventoryManager* invMgr);
};

