#include "pch.h"
#include "InventoryUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "ObjectManager.h"
#include "InfoComponent.h"

InventoryUI* InventoryUI::Create(ObjectManager* owner)
{
	auto* instance = new InventoryUI(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT InventoryUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto font      = AddComponent<FontComponent>();

	renderer->SetRenderType(UIRenderType::Inventory);

	return S_OK;
}

void InventoryUI::Update(float dt)
{
	const auto& info = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo();
	auto font = GetComponent<FontComponent>();

	font->ClearText();

	font->AddText(L"·¹º§", {102, 325, 250, 380}, Color::White, DT_LEFT | DT_TOP, FontType::TipText);
	font->AddText(L"" + to_wstring(info.level), {115,350,250,390}, Color::White, DT_LEFT | DT_TOP, FontType::TipText);
	font->AddText(L"Èû", {505,320,580,500},Color::White, DT_LEFT | DT_TOP, FontType::TipText);
	font->AddText(to_wstring((int)info.power), {500, 350, 580, 500}, Color::White, DT_LEFT | DT_TOP, FontType::TipText);
}