#include "pch.h"
#include "HPBarBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "PlayerInfoComponent.h"

HPBarBack* HPBarBack::Create(ObjectManager* owner, ObjectType type)
{
	auto* instance = new HPBarBack(owner, type);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();

	renderer->SetScale(0.25f, 0.25f);

	transform->SetPosition(WINCX * 0.5f, WINCY * 0.9f);

	renderer->SetTexture(L"hpbar_back");
	return S_OK;
}
