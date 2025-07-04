#include "pch.h"
#include "MapBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ButtonComponent.h"
#include "HoverComponent.h"

MapBtn* MapBtn::Create(ObjectManager* owner)
{
	auto* instance = new MapBtn(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;;
}

HRESULT MapBtn::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	transform->SetPosition(780.f,665.f);

	renderer->SetScale(0.4f, 0.4f);
	renderer->SetTexture(L"mapbtn");

	return S_OK;

}

void MapBtn::Free()
{
	Object::Free();
}
