#include "pch.h"
#include "UIObj.h"
#include "TransformComponent.h"
#include "UIRenderer.h"

UIObj::UIObj(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

UIObj* UIObj::Create(ObjectManager* owner, ObjectType objType)
{
	UIObj* instance = new UIObj(owner, objType);

	if (FAILED(instance->Ready_Object()))
	{
		Safe_Release(instance);
		instance = nullptr;
	}
	return instance;
}

HRESULT UIObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>(L"Shield");

	return S_OK;
}

void UIObj::Update(float dt)
{
	Object::Update(dt);
}

void UIObj::Late_Update(float dt)
{
	Object::Late_Update(dt);
}

void UIObj::Free()
{
	Object::Free();
}
