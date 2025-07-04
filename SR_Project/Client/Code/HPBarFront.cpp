#include "pch.h"
#include "HPBarFront.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ProgressBar.h"
#include "HoverComponent.h"
#include "FontComponent.h"
#include "Player.h"
#include "ObjectManager.h"
#include "InfoComponent.h"
#include "InputSystem.h"

#include "EngineCore.h"

HPBarFront* HPBarFront::Create(ObjectManager* owner)
{
	auto* instance = new HPBarFront(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HPBarFront::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto hpUI      = AddComponent<ProgressBar<PlayerInfo>>();
	auto hover     = AddComponent<HoverComponent>();
	auto font      = AddComponent<FontComponent>();
	
	renderer->SetTexture(L"hpbar_front");
	renderer->SetPivot(UIPivot::Bottom);
	renderer->SetScale(0.35f, 0.32f);
	transform->SetPosition(610.f, 690.f);

	font->SetFontType(FontType::Title);

	hover->SetUpdateCallBack([this](bool isHovered) {
		const auto& input = EngineCore::GetInstance()->GetInputSystem();
		auto font = GetComponent<FontComponent>();
		font->ClearText();

		if (!isHovered) return;
		});

	return S_OK;
}
