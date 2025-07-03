#include "pch.h"
#include "UILoader.h"

// Components
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "ProgressBar.h"
#include "SlotComponent.h"

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

#include "ArrowSlot.h"

void UILoader::LoadUI(ObjectManager* objMgr, Player* player)
{
	const auto& playerInfo = player->GetComponent<InfoComponent<PlayerInfo>>();

	auto hpBarFront = HPBarFront::Create(objMgr);
	objMgr->AddUIObject(hpBarFront);

	auto hpInfo = hpBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(hpInfo);
	hpInfo->SetTrackType(UIEventType::HP_Changed);

	auto expBarFront = ExpBarFront::Create(objMgr);
	objMgr->AddUIObject(expBarFront);

	objMgr->AddUIObject(ExpBarBack::Create(objMgr));

	auto expInfo = expBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(expInfo);
	expInfo->SetTrackType(UIEventType::EXP_Changed);

	objMgr->AddUIObject(HotBarBack::Create(objMgr));
	//objMgr->AddUIObject(HPBarBack::Create(objMgr));

	objMgr->AddUIObject(Cursor::Create(objMgr));

	struct SlotInfo { float x; float y; };

	vector<SlotInfo> slotPos = {
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
	inventoryBtn->SetTargetPanel(inventoryPanel->GetComponent<PanelComponent>());

	objMgr->AddUIObject(MouseRightUI::Create(objMgr));
	objMgr->AddUIObject(DashUI::Create(objMgr));

	auto exitBtn = ExitBtn::Create(objMgr);
	objMgr->AddUIObject(exitBtn);

	exitBtn->SetTargetPanel(inventoryPanel->GetComponent<PanelComponent>());

	auto debugUI = UIDebugObj::Create(objMgr);
	objMgr->AddUIObject(debugUI);
	debugUI->SetPlayer(player);
}