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

private:                // 새로 분리한 빌더들
    void BuildCursorAndInventory(ObjectManager* objMgr);
    void BuildPlayerBars(ObjectManager* objMgr);
    void BuildHotbar(ObjectManager* objMgr);
    void BuildQuickSlots(ObjectManager* objMgr);
    void BuildFilters(ObjectManager* objMgr);
    void BuildQuestUI(ObjectManager* objMgr);
    void BuildMiscUI(ObjectManager* objMgr);
    void BuildWorldMapUI(ObjectManager* objMgr);   // 기존 함수 유지
};
