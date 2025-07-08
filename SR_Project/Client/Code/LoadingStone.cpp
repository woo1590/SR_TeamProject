#include "pch.h"
#include "LoadingStone.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "AnimatorComponent.h"
#include "FontComponent.h"

LoadingStone* LoadingStone::Create(ObjectManager* owner)
{
	auto* instance = new LoadingStone(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LoadingStone::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(150.f, 500.f);
	//transform->SetScale(1.f,1.f);
	//transform->SetScale(0.8f, 0.8f); // 

	auto renderer = AddComponent<UIRenderer>();
	renderer->SetTexture(L"loading_stone");
	renderer->SetRenderType(UIRenderType::WorldMap);
	//renderer->SetPivot(UIPivot::Left);

	auto animator = AddComponent<AnimatorComponent>();
	animator->SetFrameSize(140, 140);
	animator->SetTotalFrameCount(11);
	animator->Play(0, 10, 0.15f); 
//	animator->SetFrame(3);

	auto font = AddComponent<FontComponent>();

	return S_OK;
}

void LoadingStone::Update(float dt)
{
	Object::Update(dt);

	auto font = GetComponent<FontComponent>();
	auto animator = GetComponent<AnimatorComponent>();

	font->ClearText();

	wstring txt = L"";

	txt += L"CurFrame: " + to_wstring(animator->GetCurFrame()) + L"\n";

	RECT r = animator->GetCurRect(); // ← 아래에서 GetCurRect() 함수 추가한다고 가정
	txt += L"Rect: (" + to_wstring(r.left) + L"," + to_wstring(r.right) + L")\n";

	txt += L"Speed: " + to_wstring(animator->GetSpeed()) + L"\n";

	font->AddText(txt, {300, 300, 600, 600});
}