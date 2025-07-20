#include "pch.h"
#include "InventoryUIBuilder.h"
#include "ObjectManager.h"
#include "InventoryManager.h"
#include "GearSlot.h"
#include "ItemSlot.h"
#include "GearSlotPlus.h"
#include "QuickSlot.h"
#include "QuickSlotPlus.h"
#include "TransformComponent.h"
#include "InventorySlot.h"
#include "UIRenderer.h"

void InventoryUIBuilder::BuildInventoryUI(ObjectManager* objMgr, InventoryManager* invMgr)
{
	BuildGearAndItemSlots(objMgr, invMgr);
	BuildQuickSlots(objMgr,invMgr);
	BuildInventorySlots(objMgr, invMgr);
}

void InventoryUIBuilder::BuildGearAndItemSlots(ObjectManager* objMgr, InventoryManager* invMgr)
{
	vector<_vec2> gearSlotPos = {
		{120.f, 170.f}, {300.f, 120.f}, {480.f, 170.f},
		{150.f, 620.f}, {300.f, 620.f}, {450.f, 620.f}
	};

	for (size_t i = 0; i < gearSlotPos.size(); ++i)
	{
		const auto& pos = gearSlotPos[i];

		auto plus = GearSlotPlus::Create(objMgr);
		plus->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
		objMgr->AddUIObject(plus);

		Object* slot = nullptr;
		SlotItemType slotType = SlotItemType::Any;

		if (i < 3)
		{
			slot = GearSlot::Create(objMgr);

			switch (i)
			{
			case 0: slotType = SlotItemType::MeleeWeapon; break;
			case 1: slotType = SlotItemType::Armor; break;
			case 2: slotType = SlotItemType::RangeWeapon; break;
			}
		}
		else
		{
			slot = ItemSlot::Create(objMgr);
			slotType = SlotItemType::Potion;
		}
		slot->GetComponent<TransformComponent>()->SetPosition(pos.x, pos.y);
		objMgr->AddUIObject(slot);
		invMgr->RegisterSlot(slot, slotType,plus);
	}
}

void InventoryUIBuilder::BuildQuickSlots(ObjectManager* objMgr, InventoryManager* invMgr)
{
	vector<_vec2> slotPos = {
		{380.f, 650.f}, {450.f, 650.f}, {520.f, 650.f}, {700.f, 650.f}
	};

	for (int i = 0; i < 3; ++i)
	{
		auto plus = QuickSlotPlus::Create(objMgr);
		plus->GetComponent<TransformComponent>()->SetPosition(slotPos[i].x, slotPos[i].y);
		plus->GetComponent<UIRenderer>()->SetScale(0.2f, 0.2f);
		objMgr->AddUIObject(plus);

		auto slot = QuickSlot::Create(objMgr);
		slot->GetComponent<TransformComponent>()->SetPosition(slotPos[i].x, slotPos[i].y);
		objMgr->AddUIObject(slot);

		invMgr->RegisterSlot(slot, SlotItemType::Potion, plus);
	}

	auto hpPotionSlot = QuickSlot::Create(objMgr);
	hpPotionSlot->GetComponent<TransformComponent>()->SetPosition(slotPos[3].x, slotPos[3].y);
	objMgr->AddUIObject(hpPotionSlot);

	auto hpPotionPlus = QuickSlotPlus::Create(objMgr);
	hpPotionPlus->GetComponent<TransformComponent>()->SetPosition(slotPos[3].x, slotPos[3].y);
	objMgr->AddUIObject(hpPotionPlus);
}

void InventoryUIBuilder::BuildInventorySlots(ObjectManager* objMgr, InventoryManager* invMgr)
{
	const _vec2 topLeft = {630.f, 200.f};
	const float slotSpacingX = 120.f;
	const float slotSpacingY = 110.f;
	const int rows = 5;
	const int cols = 3;

	for (int y = 0; y < rows; ++y)
	{
		for (int x = 0; x < cols; ++x)
		{
			const float px = topLeft.x + x * slotSpacingX;
			const float py = topLeft.y + y * slotSpacingY;

			auto slot = InventorySlot::Create(objMgr);
			slot->GetComponent<TransformComponent>()->SetPosition(px, py);
			objMgr->AddUIObject(slot);

			invMgr->RegisterSlot(slot, SlotItemType::Any);
		}
	}
}
