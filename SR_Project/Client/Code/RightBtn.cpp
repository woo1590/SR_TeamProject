#include "pch.h"
#include "RightBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "ObjectManager.h"
#include "InventoryCam.h"

RightBtn* RightBtn::Create(ObjectManager* owner)
{
	auto instance = new RightBtn(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT RightBtn::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	
	tf->SetPosition(450.f, 500.f);

	renderer->SetTexture(L"button_right");
	renderer->SetRenderType(UIRenderType::Inventory);

	hover->SetUpdateCallBack([renderer](bool isHovered) {
		const auto& input = EngineCore::GetInstance()->GetInputSystem();
		if (isHovered && input->IsKeyDown(KEY::LBUTTON))
			renderer->SetAlpha(0.5f);
		else
			renderer->SetAlpha(1.f);
		});

	auto cam = dynamic_cast<InventoryCam*>(owner->GetFrontObject(ObjectType::UICamera));
	cam->SetRightBtn(this);

	return S_OK;
}
