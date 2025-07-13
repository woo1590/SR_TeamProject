#include "pch.h"
#include "PlayerDeathUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

PlayerDeathUI* PlayerDeathUI::Create(ObjectManager* owner)
{
	auto* instance = new PlayerDeathUI(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PlayerDeathUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>(); 
	auto renderer = AddComponent<UIRenderer>();
	transform->SetPosition(645.f,360.f);
	transform->SetScale(2.55f, 1.5f);

	renderer->SetTexture(L"player_deathui");
	renderer->SetRenderType(UIRenderType::DeathUI);

	return S_OK;
}