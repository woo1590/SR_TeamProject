#include "pch.h"
#include "UILoader.h"

// Components
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "ProgressBar.h"
#include "SlotComponent.h"
#include "PanelComponent.h"
#include "ButtonComponent.h"

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

#include "ArrowSlot.h"

void UILoader::LoadUI(ObjectManager* objMgr, Player* player)
{
	const auto& playerInfo = player->GetComponent<InfoComponent<PlayerInfo>>();

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

	objMgr->AddUIObject(Cursor::Create(objMgr));

	struct QuickSlotInfo { float x; float y; };

	vector<QuickSlotInfo> slotPos = {
		{380.f,650.f},
		{450.f,650.f},
		{520.f,650.f},
		{700.f,650.f},
	};

	for (const auto& pos : slotPos)
	{
		auto plus = QuickSlotPlus::Create(objMgr);
		plus->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
		objMgr->AddUIObject(plus);

		auto slot = QuickSlot::Create(objMgr);
		slot->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
		objMgr->AddUIObject(slot);
	}

	struct GearSlotInfo { float x; float y; };

	vector<GearSlotInfo> gearSlotPos = {
		{120.f,170.f},
		{300.f,120.f},
		{480.f,170.f},
		{150.f,620.f},
		{300.f,620.f},
		{450.f,620.f},
	};

	for (const auto& pos : gearSlotPos)
	{
		auto plus = GearSlotPlus::Create(objMgr);
		plus->GetComponent<TransformComponent>()->SetPosition(pos.x + 5.f, pos.y + 5.f);
		objMgr->AddUIObject(plus);
	
		auto slot = GearSlot::Create(objMgr);
		slot->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
		objMgr->AddUIObject(slot);
	}

	auto smallSlot1 = QuickSlot::Create(objMgr);
	smallSlot1->GetComponent<TransformComponent>()->SetPosition(300.f, 655.f);
	smallSlot1->GetComponent<SlotComponent>()->SetSlotSize(SlotSize::Small);
	objMgr->AddUIObject(smallSlot1);

	auto smallSlot2 = QuickSlot::Create(objMgr);
	smallSlot2->GetComponent<TransformComponent>()->SetPosition(770.f, 655.f);
	smallSlot2->GetComponent<SlotComponent>()->SetSlotSize(SlotSize::Small);
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

	//objMgr->AddUIObject(EmeraldSword::Create(objMgr));

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

	const _vec2 topLeft = {630.f, 200.f};
	const float slotSpacingX = 80.f;
	const float slotSpacingY = 100.f;
	const int rows = 5;
	const int cols = 4;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			const float px = topLeft.x + x * slotSpacingX;
			const float py = topLeft.y + y * slotSpacingY;

			auto slot = InventorySlot::Create(objMgr);
			slot->GetComponent<TransformComponent>()->SetPosition(px, py);
			objMgr->AddUIObject(slot);
		}
	}
}