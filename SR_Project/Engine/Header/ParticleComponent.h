#pragma once

#include "ObjectComponent.h"

BEGIN(Engine)

class UIRenderer;

class ENGINE_DLL ParticleComponent : public ObjectComponent
{
private:
	explicit ParticleComponent(Object* owner)
		:ObjectComponent(owner) {}

public:
	static ParticleComponent* Create(Object* owner);
	HRESULT Ready_Component();

public:
	void SetColor(D3DXCOLOR _color) { color = _color; }
	void SetLife(float seconds) { life = max(0.01f, seconds); }
	void SetSizeRange(float start, float end);
	void SetGlow(bool enable) { glow = enable; }

	void Update(float dt) override;
	void Render();

private:
	float elapsed = 0.f;
	float life = 3.f;
	float startSize = 1.f;
	float endSize = 1.f;
	bool glow = false;

	D3DXCOLOR color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	UIRenderer* renderer = nullptr;
};

END