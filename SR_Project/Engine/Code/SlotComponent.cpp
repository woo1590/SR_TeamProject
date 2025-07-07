#include "EnginePCH.h"
#include "SlotComponent.h"
#include "UIRenderer.h"
#include "Object.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "Scene.h"
#include "UIManager.h"
#include "InventoryManager.h"

void SlotComponent::OnClick()
{
	auto transform = owner->GetComponent<TransformComponent>();

	if (slotType == SlotType::Quick)
	{
		originalY = transform->GetPosition().y;
		shakeTime = 0.f;
		shakeDuration = 0.05f;
		isShaking = true;
	}
	else if (slotType == SlotType::Gear || slotType == SlotType::Item || slotType == SlotType::Inventory)
	{
		isSelected = true;

		auto scene = owner->GetScene();
		if (auto uiMgr = scene->GetUIManager())
		{
			if (auto invMgr = uiMgr->GetInventory())
				invMgr->SelectSlot(this);
		}
	}
}

void SlotComponent::BindRenderers(UIRenderer* _base, UIRenderer* _highlight)
{
	base = _base;
	highlight = _highlight;
}

void SlotComponent::Update(float dt)
{
	auto transform = owner->GetComponent<TransformComponent>();
	assert(transform && "SlotComponent::Update - transformComponent is missing");

	if (isShaking)
	{
		shakeTime += dt;

		if (shakeTime >= shakeDuration)
		{
			isShaking = false;
			shakeTime = 0.f;

			_vec3 pos = transform->GetPosition();
			pos.y = originalY;
			transform->SetPosition(pos);
		}
		else
		{
			float t = shakeTime / shakeDuration;
			float offset = sinf(t * D3DX_PI) * shakePower;

			_vec3 pos = transform->GetPosition();
			pos.y = originalY + offset;
			transform->SetPosition(pos);
		}
	}

	if (highlight)
	{
		if (isSelected)
		{
			highlight->SetTexture(L"slot_selected");
			highlight->SetVisible(true);
		}
		else if (isHovered)
		{
			switch (slotType)
			{
			case SlotType::Quick:
				highlight->SetTexture(L"quickslot_hover");
				break;
			case SlotType::Gear:
			case SlotType::Item:
			case SlotType::Inventory:
				highlight->SetTexture(L"gearslot_hover");
				break;
			default:
				break;
			}
			highlight->SetVisible(true);
		}
		else
			highlight->SetVisible(false);
	}
}
