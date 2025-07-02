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
#include "Cursor.h"
#include "QuickSlot.h"
#include "Emerald.h"
#include "HpBarBack.h"

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

	auto expInfo = expBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(expInfo);
	expInfo->SetTrackType(UIEventType::EXP_Changed);

	auto debugUI = UIDebugObj::Create(objMgr);
	objMgr->AddUIObject(debugUI);
	debugUI->SetPlayer(player);

	objMgr->AddUIObject(HotBarBack::Create(objMgr));
	//objMgr->AddUIObject(HPBarBack::Create(objMgr));

	objMgr->AddUIObject(Cursor::Create(objMgr));

	auto quickSlot1 = QuickSlot::Create(objMgr);
	quickSlot1->GetComponent<TransformComponent>()->SetPosition(380.f, 650.f);
	objMgr->AddUIObject(quickSlot1);

	auto quickSlot2 = QuickSlot::Create(objMgr);
	quickSlot2->GetComponent<TransformComponent>()->SetPosition(450.f, 650.f);
	objMgr->AddUIObject(quickSlot2);

	auto quickSlot3 = QuickSlot::Create(objMgr);
	quickSlot3->GetComponent<TransformComponent>()->SetPosition(520.f, 650.f);
	objMgr->AddUIObject(quickSlot3);

	auto quickSlot4 = QuickSlot::Create(objMgr);
	quickSlot4->GetComponent<TransformComponent>()->SetPosition(700.f, 650.f);
	objMgr->AddUIObject(quickSlot4);

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
}
