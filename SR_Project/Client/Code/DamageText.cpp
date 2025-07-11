#include "pch.h"
#include "DamageText.h"
#include "TransformComponent.h"
#include "FontComponent.h"
#include "UIRenderer.h"
#include "WorldUIComponent.h"
#include "GraphicDevice.h"
#include "EngineCore.h"
#include "RenderSystem.h"

DamageText* DamageText::Create(ObjectManager* owner)
{
	auto* instance = new DamageText(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT DamageText::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto font      = AddComponent<FontComponent>();

	font->UseTransform(true, DT_CENTER | DT_BOTTOM);
	RECT rc = {0, 0, 200, 80};
	font->AddText(L"", rc, D3DXCOLOR(1, 0.2f, 0.2f, 1), DT_CENTER | DT_VCENTER ,FontType::QuestTitle);
	renderer->SetTexture(L"debugui");

	return S_OK;
}

void DamageText::Update(float dt)
{
	Object::Update(dt);
	elapsed += dt;

	offsetX += screenDir.x * kSideMove * dt;
	offsetY += screenDir.y * kSideMove * dt + kAscend * dt;

	_vec2 base = WorldUIComponent::ToScreen(worldPos, ScreenMode::Position);

	_vec3 scr(base.x + offsetX, base.y + offsetY, 0.f);
	GetComponent<TransformComponent>()->SetWorldPosition(scr);

	float alpha = clamp(1.f - elapsed / lifeTime, 0.f, 1.f);
	GetComponent<FontComponent>()->SetAlpha(alpha);
}