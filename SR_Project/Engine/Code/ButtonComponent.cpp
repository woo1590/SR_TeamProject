#include "EnginePCH.h"
#include "ButtonComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "Object.h"
#include "HoverComponent.h"

ButtonComponent* ButtonComponent::Create(Object* owner)
{
	auto* instance = new ButtonComponent(owner);
	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ButtonComponent::Ready_Component()
{
	auto hover = owner->GetComponent<HoverComponent>();
	assert(hover && "HoverComponent missing");

	hover->SetCallBack([this](bool over){ isHovered = over;});
	return S_OK;
}

void ButtonComponent::ApplyHover(bool over)
{
	if (!renderer || baseKey.empty() || hoverKey.empty()) return;

	renderer->SetTexture(over ? hoverKey : baseKey);
}


void ButtonComponent::Update(float dt)
{
	if (prevHover != isHovered)
	{
		ApplyHover(isHovered);
		prevHover = isHovered;
	}

	if (!isHovered) return;

	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	if (input->IsKeyPressed(KEY::LBUTTON) && onClick)
		onClick();
}