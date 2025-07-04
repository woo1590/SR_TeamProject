#include "pch.h"
#include "LevelFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"


LevelFront* LevelFront::Create(ObjectManager* owner)
{
	auto* instance = new LevelFront(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LevelFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto front = AddComponent<UIRenderer>();
	auto back = AddComponent<UIRenderer>();

	transform->SetPosition(120.f, 355.f);

	front->SetTexture(L"level_front");
	back->SetTexture(L"level_back");

	front->SetRenderType(UIRenderType::Inventory);
	back->SetRenderType(UIRenderType::Inventory);

	front->SetScale(0.65f ,0.4f);
	back->SetScale(0.38f, 0.45f);

	return S_OK;
}