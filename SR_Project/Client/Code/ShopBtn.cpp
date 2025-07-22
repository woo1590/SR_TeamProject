#include "pch.h"
#include "ShopBtn.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "HoverComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "InventoryCam.h"
#include "ObjectManager.h"
#include "SlotComponent.h"
#include "FontComponent.h"

ShopBtn* ShopBtn::Create(ObjectManager* owner)
{
	auto instance = new ShopBtn(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ShopBtn::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto base = AddComponent<UIRenderer>();
	auto highlight = AddComponent<UIRenderer>();
	auto hover = AddComponent<HoverComponent>();
	auto slot = AddComponent<SlotComponent>();
	auto font = AddComponent<FontComponent>();

	tf->SetPosition(800.f, 600.f);
	tf->SetScale(4.f, 1.6f);

	base->SetTexture(L"shop_button");
	base->SetRenderType(UIRenderType::Shop);

	highlight->SetTexture(L"quickslot_hover");
	highlight->SetRenderType(UIRenderType::Shop);
	highlight->SetVisible(false);
	highlight->SetScale(8.f, 3.5f);

	hover->SetUpdateCallBack([base](bool isHovered) {
		const auto& input = EngineCore::GetInstance()->GetInputSystem();
		if (isHovered && input->IsKeyDown(KEY::LBUTTON))
			base->SetAlpha(0.5f);
		else
			base->SetAlpha(1.f);
		});

	slot->BindRenderers(base, highlight);
	slot->SetSlotType(SlotType::Quick);

	return S_OK;
}

void ShopBtn::Update(float dt)
{
	Object::Update(dt);

	auto font = GetComponent<FontComponent>();
	auto pos = GetComponent<TransformComponent>()->GetPosition();
	font->ClearText();

	font->AddText(L"아이템 구매", {700,560,900,600}, Color::White, DT_CENTER | DT_VCENTER, FontType::CookieRunFont);
	font->AddText(L"확인하려면 계속 누르세요", {700,600,900,650}, Color::White, DT_CENTER | DT_VCENTER, FontType::TipText);
}