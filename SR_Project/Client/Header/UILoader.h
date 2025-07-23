#pragma once

BEGIN(Engine)
class InventoryManager;
class UIManager;
class ObjectManager;
class TooltipManager;
class DialogManager;
class Object;
class ShopManager;
END

class UILoader
{
public:
    void LoadUI(ObjectManager* objMgr);

    static Object* CreateInventoryObj(ObjectManager* objMgr, ItemType type);

private:
    void BuildInventory(ObjectManager* objMgr, InventoryManager* invMgr);
    void BuildPlayerBars(ObjectManager* objMgr);
    void BuildHotbar(ObjectManager* objMgr);
    void BuildQuickSlots(ObjectManager* objMgr);
    void BuildFilters(ObjectManager* objMgr, InventoryManager* invMgr, TooltipManager* tooltipMgr);
    void BuildQuestUI(ObjectManager* objMgr);
    void BuildMiscUI(ObjectManager* objMgr);
    void BuildWorldMapUI(ObjectManager* objMgr);
    void BuildDeathUI(ObjectManager* objMgr);
    void BuildDialogUI(ObjectManager* objMgr, DialogManager* dialogMgr);
    void BuildMiniMap(ObjectManager* objMgr);
    void BuildShopUI(ObjectManager* objMgr, InventoryManager* invMgr, TooltipManager* tooltipMgr,ShopManager* shopMgr);
};
