#include "pch.h"
#include "HotBarBack.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "ObjectManager.h"
#include "InfoComponent.h"

HotBarBack* HotBarBack::Create(ObjectManager* owner)
{
	auto* instance = new HotBarBack(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT HotBarBack::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer  = AddComponent<UIRenderer>();
	auto font      = AddComponent<FontComponent>();


	transform->SetPosition(WINCX * 0.5f, WINCY * 0.96f);
	
	transform->SetScale(0.4f, 0.3f);
	renderer->SetTexture(L"hotbar_back");

	return S_OK;
}

void HotBarBack::Update(float dt)
{
	const auto& info = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo();
	auto font = GetComponent<FontComponent>();

	font->ClearText();

	font->AddText(L"I", {315, 659, 350, 700}, Color::White);
	font->AddText(L"1", {400, 659, 450, 700}, Color::White);
	font->AddText(L"2", {470, 659, 500, 700}, Color::White);
	font->AddText(L"3", {540, 659, 560, 700}, Color::White);
	font->AddText(L"E", {720, 659, 750, 700}, Color::White);
	font->AddText(L"M", {780, 659, 820, 700}, Color::White);

	font->AddText(L"Lv:" + to_wstring(info.level), {630, 700, 680, 740});
}