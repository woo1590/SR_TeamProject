#include "pch.h"
#include "PigItem.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

PigItem* PigItem::Create(ObjectManager* owner)
{
	auto* instance = new PigItem(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PigItem::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(0.3f, 0.3f);

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"pig");
	renderer->SetRenderType(UIRenderType::Inventory);

	auto info = AddComponent<InfoComponent<ItemInfo>>();
	info->SetInfo({L"아기 돼지", L"pig", ItemType::Costume, Rarity::Default, 0,
		L"지금 이 즐거운 꿀꿀 소리는 여러분이 내는 건가요, 아님 이 사랑스러운 아기 돼지가 내는 건가요? 새로운 반려동물이 얼마나 귀여운지 정말 깨물어 주고 싶네요! 하지만 그러면 안돼겠죠"});

	auto item = AddComponent<ItemComponent>();

	return S_OK;
}