#include "pch.h"
#include "UILoader.h"

// Components
#include "TransformComponent.h"
#include "InfoComponent.h"
#include "ProgressBar.h"

// Objects
#include "HPBarFront.h"
#include "Player.h"
#include "UIDebugObj.h"
#include "HotBarBack.h"
#include "ExpBarFront.h"
#include "Cursor.h"
#include "Slot.h"
#include "Emerald.h"
#include "HPBarBack.h"
#include "ArrowSlot.h"

void UILoader::LoadUI(ObjectManager* objMgr, Player* player)
{
	const auto& playerInfo = player->GetComponent<InfoComponent<PlayerInfo>>();

	auto hpBarFront = HPBarFront::Create(objMgr);
	objMgr->AddUIObject(hpBarFront);

	auto hpInfo = hpBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(hpInfo);
	hpInfo->SetUIEventType(UIEventType::HP_Changed);

	auto expBarFront = ExpBarFront::Create(objMgr);
	objMgr->AddUIObject(expBarFront);

	auto expInfo = expBarFront->GetComponent<ProgressBar<PlayerInfo>>();
	playerInfo->Attach(expInfo);
	expInfo->SetUIEventType(UIEventType::EXP_Changed);

	auto debugUI = UIDebugObj::Create(objMgr);
	objMgr->AddUIObject(debugUI);
	debugUI->SetPlayer(player);

	objMgr->AddUIObject(HotBarBack::Create(objMgr));
	//objMgr->AddUIObject(HPBarBack::Create(objMgr));

	objMgr->AddUIObject(Cursor::Create(objMgr));

	auto slot1 = Slot::Create(objMgr);
	slot1->GetComponent<TransformComponent>()->SetPosition(380.f, 650.f);
	objMgr->AddUIObject(slot1);

	auto slot2 = Slot::Create(objMgr);
	slot2->GetComponent<TransformComponent>()->SetPosition(450.f, 650.f);
	objMgr->AddUIObject(slot2);

	auto slot3 = Slot::Create(objMgr);
	slot3->GetComponent<TransformComponent>()->SetPosition(520.f, 650.f);
	objMgr->AddUIObject(slot3);

	auto slot4 = Slot::Create(objMgr);
	slot4->GetComponent<TransformComponent>()->SetPosition(705.f, 650.f);
	objMgr->AddUIObject(slot4);

	auto slot5 = Slot::Create(objMgr);
	slot5->GetComponent<TransformComponent>()->SetPosition(770.f, 655.f);
	slot5->GetComponent<UIRenderer>()->SetScale(0.2f,0.2f);
	objMgr->AddUIObject(slot5);

	auto arrowSlot = ArrowSlot::Create(objMgr);
	objMgr->AddUIObject(arrowSlot);
}
