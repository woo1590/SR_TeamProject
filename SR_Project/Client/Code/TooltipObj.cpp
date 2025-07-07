#include "pch.h"
#include "TooltipObj.h"
#include "TransformComponent.h"
#include "InfoComponent.h"

TooltipObj* TooltipObj::Create(ObjectManager* owner)
{
	auto* instance = new TooltipObj(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT TooltipObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(1100.f, 200.f);
	transform->SetScale(2.f, 2.f);
	renderer = AddComponent<UIRenderer>();
	renderer->SetRenderType(UIRenderType::Inventory);
	renderer->SetVisible(false);

	font = AddComponent<FontComponent>();

	return S_OK;
}

void TooltipObj::Update(float dt)
{
	if (!inventory) return;

	auto* selectedSlot = inventory->GetSelectedSlot();
	if (!selectedSlot || !selectedSlot->HasItem())
	{
		font->ClearText();
		renderer->SetVisible(false);
		prevSlot = nullptr;
		return;
	}

	if (selectedSlot == prevSlot) return;

	auto* itemObj = selectedSlot->GetItem();
	if (!itemObj) return;

	auto* infoComp = itemObj->GetComponent<InfoComponent<ItemInfo>>();
	if (!infoComp) return;

	const auto& info = infoComp->GetInfo();

	font->ClearText();

	RECT nameRect = {1010, 290, 1280, 440};
	RECT valueRect = {1010, 360, 1280, 570};
	RECT descRect = {1010, 410, 1280, 730};

	font->AddText(info.name, nameRect, Color::White, DT_LEFT, FontType::Title);

	switch (info.type)
	{
	case ItemType::Sword:
		font->AddText(L"근접 공격력:  " + to_wstring(info.value), valueRect, Color::White, DT_LEFT, FontType::Large);
		break;
	case ItemType::Armor:
		font->AddText(L"추가 체력: + " + to_wstring(info.value), valueRect, Color::White, DT_LEFT, FontType::Large);
		break;
	case ItemType::Potion:
		font->AddText(to_wstring(info.value) + L" 초 쿨타임", valueRect, Color::White, DT_LEFT, FontType::Large);
		break;
	case ItemType::Arrow:
		font->AddText(L"원거리 공격력:  " + to_wstring(info.value), valueRect, Color::White, DT_LEFT, FontType::Large);
		break;
	default:
		font->AddText(L"아이템 수치:  " + to_wstring(info.value), valueRect, Color::White, DT_LEFT, FontType::Large);
		break;
	}

	font->AddText(info.description, descRect, Color::White, DT_WORDBREAK, FontType::Title);

	renderer->SetTexture(info.renderKey); 
	renderer->SetScale(0.5f, 0.5f);
	renderer->SetVisible(true);

	prevSlot = selectedSlot;
}
