#include "pch.h"
#include "UILoader.h"

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

  //  ADD(ParticleObj::Create(objMgr));
}

void UILoader::BuildInventory(ObjectManager* objMgr,InventoryManager* invMgr)
{
    InventoryUIBuilder::BuildInventoryUI(objMgr, invMgr);

    auto tooltip = TooltipObj::Create(objMgr);
    tooltip->SetInventoryManager(invMgr);
    ADD(tooltip);

    vector<Object*> items =
    {
        SwordItem::Create(objMgr), BowItem::Create(objMgr),
        WolfArmor::Create(objMgr), RocketItem::Create(objMgr),
        FishingItem::Create(objMgr), PigItem::Create(objMgr),
        SpearItem::Create(objMgr), CrossBowItem::Create(objMgr),
        BoostItem::Create(objMgr), GhostCloakItem::Create(objMgr),

    };

    auto* player = dynamic_cast<Player*>(objMgr->GetFrontObject(ObjectType::Player));
    assert(player && "Player must exist in scene");

    for (auto* it : items) 
    {
        auto* itemComp = it->GetComponent<ItemComponent>();
        if (itemComp)
        {
            ItemType itemType = itemComp->GetItemType();

            itemComp->SetEquipCallBack([player, itemType](Object* user) { player->EquipItem(itemType);});
            itemComp->SetUnEquipCallBack([player, itemType](Object* user) { player->UnEquipItem(itemType);});
        }
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

    auto hpback = HPBarBack::Create(objMgr);
    ADD(hpback);

    auto arrows = Arrows::Create(objMgr);
    ADD(arrows);

    //auto hpWhite = HPBarWhite::Create(objMgr);
    //auto whiteBar = hpWhite->AddComponent<ProgressBar<PlayerInfo>>();
    //playerInfo->Attach(whiteBar);
    //ADD(hpWhite);
    //
    //auto frontTf = hpFront->GetComponent<TransformComponent>();
    //hpback->GetComponent<TransformComponent>()->SetParent(frontTf);
    //hpWhite->GetComponent<TransformComponent>()->SetParent(frontTf);

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

void UILoader::BuildFilters(ObjectManager* objMgr, InventoryManager* invMgr, TooltipManager* tooltipMgr)
{
    struct Pos { float x, y; };
    array<Pos, 6> leftPos = {{{120, 170}, {300, 120}, {480, 170}, 
                              {150, 620}, {300, 620}, {450, 620}}};

    array<Object*, 6> leftFilters =
    {
        SwordFilter::Create(objMgr),  ArmorFilter::Create(objMgr),
        ArrowFilter::Create(objMgr),  PotionFilter::Create(objMgr),
        PotionFilter::Create(objMgr), PotionFilter::Create(objMgr)
    };

    for (size_t i = 0; i < leftFilters.size(); ++i)
    {
        auto* obj = leftFilters[i];
        auto transform = obj->GetComponent<TransformComponent>();
        transform->SetPosition(leftPos[i].x + 35.f, leftPos[i].y + 35.f);
        obj->GetComponent<UIRenderer>()->SetScale(0.8f, 0.8f);
        ADD(obj);
    }

    struct FInfo { Object* obj; optional<ItemType> type; };

    vector<FInfo> btns = 
    {
        {Filter::Create(objMgr), nullopt},
        {SwordFilter::Create(objMgr),   ItemType::Sword},
        {ArrowFilter::Create(objMgr),   ItemType::Bow},
        {ArmorFilter::Create(objMgr),   ItemType::Armor},
        {PotionFilter::Create(objMgr),  ItemType::Potion},
        {EnchantFilter::Create(objMgr), ItemType::Enchant},
        {CostumeFilter::Create(objMgr), ItemType::Costume},
    };

    for (auto& [obj, typeOpt] : btns)
    {
        if (auto* btn = obj->GetComponent<ButtonComponent>())
            btn->SetOnClick([invMgr, typeOpt] { invMgr->ApplyFilter(typeOpt); });

        auto hover = obj->AddComponent<HoverComponent>();
        hover->SetCallBack([obj, typeOpt](bool over) {
            auto tooltipMgr = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetUIManager()->GetTooltip();
            if (!tooltipMgr) return;

            auto pos = obj->GetComponent<TransformComponent>()->GetWorldPosition();

            if (over)
            {
                wstring name = L"";

                if (!typeOpt.has_value()) 
                    name = L"전체";
                else
                {
                    switch (typeOpt.value())
                    {
                    case ItemType::Sword:   name = L"근접"; break;
                    case ItemType::Bow:     name = L"원거리"; break;
                    case ItemType::Armor:   name = L"방어구"; break;
                    case ItemType::Potion:  name = L"유물"; break;
                    case ItemType::Enchant: name = L"효과 부여됨"; break;
                    case ItemType::Costume: name = L"코스메틱"; break;
                    default:                name = L"알 수 없음"; break;
                    }
                }
                tooltipMgr->ShowInventoryTooltip(name, pos.x, pos.y);
            }
            else
                tooltipMgr->HideInventoryTooltip();
            });

        ADD(obj);
    }

    auto filterTooltip = FilterTooltip::Create(objMgr);
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

    auto AddMap = [&](float x, float y, LOADID loadID)
        {
            auto node = MapNode_Front::Create(objMgr);
            auto transform = node->GetComponent<TransformComponent>();
            transform->SetPosition(x, y);
            transform->SetParent(rootTransform);

            auto button = node->GetComponent<HoverButtonComponent>();
            button->SetOnClick([=]() 
                {
                    auto newScene = LoadingScene::Create(loadID);
                    EngineCore::GetInstance()->GetSceneManager()->SetActiveScene(newScene);
                });
            ADD(node);
        };

    AddMap(-600.f, 80.f, LOADID::Stage1);
    AddMap(-480.f, -150.f, LOADID::Stage1);

   // ADD(LoadingStone::Create(objMgr));

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

    auto angry = Angry::Create(objMgr);
    ADD(angry);

    panel->AddChild(atri);

    dialogMgr->SetPanel(panel->GetComponent<PanelComponent>());

    dialogMgr->SetEmotionChangeCallBack([atri](Emotion emotion) {
        if (atri)
            atri->SetEmotion(emotion);
        });
}

void UILoader::BuildMiniMap(ObjectManager* objMgr)
{
    auto renderSystem = EngineCore::GetInstance()->GetRenderSystem();
    
    auto minimapCam = MiniMapCam::Create(objMgr);
    ADD(minimapCam);
    renderSystem->SetMinimapCamera(minimapCam->GetComponent<CameraComponent>());
    
    ADD(MiniMap::Create(objMgr));
    
    auto inventoryCam = InventoryCam::Create(objMgr);
    ADD(inventoryCam);
    renderSystem->SetInventoryCamera(inventoryCam->GetComponent<CameraComponent>());
    
    ADD(InventoryPlayer::Create(objMgr));
    
    //renderSystem->ClearSystem();
    
    auto player = objMgr->GetFrontObject(ObjectType::Player);
    auto playerObj = dynamic_cast<Player*>(player);
    if (playerObj)
    {
        auto& bones = playerObj->GetBones();
        for (const auto& pair : bones)
        {
            Object* bone = pair.second;
            if (bone)
            {
                auto renderer = bone->GetComponent<MeshRenderer>();
                if (renderer)
                    renderSystem->RegisterInventoryRenderer(renderer);
            }
        }
    }
}
