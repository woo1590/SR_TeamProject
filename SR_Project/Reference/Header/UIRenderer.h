#pragma once

#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL UIRenderer : public RendererComponent
{
public:
	explicit UIRenderer(Object* owner)
		:RendererComponent(owner, RENDER_ID::Render_UI) {}
	virtual ~UIRenderer()
	{
		if (tex2D)
			tex2D->Release();

		if (texture)
			texture->Release();
	}

public:
	static UIRenderer* Create(Object* owner);

	void SetTexture(const wstring& key);
	void Render() override;

	void SetRatio(float _ratio) { targetRatio = clamp(_ratio, 0.f, 1.f); }
	void ApplyRatioVertical(float _ratio);
	void ApplyRatioHorizontal(float _ratio);

	void SetLayer(int idx) { layerIdx = idx; }
	int  GetLayer() const { return layerIdx; }

	void SetPivot(UIPivot _pivot);
	void UpdateCenter();

	void SetScale(float x, float y) { scale = {x, y}; }
	
	_vec2 GetScale() const { return scale; }
	LONG GetFullWidth() const { return fullWidth; }
	LONG GetFullHeight() const { return fullHeight; }

	void SetVisible(bool visible) { isVisible = visible; }
	bool IsVisible() const { return isVisible; }

private:
	void Free()override{}

	LPDIRECT3DBASETEXTURE9 texture = nullptr;
	IDirect3DTexture9* tex2D = nullptr;
	RECT  srcRect{};
	_vec3 center{}, pos{};

	float curRatio = 1.f, targetRatio = 1.f;
	float lerpSpeed = 6.f;

	LONG fullWidth = 0, fullHeight = 0;
	int layerIdx = 0;

	UIPivot pivot = UIPivot::Center;
	_vec2 scale = {1.f, 1.f};

	bool isVisible = true;
};

END