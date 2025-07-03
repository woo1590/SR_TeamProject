#include "EnginePCH.h"
#include "ButtonComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "Object.h"
#include "TransformComponent.h"
#include "HoverComponent.h"

ButtonComponent* ButtonComponent::Create(Object* owner)
{
	auto* instance = new ButtonComponent(owner);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ButtonComponent::Ready_Component()
{
	auto hover = owner->GetComponent<HoverComponent>();
	assert(hover && "ButtonComponent::Ready_Component - HoverComponent is missing");

	hover->SetCallBack([this](bool over)
		{
			isHovered = over;
			if (highlight)
				highlight->SetVisible(over);
		});

	return S_OK;
}

void ButtonComponent::Update(float dt)
{
	if (!isHovered) return;

	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	if (input->IsKeyPressed(KEY::LBUTTON) && onClick)
		onClick();
}