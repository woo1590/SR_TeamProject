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

#include "ArrowSlot.h"
#include "UIManager.h"
#include "InventoryUI.h"
#include "SceneManager.h"
#include "Scene.h"
#include "EngineCore.h"
#include "InventoryManager.h"
#include "InventoryUIBuilder.h"
#include "QuestSystem.h"

void UILoader::LoadUI(ObjectManager* objMgr, Player* player)
{
	const auto& playerInfo = player->GetComponent<InfoComponent<PlayerInfo>>();
	auto* scene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
	auto* uiMgr = scene->GetUIManager();
	auto* invMgr = uiMgr->GetInventory();

	InventoryUIBuilder::BuildInventoryUI(objMgr, invMgr);
	auto tooltip = TooltipObj::Create(objMgr);
	tooltip->SetInventoryManager(invMgr);
	objMgr->AddUIObject(tooltip);

	auto questPanel = QuestPanel::Create(objMgr);
	objMgr->AddUIObject(questPanel);

	auto questTextObj = QuestTextObj::Create(objMgr);
	objMgr->AddUIObject(questTextObj);

	auto questSystem = objMgr->GetOwner()->GetUIManager()->GetQuestSystem();
	questSystem->SetPanel(questPanel);
	questSystem->SetTextObj(questTextObj);

	auto swordItem = SwordItem::Create(objMgr);
	objMgr->AddUIObject(swordItem);
	invMgr->InsertItem(swordItem);

	auto arrowItem = BowItem::Create(objMgr);
	objMgr->AddUIObject(arrowItem);
	invMgr->InsertItem(arrowItem);

	auto armorItem = WolfArmor::Create(objMgr);
	objMgr->AddUIObject(armorItem);
	invMgr->InsertItem(armorItem);

	auto rocketItem = RocketItem::Create(objMgr);
	objMgr->AddUIObject(rocketItem);
	invMgr->InsertItem(rocketItem);

	auto fishingItem = FishingItem::Create(objMgr);
	objMgr->AddUIObject(fishingItem);
	invMgr->InsertItem(fishingItem);

	auto hpBarFront = HPBarFront::Create(objMgr);
	objMgr->AddUIObject(hpBarFront);
	auto hpInfo = hpBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(hpInfo);
	hpInfo->SetEventType(UIEventType::HP_Changed);

	auto expBarFront = ExpBarFront::Create(objMgr);
	objMgr->AddUIObject(expBarFront);

	auto barBack1 = ExpBarBack::Create(objMgr);
	auto barBack2 = ExpBarBack::Create(objMgr);
	
	barBack1->GetComponent<TransformComponent>()->SetPosition(350.f,717.f);
	barBack2->GetComponent<TransformComponent>()->SetPosition(670.f,717.f);

	objMgr->AddUIObject(barBack1);
	objMgr->AddUIObject(barBack2);

	auto expInfo = expBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(expInfo);
	expInfo->SetEventType(UIEventType::EXP_Changed);

	objMgr->AddUIObject(HotBarBack::Create(objMgr));
	//objMgr->AddUIObject(HPBarBack::Create(objMgr));

	objMgr->AddUIObject(ScrollBack::Create(objMgr));

	objMgr->AddUIObject(Cursor::Create(objMgr));

	objMgr->AddUIObject(InventoryEmerald::Create(objMgr));
	objMgr->AddUIObject(InventoryEnchant::Create(objMgr));

	objMgr->AddUIObject(InventoryUI::Create(objMgr));

	struct GearSlotInfo { float x; float y; };

	vector<GearSlotInfo> gearSlotPos = {
		{120.f, 170.f},
		{300.f, 120.f},
		{480.f, 170.f},
		{150.f, 620.f},
		{300.f, 620.f},
		{450.f, 620.f},
	};

	vector<Object*> filters = {
		SwordFilter::Create(objMgr),
		ArmorFilter::Create(objMgr),
		ArrowFilter::Create(objMgr),
		PotionFilter::Create(objMgr),
		PotionFilter::Create(objMgr),
		PotionFilter::Create(objMgr),
	};

	for (size_t i = 0; i < filters.size(); ++i)
	{
		const auto& base = gearSlotPos[i];
		filters[i]->GetComponent<TransformComponent>()->SetPosition(base.x + 35.f, base.y + 35.f);
		filters[i]->GetComponent<UIRenderer>()->SetScale(0.8f, 0.8f);
		objMgr->AddUIObject(filters[i]);
	}

	auto smallSlot1 = QuickSlot::Create(objMgr);
	smallSlot1->GetComponent<TransformComponent>()->SetPosition(300.f, 655.f);
	smallSlot1->GetComponent<TransformComponent>()->SetScale(0.2f,0.2f);
	smallSlot1->GetComponent<UIRenderer>()->SetScale(1.6f, 1.6f);
	objMgr->AddUIObject(smallSlot1);

	auto smallSlot2 = QuickSlot::Create(objMgr);
	smallSlot2->GetComponent<TransformComponent>()->SetPosition(770.f, 655.f);
	smallSlot2->GetComponent<TransformComponent>()->SetScale(0.2f,0.2f);
	smallSlot2->GetComponent<UIRenderer>()->SetScale(1.6f, 1.6f);
	objMgr->AddUIObject(smallSlot2);

	auto arrowSlot = ArrowSlot::Create(objMgr);
	arrowSlot->GetComponent<TransformComponent>()->SetPosition(940.f, 660.f);
	objMgr->AddUIObject(arrowSlot);

	objMgr->AddUIObject(Emerald::Create(objMgr));
	objMgr->AddUIObject(MapBtn::Create(objMgr));
	
	auto inventoryPanel = InventoryPanel::Create(objMgr);
	objMgr->AddUIObject(inventoryPanel);

	auto inventoryBtn = InventoryBtn::Create(objMgr);
	objMgr->AddUIObject(inventoryBtn);

	objMgr->AddUIObject(MouseRightUI::Create(objMgr));
	objMgr->AddUIObject(DashUI::Create(objMgr));

	auto exitBtn = ExitBtn::Create(objMgr);
	objMgr->AddUIObject(exitBtn);

	auto debugUI = UIDebugObj::Create(objMgr);
	objMgr->AddUIObject(debugUI);
	debugUI->SetPlayer(player);

	auto hpPotion = HP_Potion::Create(objMgr);
	objMgr->AddUIObject(hpPotion);
	hpPotion->GetComponent<TransformComponent>()->SetPosition(700.f,650.f);

	objMgr->AddUIObject(GearStrengthBack::Create(objMgr));
	objMgr->AddUIObject(GearStrength::Create(objMgr));

	objMgr->AddUIObject(LevelFront::Create(objMgr));
	objMgr->AddUIObject(Filter::Create(objMgr));
	objMgr->AddUIObject(SwordFilter::Create(objMgr));
	objMgr->AddUIObject(ArrowFilter::Create(objMgr));
	objMgr->AddUIObject(ArmorFilter::Create(objMgr));
	objMgr->AddUIObject(PotionFilter::Create(objMgr));
	objMgr->AddUIObject(EnchantFilter::Create(objMgr));
	objMgr->AddUIObject(CostumeFilter::Create(objMgr));

	
}

void UILoader::Update(float dt)
{

}