#include "EnginePCH.h"
#include "PanelComponent.h"
#include "Object.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "InputSystem.h"

PanelComponent* PanelComponent::Create(Object* owner)
{
	auto* instance = new PanelComponent(owner);

	return (FAILED(instance->Ready_Component())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT PanelComponent::Ready_Component()
{
	SetVisible(isVisible);
	return S_OK;
}

void PanelComponent::SetVisible(bool visible)
{
	isVisible = visible;

	auto renderer = owner->GetComponent<UIRenderer>();
	renderer->SetVisible(visible);
}

void PanelComponent::Toggle()
{
	SetVisible(!isVisible);
}

void PanelComponent::Update(float dt)
{
	const auto& input = EngineCore::GetInstance()->GetInputSystem();
	
	if (input->IsKeyPressed(KEY::I))
		Toggle();
}