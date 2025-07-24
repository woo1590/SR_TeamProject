#include "pch.h"
#include "UILoader.h"
#include "ChangeScene.h"
#include "GameManager.h"

/* --- Engine forward-only Header ------------------------------------------- */
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"
#include "UIManager.h"
#include "TooltipManager.h"
#include "ObjectManager.h"
#include "QuestSystem.h"
#include "InventoryManager.h"
#include "ProgressBar.h"
#include "InfoComponent.h"
#include "ItemComponent.h"
#include "DialogComponent.h"
#include "DialogManager.h"
#include "PanelComponent.h"
#include "RenderSystem.h"
#include "CameraComponent.h"
#include "HoverButtonComponent.h"
#include "MeshRendererComponent.h"
#include "InventoryComponent.h"

/* --- UI Object ------------------------------*/
#include "Cursor.h"
#include "InventoryUIBuilder.h"
#include "InventoryUI.h"
#include "InventoryPanel.h"
#include "InventoryBtn.h"
#include "HPBarFront.h"
#include "ExpBarFront.h"
#include "ExpBarBack.h"
#include "HotBarBack.h"
#include "ScrollBack.h"
#include "QuickSlot.h"
#include "ArrowSlot.h"
#include "HP_Potion.h"
#include "DashUI.h"
#include "MouseRightUI.h"
#include "SwordFilter.h"
#include "ArrowFilter.h"
#include "ArmorFilter.h"
#include "PotionFilter.h"
#include "EnchantFilter.h"
#include "CostumeFilter.h"
#include "Filter.h"
#include "FilterTooltip.h"
#include "QuestPanel.h"
#include "QuestTextObj.h"
#include "Emerald.h"
#include "ExitBtn.h"
#include "MapBtn.h"
#include "GearStrengthBack.h"
#include "GearStrength.h"
#include "LevelFront.h"
#include "UIDebugObj.h"
#include "WorldMapPanel.h"
#include "WorldTooltip.h"
#include "Locked_Node.h"
#include "LockNode_Back.h"
#include "MapNode_Front.h"
#include "WorldMapTextPanel.h"
#include "PigItem.h"
#include "RocketItem.h"
#include "WolfArmor.h"
#include "FishingItem.h"
#include "TooltipObj.h"
#include "SwordItem.h"
#include "BowItem.h"
#include "HPBarBack.h"
#include "ParticleObj.h"
#include "HPBarWhite.h"
#include "SpearItem.h"
#include "CrossBowItem.h"
#include "BoostItem.h"
#include "GhostCloakItem.h"
#include "Arrows.h"
#include "MiniMapCam.h"
#include "MiniMap.h"
#include "LoadingScene.h"
#include "InventoryPlayer.h"
#include "InventoryCam.h"
#include "BaseCharacter.h"
#include "Player.h"
#include "Stage1Icon.h"
#include "Stage2Icon.h"
#include "Stage0Icon.h"
#include "LeftBtn.h"
#include "RightBtn.h"
#include "ShopBackGround.h"
#include "ShopManager.h"
#include "ShopSlot.h"
#include "ShopTooltip.h"
#include "ShopBtn.h"
#include "QuestSystem.h"

// DeathUI
#include "PlayerDeathUI.h"
#include "DeathFrame.h"
#include "BossIcon.h"

// QuestUI
#include "QuestPanel.h"
#include "atri.h" 
#include "Angry.h"

#define ADD(obj) objMgr->AddUIObject(obj)

void UILoader::LoadUI(ObjectManager* objMgr)
{
    auto* scene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto* uiMgr = scene->GetUIManager();
    auto* invMgr = uiMgr->GetInventory();
    auto* tooltipMgr = uiMgr->GetTooltip();
    auto* dialogMgr = uiMgr->GetDialog();
    auto* shopMgr = uiMgr->GetShop();
    auto* quest = uiMgr->GetQuestSystem();

    quest->LoadDataFrom();

    ADD(Cursor::Create(objMgr));

    BuildInventory(objMgr,invMgr);
    BuildPlayerBars(objMgr);
    BuildHotbar(objMgr);
    BuildQuickSlots(objMgr);
    BuildFilters(objMgr, invMgr, tooltipMgr);
    BuildQuestUI(objMgr);
    BuildMiscUI(objMgr);
    BuildWorldMapUI(objMgr);
    BuildDeathUI(objMgr);
    BuildDialogUI(objMgr,dialogMgr); 
    BuildMiniMap(objMgr);
    BuildShopUI(objMgr, invMgr, tooltipMgr,shopMgr);

  //  ADD(ParticleObj::Create(objMgr));
}

void UILoader::BuildInventory(ObjectManager* objMgr, InventoryManager* invMgr)
{
    auto* playerObj = objMgr->GetFrontObject(ObjectType::Player);
    invMgr->SetPlayer(playerObj);

    InventoryUIBuilder::BuildInventoryUI(objMgr, invMgr);
    auto tooltipObj = TooltipObj::Create(objMgr);
    tooltipObj->SetInventoryManager(invMgr);
    ADD(tooltipObj);

    auto player = dynamic_cast<Player*>(playerObj);
    auto invComp = player->GetComponent<InventoryComponent>();

    auto equipFunc = [player, invComp](ItemType type) {player->EquipItem(type); invComp->Equip(type); };
    auto unequipFunc = [player, invComp](ItemType type){player->UnEquipItem(type); invComp->UnEquip(type); };
    auto createFunc = [objMgr,equipFunc,unequipFunc](ItemType type)->Object*
        {
            Object* itemObj = CreateInventoryObj(objMgr, type);
            if (itemObj)
            {
                ADD(itemObj);
                auto itemComp = itemObj->GetComponent<ItemComponent>();
                if (itemComp)
                {
                    itemComp->SetEquipCallBack([equipFunc, type](Object*) {equipFunc(type); });
                    itemComp->SetUnEquipCallBack([unequipFunc, type](Object*) {unequipFunc(type); });
                }
            }
            return itemObj;
        };

    invMgr->BindInventory(equipFunc, unequipFunc, createFunc);

    for (const auto& itemType : invComp->GetUnequippedItems())
    {
        Object* itemObj = createFunc(itemType);
        if (itemObj) invMgr->InsertItem(itemObj);
    }
    for (const auto& itemType : invComp->GetEquippedItems())
    {
        Object* itemObj = createFunc(itemType);
        if (itemObj)
        {
            SlotItemType category = invMgr->GetSlotCategory(itemType);
            SlotComponent* equipSlot = invMgr->FindSlotByType(category);
            if (equipSlot)
            {
                equipSlot->SetItem(itemObj);
                player->EquipItem(itemType);
            }
        }
    }

    ADD(InventoryUI::Create(objMgr));
    ADD(InventoryPanel::Create(objMgr));
    ADD(InventoryBtn::Create(objMgr));

    ADD(LeftBtn::Create(objMgr));
    ADD(RightBtn::Create(objMgr));
}


void UILoader::BuildPlayerBars(ObjectManager* objMgr)
{
    auto playerInfo = objMgr->GetFrontObject(ObjectType::Player)
        ->GetComponent<InfoComponent<PlayerInfo>>();

    auto hpFront = HPBarFront::Create(objMgr);
    playerInfo->Attach(hpFront->GetComponent<ProgressBar<PlayerInfo>>());
    hpFront->GetComponent<ProgressBar<PlayerInfo>>()->SetEventType(UIEventType::HP_Changed);
    ADD(hpFront);

    auto hpback = HPBarBack::Create(objMgr);
    ADD(hpback);

    auto arrows = Arrows::Create(objMgr);
    dynamic_cast<Player*>(objMgr->GetFrontObject(ObjectType::Player))->SetArrows(arrows);
    ADD(arrows);

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

void UILoader::BuildFilters(ObjectManager* objMgr,InventoryManager* invMgr,TooltipManager* tooltipMgr)
{
    struct FInfo { Object* obj; optional<SlotItemType> category; };
    vector<FInfo> btns = {
        {Filter::Create(objMgr), nullopt}, 
        {SwordFilter::Create(objMgr),   SlotItemType::MeleeWeapon},
        {ArrowFilter::Create(objMgr),   SlotItemType::RangeWeapon},
        {ArmorFilter::Create(objMgr),   SlotItemType::Armor},
        {PotionFilter::Create(objMgr),  SlotItemType::Potion},
        {EnchantFilter::Create(objMgr), SlotItemType::Enchant}, 
        {CostumeFilter::Create(objMgr), SlotItemType::Costume}, 
    };

    for (auto& info : btns)
    {
        Object* btnObj = info.obj;
        optional<SlotItemType> category = info.category;

        if (auto* btn = btnObj->GetComponent<ButtonComponent>())
            btn->SetOnClick([invMgr, category]() {invMgr->ApplyFilter(category);});

        auto* hover = btnObj->AddComponent<HoverComponent>();
        hover->SetCallBack([btnObj, category](bool over) {
            auto tooltipMgr = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetUIManager()->GetTooltip();
            if (!tooltipMgr) return;

            if (over)
            {
                wstring name;
                if (!category.has_value())       name = L"전체";
                else switch (*category)
                {
                case SlotItemType::MeleeWeapon:  name = L"근접";   break;
                case SlotItemType::RangeWeapon:  name = L"원거리"; break;
                case SlotItemType::Armor:        name = L"방어구"; break;
                case SlotItemType::Potion:       name = L"유물";   break;
                case SlotItemType::Enchant:      name = L"효과부여됨"; break;
                case SlotItemType::Costume:      name = L"코스메틱"; break;
                default:                         name = L"기타";   break;
                }

                auto pos = btnObj->GetComponent<TransformComponent>()->GetWorldPosition();
                tooltipMgr->ShowInventoryTooltip(name, pos.x, pos.y);
            }
            else
                tooltipMgr->HideInventoryTooltip();
            });

        ADD(btnObj);
    }

    auto* filterTooltip = FilterTooltip::Create(objMgr);
    ADD(filterTooltip);
    tooltipMgr->SetInventoryTooltip(filterTooltip);
}


void UILoader::BuildQuestUI(ObjectManager* objMgr)
{
    auto panel = QuestPanel::Create(objMgr);
    auto text = QuestTextObj::Create(objMgr);
    ADD(panel); ADD(text);

    auto questSys = objMgr->GetOwner()->GetUIManager()->GetQuestSystem();
    //questSys->SetPanel(panel);
    questSys->SetTextObj(text);
}

void UILoader::BuildMiscUI(ObjectManager* objMgr)
{
    ADD(Emerald::Create(objMgr));

    auto exitGame = ExitBtn::Create(objMgr,ExitBtnType::Inventory);
    auto exitMap = ExitBtn::Create(objMgr,ExitBtnType::WorldMap);
    auto exitShop = ExitBtn::Create(objMgr, ExitBtnType::Shop);
    exitShop->GetComponent<UIRenderer>()->SetRenderType(UIRenderType::Shop);
    exitMap->GetComponent<UIRenderer>()->SetRenderType(UIRenderType::WorldMap);
    ADD(exitGame); ADD(exitMap); ADD(exitShop);

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
    auto rootTf = panel->GetComponent<TransformComponent>();

    auto AddLocked = [&](float x, float y)
        {
            auto node = Locked_Node::Create(objMgr);
            auto transform = node->GetComponent<TransformComponent>();
            transform->SetPosition(x, y);
            transform->SetParent(rootTf);
            ADD(node);

            auto back = LockNode_Back::Create(objMgr);
            back->GetComponent<TransformComponent>()->SetParent(transform);
            ADD(back);
        };

    for (auto p : {_vec2{-100, 200}, {-150, 0}, {-120, -200}, {-450, 240}})
        AddLocked(p.x, p.y);

    auto AddMap = [&]<typename IconType>(float x, float y, LOADID loadID, StageSelect stage)
        {
            auto node = MapNode_Front::Create(objMgr);
            node->SetLoadID(loadID);
            node->SetStage(stage);

            auto tf = node->GetComponent<TransformComponent>();
            tf->SetPosition(x, y);
            tf->SetParent(rootTf);

            auto button = node->GetComponent<HoverButtonComponent>();
            button->SetOnClick([loadID]() 
                {
                    auto command = ChangeScene::Create(loadID);
                    EngineCore::GetInstance()->RegisterCommand(command);
                    GameManager::GetInstance()->ClearScene(loadID);
                });
            ADD(node); 

            auto icon = IconType::Create(objMgr);
            auto iconTf = icon->GetComponent<TransformComponent>();
            iconTf->SetParent(tf);
            iconTf->SetPosition(0.f, 0.f);
            ADD(icon);
        };

    AddMap.template operator()<Stage1Icon>(-600.f, 80.f, LOADID::Stage1,StageSelect::Stage1);
    AddMap.template operator()<Stage2Icon>(-480.f, -150.f, LOADID::Stage2,StageSelect::Stage2);
    AddMap.template operator()<Stage0Icon>(-750.f, 150.f, LOADID::Village, StageSelect::Stage0);

    auto AddText = [&](float x, float y, const wchar_t* txt)
        {
            auto panel = WorldMapTextPanel::Create(objMgr);
            panel->GetComponent<TransformComponent>()->SetPosition(x, y);
            panel->SetText(txt);
            ADD(panel);
        };

    AddText(400, 100, L"주 대륙");
    AddText(610, 100, L"섬 영지");
    AddText(820, 100, L"다른 차원");

    auto worldTooltip = WorldTooltip::Create(objMgr);
    worldTooltip->GetComponent<UIRenderer>()->SetVisible(false);
    ADD(worldTooltip);

    tooltip->SetWorldTooltip(worldTooltip);
}

void UILoader::BuildDeathUI(ObjectManager* objMgr)
{
    auto playerDeathUI = PlayerDeathUI::Create(objMgr);
    ADD(playerDeathUI);

    auto deathFrame = DeathFrame::Create(objMgr);
    ADD(deathFrame);

    auto bossIcon = BossIcon::Create(objMgr);
    bossIcon->GetComponent<UIRenderer>()->SetRenderType(UIRenderType::DeathUI);
    bossIcon->GetComponent<TransformComponent>()->SetPosition(645.f, 260.f);
    bossIcon->GetComponent<TransformComponent>()->SetScale(0.7f, 1.3f);
    ADD(bossIcon);
}

void UILoader::BuildDialogUI(ObjectManager* objMgr, DialogManager* dialogMgr)
{
    auto panel = QuestPanel::Create(objMgr);
    ADD(panel);

    auto atri = Atri::Create(objMgr);
    ADD(atri);

    //auto angry = Angry::Create(objMgr);
    //ADD(angry);

    panel->AddChild(atri);

    dialogMgr->SetPanel(panel->GetComponent<PanelComponent>());

    dialogMgr->SetEmotionChangeCallBack([atri](Emotion emotion) {if (atri) atri->SetEmotion(emotion);});
}

void UILoader::BuildMiniMap(ObjectManager* objMgr)
{
    auto renderSystem = EngineCore::GetInstance()->GetRenderSystem();
    ADD(InventoryPlayer::Create(objMgr));
}

void UILoader::BuildShopUI(ObjectManager* objMgr, InventoryManager* invMgr, TooltipManager* tooltipMgr,ShopManager* shopMgr)
{
    ADD(ShopBackGround::Create(objMgr));
    ADD(ShopBtn::Create(objMgr));
    shopMgr->BindCreateCallBack([objMgr](ItemType type)->Object*{return CreateInventoryObj(objMgr, type);});

    const int cols = 3;
    const int rows = 2;
    const _vec2 topLeft = {530.f, 150.f};
    const float slotSpacingX = 180.f;
    const float slotSpacingY = 200.f;

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            const float px = topLeft.x + x * slotSpacingX;
            const float py = topLeft.y + y * slotSpacingY;

            auto slot = ShopSlot::Create(objMgr);
            slot->GetComponent<TransformComponent>()->SetPosition(px, py);
            ADD(slot);

            shopMgr->RegisterShopSlot(slot);
        }
    }
    shopMgr->StockItem(ItemType::Sword, 0);
    shopMgr->StockItem(ItemType::Armor, 1);
    shopMgr->StockItem(ItemType::Bow, 2);
    shopMgr->StockItem(ItemType::FishingItem, 3);
    shopMgr->StockItem(ItemType::Spear, 4);
    shopMgr->StockItem(ItemType::CrossBow, 5);

    auto shopTooltip = ShopTooltip::Create(objMgr);
    shopTooltip->SetShopManager(shopMgr);
    ADD(shopTooltip);
    tooltipMgr->SetShopTooltip(shopTooltip);

}

Object* UILoader::CreateInventoryObj(ObjectManager* objMgr, ItemType type)
{
    switch (type)
    {
    case ItemType::Sword:       return SwordItem::Create(objMgr);
    case ItemType::Spear:       return SpearItem::Create(objMgr);
    case ItemType::Armor:       return WolfArmor::Create(objMgr);
    case ItemType::Bow:         return BowItem::Create(objMgr);
    case ItemType::CrossBow:    return CrossBowItem::Create(objMgr);
    case ItemType::FishingItem: return FishingItem::Create(objMgr);
    case ItemType::BoostItem:   return BoostItem::Create(objMgr);
    case ItemType::PigItem:     return PigItem::Create(objMgr);
    case ItemType::GhostCloak:  return GhostCloakItem::Create(objMgr);
    case ItemType::RocketItem:  return RocketItem::Create(objMgr);
    default:                    return nullptr;
    }
}
