#include "pch.h"
#include "HPBarFront.h"
#include "TransformComponent.h"
#include "HpComponent.h"

HPBarFront* HPBarFront::Create(ObjectManager* owner, ObjectType type, PlayerInfoComponent* playerInfo)
{
	auto* instance = new HPBarFront(owner, type);
	
	return (FAILED(instance->Ready_Object(playerInfo))) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarFront::Ready_Object(PlayerInfoComponent* playerInfo)
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(600.f, 500.f);
	//transform->SetScale(0.5f, 0.5f, 0.5f);

	auto hpUI = AddComponent<HpComponent>();

	if (playerInfo)
		hpUI->AttachPlayerInfo(playerInfo);

	return S_OK;
}