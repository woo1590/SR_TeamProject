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
	SetVisible(false);
	return S_OK;
}

void PanelComponent::SetVisible(bool visible)
{
	if (auto renderer = owner->GetComponent<UIRenderer>())
		renderer->SetVisible(visible);
}

void PanelComponent::Toggle()
{
	if (auto renderer = owner->GetComponent<UIRenderer>())
		renderer->SetVisible(!renderer->IsVisible());
}

bool PanelComponent::IsVisible() const
{
	if (auto renderer = owner->GetComponent<UIRenderer>())
		return renderer->IsVisible();
	return false;
}