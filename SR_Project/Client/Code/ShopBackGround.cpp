#include "pch.h"
#include "ShopBackGround.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"
#include "InfoComponent.h"
#include "ObjectManager.h"
#include "ShopEmerald.h"

ShopBackGround* ShopBackGround::Create(ObjectManager* owner)
{
	auto instance = new ShopBackGround(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ShopBackGround::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto r = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();

	tf->SetPosition(WINCX * 0.5f, WINCY * 0.5f);
	tf->SetScale(0.65f, 0.4f);
	r->SetTexture(L"shop_back");
	r->SetRenderType(UIRenderType::Shop);

	owner->AddUIObject(ShopEmerald::Create(owner));

	return S_OK;
}

void ShopBackGround::Update(float dt)
{
	Object::Update(dt);

	auto playerInfo = owner->GetFrontObject(ObjectType::Player)->GetComponent<InfoComponent<PlayerInfo>>()->GetInfo();

	int gold = playerInfo.gold;

	auto font = GetComponent<FontComponent>();
	font->ClearText();

	font->AddText(L"마을 상인", {50, 550, 400, 600}, Color::Yellow, DT_LEFT, FontType::MineCraftFont);
	font->AddText(L"이 상인은 합당한 가격에 일반 및 희귀한 아이템을 판매합니다", {50,600,400,700}, Color::White, DT_LEFT | DT_WORDBREAK, FontType::TipText);

	font->AddText(to_wstring(gold), {200,90,300,200}, Color::White, DT_LEFT, FontType::MineCraftFont);
	font->AddText(L"판매용 아이템", {475, 20, 600, 200}, Color::Yellow, DT_LEFT, FontType::TipText);
}