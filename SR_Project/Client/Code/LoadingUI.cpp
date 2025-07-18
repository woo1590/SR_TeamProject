#include "pch.h"
#include "LoadingUI.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "FontComponent.h"

LoadingUI* LoadingUI::Create(ObjectManager* owner)
{
	auto* instance = new LoadingUI(owner);
	
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LoadingUI::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto font = AddComponent<FontComponent>();
	
	transform->SetScale(0.65f, 0.4f);
	transform->SetPosition(WINCX * 0.5f, WINCY * 0.5f); 

	renderer->SetTexture(L"loadingscene");

	return S_OK;
}

void LoadingUI::Update(float dt)
{
	Object::Update(dt);
	
	pulseTime += dt;
	scaletime

	float alpha = 0.5f + 0.5f * sinf(pulseTime * 2.f);
	auto font = GetComponent<FontComponent>();

	font->ClearText();

	if (!visible) return;

	font->AddText(L"아무 버튼이나 누르세요", 
		{0, 600, 1280, 720},
		D3DXCOLOR(1.f,1.f,1.f,alpha),
		DT_CENTER, 
		FontType::DmgText);
}