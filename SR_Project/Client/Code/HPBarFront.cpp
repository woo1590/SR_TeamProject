#include "pch.h"
#include "HPBarFront.h"
#include "TransformComponent.h"
#include "HpComponent.h"
#include "UIRenderer.h"

HPBarFront* HPBarFront::Create(ObjectManager* owner, ObjectType type, PlayerInfoComponent* playerInfo)
{
	auto* instance = new HPBarFront(owner, type);
	
	return (FAILED(instance->Ready_Object(playerInfo))) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarFront::Ready_Object(PlayerInfoComponent* playerInfo)
{
	auto transform = AddComponent<TransformComponent>();
	auto hpUI      = AddComponent<HpComponent>();
	auto renderer  = GetComponent<UIRenderer>();

	renderer->SetScale(0.32f, 0.32f);
	transform->SetPosition(WINCX * 0.5f, WINCY * 0.96f);

	if (playerInfo)
		hpUI->AttachPlayerInfo(playerInfo);

	return S_OK;
}