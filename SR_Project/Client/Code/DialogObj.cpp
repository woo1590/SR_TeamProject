#include "pch.h"
#include "DialogObj.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

DialogObj* DialogObj::Create(ObjectManager* owner)
{
	auto* instance = new DialogObj(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT DialogObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	return S_OK;
}