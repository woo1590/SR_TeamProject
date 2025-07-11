#include "pch.h"
#include "ParticleObj.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ParticleComponent.h"
#include "FontComponent.h"

ParticleObj* ParticleObj::Create(ObjectManager* owner)
{
	auto* instance = new ParticleObj(owner);

	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT ParticleObj::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();
	auto renderer = AddComponent<UIRenderer>();
	auto particle = AddComponent<ParticleComponent>();
	auto font = AddComponent<FontComponent>();

	transform->SetScale(10.f, 10.f);
	transform->SetPosition(200.f, 200.f);

	renderer->SetLayer(15);
	renderer->SetTexture(L"debugui");

	particle->SetColor(D3DXCOLOR(1.f, 1.f, 0.5f, 1.f));
	particle->SetLife(10.f);
	particle->SetSizeRange(0.2f, 1.f);
	particle->SetGlow(true);
	
	return S_OK;
}

void ParticleObj::Update(float dt)
{
	Object::Update(dt);
	
	auto transform = GetComponent<TransformComponent>();
	_vec3 pos = transform->GetPosition();

	auto font = GetComponent<FontComponent>();

	font->ClearText();

	//font->AddText(L"pos: (" + to_wstring((int)pos.x) + L" ," + to_wstring((int)pos.y)
	//	+ L" ," + to_wstring((int)pos.z) + L" )", {100,100,500,300},Color::White,DT_CENTER,FontType::QuestTitle);
}