#include "pch.h"
#include "UILoader.h"

// Components
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "ProgressBar.h"
#include "SlotComponent.h"
#include "PanelComponent.h"
#include "ButtonComponent.h"
#include "UIRenderer.h"

// Objects
#include "HPBarFront.h"
#include "Player.h"
#include "UIDebugObj.h"
#include "HotBarBack.h"
#include "ExpBarFront.h"
#include "ExpBarBack.h"
#include "Cursor.h"
#include "QuickSlot.h"
#include "Emerald.h"
#include "HpBarBack.h"
#include "InventoryBtn.h"
#include "InventoryPanel.h"
#include "MapBtn.h"
#include "MouseRightUI.h"
#include "DashUI.h"
#include "QuickSlotPlus.h"
#include "ExitBtn.h"
#include "EmeraldSword.h"
#include "GearSlot.h"
#include "GearSlotPlus.h"
#include "HP_Potion.h"
#include "GearStrength.h"
#include "GearStrengthBack.h"
#include "LevelFront.h"
#include "SwordFilter.h"
#include "ArrowFilter.h"
#include "Filter.h"
#include "ArmorFilter.h"
#include "PotionFilter.h"
#include "EnchantFilter.h"
#include "CostumeFilter.h"
#include "InventorySlot.h"
#include "Object.h"
#include "ItemSlot.h"
#include "Scene.h"
#include "InventoryEmerald.h"
#include "InventoryEnchant.h"
#include "ScrollBack.h"
#include "SwordItem.h"
#include "BowItem.h"
#include "WolfArmor.h"
#include "RocketItem.h"
#include "TooltipObj.h"
#include "FishingItem.h"
#include "QuestPanel.h"
#include "QuestTextObj.h"
#include "WorldMapPanel.h"
#include "Locked_Node.h"
#include "MapNode.h"
#include "LockNode_Back.h"
#include "MapNode_Front.h"
#include "LoadingStone.h"
#include "WorldMapTextPanel.h"
#include "WorldTooltip.h"

#include "ArrowSlot.h"
#include "UIManager.h"
#include "InventoryUI.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EngineCore.h"
#include "InventoryManager.h"
#include "InventoryUIBuilder.h"
#include "QuestSystem.h"
#include "ObjectManager.h"
#include "TooltipManager.h"

#define ADD(obj) objMgr->AddUIObject(obj)

void UILoader::LoadUI(ObjectManager* objMgr)
{
    BuildCursorAndInventory(objMgr);
    BuildPlayerBars(objMgr);
    BuildHotbar(objMgr);
    BuildQuickSlots(objMgr);
    BuildFilters(objMgr);
    BuildQuestUI(objMgr);
    BuildMiscUI(objMgr);
    BuildWorldMapUI(objMgr);
}

void UILoader::BuildCursorAndInventory(ObjectManager* objMgr)
{
    auto* scene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto* uiMgr = scene->GetUIManager();
    auto* invMgr = uiMgr->GetInventory();

    ADD(Cursor::Create(objMgr));
    InventoryUIBuilder::BuildInventoryUI(objMgr, invMgr);

    auto tooltip = TooltipObj::Create(objMgr);
    tooltip->SetInventoryManager(invMgr);
    ADD(tooltip);

    vector<Object*> items =
    {
        SwordItem::Create(objMgr), BowItem::Create(objMgr),
        WolfArmor::Create(objMgr), RocketItem::Create(objMgr),
        FishingItem::Create(objMgr)
    };

    for (auto* it : items) 
    {
        ADD(it);
        invMgr->InsertItem(it); 
    }

    ADD(InventoryUI::Create(objMgr));
    ADD(InventoryPanel::Create(objMgr));
    ADD(InventoryBtn::Create(objMgr));
}

void UILoader::BuildPlayerBars(ObjectManager* objMgr)
{
    auto playerInfo = objMgr->GetFrontObject(ObjectType::Player)
        ->GetComponent<InfoComponent<PlayerInfo>>();

    auto hpFront = HPBarFront::Create(objMgr);
    playerInfo->Attach(hpFront->GetComponent<ProgressBar<PlayerInfo>>());
    hpFront->GetComponent<ProgressBar<PlayerInfo>>()->SetEventType(UIEventType::HP_Changed);
    ADD(hpFront);

    auto expFront = ExpBarFront::Create(objMgr);
    playerInfo->Attach(expFront->GetComponent<ProgressBar<PlayerInfo>>());
    expFront->GetComponent<ProgressBar<PlayerInfo>>()->SetEventType(UIEventType::EXP_Changed);
    ADD(expFront); 
   
    for (float x : {350.f, 670.f})
    {
        auto back = ExpBarBack::Create(objMgr);
        back->GetComponent<TransformComponent>()->SetPosition(x, 717.f);
        ADD(back);
    }
}

void UILoader::BuildHotbar(ObjectManager* objMgr)
{
    ADD(HotBarBack::Create(objMgr));
    ADD(ScrollBack::Create(objMgr));

    auto hpPotion = HP_Potion::Create(objMgr);
    hpPotion->GetComponent<TransformComponent>()->SetPosition(700.f, 650.f);
    ADD(hpPotion);

    ADD(DashUI::Create(objMgr));
    ADD(MouseRightUI::Create(objMgr));
}

void UILoader::BuildQuickSlots(ObjectManager* objMgr)
{
    const vector<_vec2> pos = {{300.f, 655.f}, {770.f, 655.f}};

    for (auto p : pos) 
    {
        auto slot = QuickSlot::Create(objMgr);
        auto transform = slot->GetComponent<TransformComponent>();
        transform->SetPosition(p.x, p.y);
        transform->SetScale(0.2f, 0.2f);
        slot->GetComponent<UIRenderer>()->SetScale(1.6f, 1.6f);
        ADD(slot);
    }

    auto arrowSlot = ArrowSlot::Create(objMgr);
    arrowSlot->GetComponent<TransformComponent>()->SetPosition(940.f, 660.f);
    ADD(arrowSlot);
}

void UILoader::BuildFilters(ObjectManager* objMgr)
{
    struct Pos { float x, y; };
    vector<Pos> base = {{120, 170}, {300, 120}, {480, 170}, {150, 620}, {300, 620}, {450, 620}};
    vector<Object*> filters = 
    {
        SwordFilter::Create(objMgr), ArmorFilter::Create(objMgr),
        ArrowFilter::Create(objMgr), PotionFilter::Create(objMgr),
        PotionFilter::Create(objMgr), PotionFilter::Create(objMgr),
    };
    for (size_t i = 0; i < filters.size(); ++i) 
    {
        auto transform = filters[i]->GetComponent<TransformComponent>();
        transform->SetPosition(base[i].x + 35.f, base[i].y + 35.f);
        filters[i]->GetComponent<UIRenderer>()->SetScale(0.8f, 0.8f);
        ADD(filters[i]);
    }

    initializer_list<Object*> iconFilters =
    {
        Filter::Create(objMgr),
        SwordFilter::Create(objMgr),
        ArrowFilter::Create(objMgr),
        ArmorFilter::Create(objMgr),
        PotionFilter::Create(objMgr),
        EnchantFilter::Create(objMgr),
        CostumeFilter::Create(objMgr)
    };

    for (auto* f : iconFilters) ADD(f);
}

void UILoader::BuildQuestUI(ObjectManager* objMgr)
{
    auto panel = QuestPanel::Create(objMgr);
    auto text = QuestTextObj::Create(objMgr);
    ADD(panel); ADD(text);

    auto questSys = objMgr->GetOwner()->GetUIManager()->GetQuestSystem();
    questSys->SetPanel(panel);
    questSys->SetTextObj(text);
}

void UILoader::BuildMiscUI(ObjectManager* objMgr)
{
    ADD(Emerald::Create(objMgr));

    auto exitGame = ExitBtn::Create(objMgr);
    auto exitMap = ExitBtn::Create(objMgr);
    exitMap->GetComponent<UIRenderer>()->SetRenderType(UIRenderType::WorldMap);
    ADD(exitGame); ADD(exitMap);

    ADD(MapBtn::Create(objMgr));
    ADD(GearStrengthBack::Create(objMgr));
    ADD(GearStrength::Create(objMgr));
    ADD(LevelFront::Create(objMgr));
    ADD(UIDebugObj::Create(objMgr));
}

void UILoader::BuildWorldMapUI(ObjectManager* objMgr)
{
    auto* scene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto* uiMgr = scene->GetUIManager();
    auto* tooltip = uiMgr->GetTooltip();

    auto panel = WorldMapPanel::Create(objMgr);
    ADD(panel);
    auto rootTransform = panel->GetComponent<TransformComponent>();

    auto AddLocked = [&](float x, float y)
        {
            auto node = Locked_Node::Create(objMgr);
            auto transform = node->GetComponent<TransformComponent>();
            transform->SetPosition(x, y);
            transform->SetParent(rootTransform);
            ADD(node);

            auto back = LockNode_Back::Create(objMgr);
            back->GetComponent<TransformComponent>()->SetParent(transform);
            ADD(back);
        };

    for (auto p : {_vec2{-100, 200}, {-150, 0}, {-120, -200}, {-450, 240}})
        AddLocked(p.x, p.y);

    auto AddMap = [&](float x, float y)
        {
            auto node = MapNode_Front::Create(objMgr);
            auto transform = node->GetComponent<TransformComponent>();
            transform->SetPosition(x, y);
            transform->SetParent(rootTransform);
            ADD(node);
        };

    for (auto p : {_vec2{-600, 80}, {-480, -150}})
        AddMap(p.x, p.y);

    ADD(LoadingStone::Create(objMgr));

    auto AddText = [&](float x, float y, const wchar_t* txt)
        {
            auto panel = WorldMapTextPanel::Create(objMgr);
            panel->GetComponent<TransformComponent>()->SetPosition(x, y);
            panel->SetText(txt);
            ADD(panel);
        };

    AddText(400, 100, L"ÁÖ ´ë·ú");
    AddText(610, 100, L"¼¶ ¿µÁö");
    AddText(820, 100, L"´Ù¸¥ Â÷¿ø");

    auto worldTooltip = WorldTooltip::Create(objMgr);
    worldTooltip->GetComponent<UIRenderer>()->SetVisible(false);
    ADD(worldTooltip);

    tooltip->SetWorldTooltip(worldTooltip);
}
