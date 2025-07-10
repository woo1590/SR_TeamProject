#pragma once

BEGIN(Engine)
class InventoryManager;
class UIManager;
class ObjectManager;
class TooltipManager;
END

class UILoader
{
public:
    void LoadUI(ObjectManager* objMgr);

private:
    void BuildInventory(ObjectManager* objMgr, InventoryManager* invMgr);
    void BuildPlayerBars(ObjectManager* objMgr);
    void BuildHotbar(ObjectManager* objMgr);
    void BuildQuickSlots(ObjectManager* objMgr);
    void BuildFilters(ObjectManager* objMgr, InventoryManager* invMgr, TooltipManager* tooltipMgr);
    void BuildQuestUI(ObjectManager* objMgr);
    void BuildMiscUI(ObjectManager* objMgr);
    void BuildWorldMapUI(ObjectManager* objMgr);
};
