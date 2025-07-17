#include "pch.h"
#include "SwordItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "UIManager.h"
#include "InventoryManager.h"

SwordItem* SwordItem::Create(ObjectManager* owner)
{
	auto* instance = new SwordItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT SwordItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.4f, 0.4f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::Inventory);
	renderer->SetTexture(L"inventory_sword");

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();

	item->SetOriginalScale({0.4f, 0.4f});

	ItemInfo swordInfo;

	// 1. 기본정보
	swordInfo.name = L"검";
	swordInfo.renderKey = L"inventory_sword";
	swordInfo.type = ItemType::Sword;
	swordInfo.rarity = Rarity::Default;
	swordInfo.value = 10.f;

	// 2. 텍스트 정보 설정
	swordInfo.flavorText = L"튼튼하고 믿고 쓸 수 있는 검입니다.";
	swordInfo.bgText = L"오랜 시간 숙련된 대장장이가 제련하여 날카로움이 살아있습니다.";

	// 3. 세부 스탯 정보 설정
	ItemStat stat;
	stat.description = L"근접 공격력";
	stat.unit = L"";
	//stat.iconKey = L"sworddamageicon";

	swordInfo.stats.push_back(stat);

	info->SetInfo(swordInfo);

	return S_OK;
}
