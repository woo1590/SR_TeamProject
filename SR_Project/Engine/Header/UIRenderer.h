#pragma once

#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL UIRenderer : public RendererComponent
{
public:
	explicit UIRenderer(Object* owner);
	virtual ~UIRenderer() = default;

public:
	static UIRenderer* Create(Object* owner);

	void SetTexture(const wstring& key);
	void Render() override;

	void SetRatio(float _ratio) { targetRatio = clamp(_ratio, 0.f, 1.f); }
	void ApplyRatio(float _ratio);

	void SetLayer(int idx) { layerIdx = idx; }
	int  GetLayer() const { return layerIdx; }

private:
	LPDIRECT3DBASETEXTURE9 texture = nullptr;
	IDirect3DTexture9* tex2D = nullptr;
	int layerIdx = 0;

	RECT  srcRect{};
	_vec3 pos{};
	_vec3 center{};

	float curRatio = 1.f;
	float targetRatio = 1.f;
	float lerpSpeed = 6.f;

	LONG fullWidth = 0;
	LONG fullHeight = 0;
};

END