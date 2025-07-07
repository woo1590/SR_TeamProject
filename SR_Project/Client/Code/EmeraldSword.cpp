#include "pch.h"
#include "EmeraldSword.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "InfoComponent.h"
#include "ItemComponent.h"

EmeraldSword* EmeraldSword::Create(ObjectManager* owner)
{
	auto* instance = new EmeraldSword(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT EmeraldSword::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto info = AddComponent<InfoComponent<ItemInfo>>();
	auto item = AddComponent<ItemComponent>();

	transform->SetPosition(500.f, 500.f);

	renderer->SetTexture(L"emerald_sword");
	renderer->SetScale(0.5f, 0.5f);

	return S_OK;
}