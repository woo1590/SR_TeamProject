#include "pch.h"
#include "LoadingSpinner.h"
#include "TransformComponent.h"
#include "UIRenderer.h"
#include "ObjectManager.h"

namespace
{
	constexpr int   NUM_DOTS = 8;        
	constexpr float RADIUS = 15.f;       
	constexpr float DOT_SCALE = 0.3f;    
	constexpr float ANIM_SPEED = 5.f;    
	constexpr float FADE_DELAY = (2.f * D3DX_PI) / NUM_DOTS;
}

LoadingSpinner* LoadingSpinner::Create(ObjectManager* owner)
{
	auto instance = new LoadingSpinner(owner);
	return (FAILED(instance->Ready_Object())) ? Safe_Release(instance), nullptr : instance;
}

HRESULT LoadingSpinner::Ready_Object()
{
	auto tf = AddComponent<TransformComponent>();
	auto r = AddComponent<UIRenderer>();

	constexpr int dotsPerSide = NUM_DOTS / 4;

	r->SetTexture(L"debugui");
	r->SetRenderType(UIRenderType::QuestUI);
	tf->SetPosition(1060.f, 650.f);

	const vector<_vec2> waypoints =
	{
		{-RADIUS, -RADIUS},  // 1. ÁÂ»ó´Ü
		{0.f,     -RADIUS},  // 2. »ó´Ü Áß¾Ó
		{RADIUS,  -RADIUS},  // 3. ¿ì»ó´Ü
		{RADIUS,      0.f},  // 4. ¿ìÃø Áß¾Ó
		{RADIUS,   RADIUS},  // 5. ¿ìÇÏ´Ü
		{0.f,      RADIUS},  // 6. ÇÏ´Ü Áß¾Ó
		{-RADIUS,  RADIUS},  // 7. ÁÂÇÏ´Ü
		{-RADIUS,     0.f}   // 8. ÁÂÃø Áß¾Ó
	};

	for (int i{}; i < NUM_DOTS; ++i)
	{
		if (i >= waypoints.size()) break;

		auto dot = DialogRect::Create(owner);
		auto dotTf = dot->GetComponent<TransformComponent>();

		dotTf->SetParent(this);
		dotTf->SetPosition(waypoints[i].x, waypoints[i].y); 
		dotTf->SetScale(DOT_SCALE, DOT_SCALE);

		owner->AddUIObject(dot);
		dots.push_back(dot);
	}

	return S_OK;
}

void LoadingSpinner::Update(float dt)
{
	Object::Update(dt);
	animTime += dt * ANIM_SPEED;

	for (int i{}; i < dots.size(); ++i)
	{
		auto renderer = dots[i]->GetComponent<UIRenderer>();
		float sinValue = sin(animTime - (i * FADE_DELAY));
		float alpha = (sinValue + 1.f) / 2.f;
		renderer->SetAlpha(alpha);
	}
}
