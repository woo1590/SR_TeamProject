#include "pch.h"
#include "Cursor.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "InputSystem.h"

Cursor* Cursor::Create(ObjectManager* owner)
{
	auto* instance = new Cursor(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT Cursor::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();

	renderer->SetTexture(L"Cursor");
	renderer->SetPivot(UIPivot::LeftTop);

	return S_OK;
}

void Cursor::Update(float dt)
{
	auto transform = GetComponent<TransformComponent>();

	_vec3 mousePos = EngineCore::GetInstance()->GetInputSystem()->GetMousePos();

	transform->SetPosition(mousePos.x, mousePos.y);
}
