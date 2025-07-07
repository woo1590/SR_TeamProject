#pragma once

#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL UIRenderer : public RendererComponent
{
public:
	explicit UIRenderer(Object* owner)
		:RendererComponent(owner, RENDER_ID::Render_UI) {}

public:
	static UIRenderer* Create(Object* owner);

	void SetTexture(const wstring& key);
	void Render() override;
	void RenderFont();

	void SetRatio(float _ratio) { targetRatio = clamp(_ratio, 0.f, 1.f); }
	void ApplyRatioVertical(float _ratio);
	void ApplyRatioHorizontal(float _ratio);

	void SetLayer(int idx) { layerIdx = idx; }
	int  GetLayer() const { return layerIdx; }

	void SetScale(float x, float y) { scale = {x, y}; }
	_vec2 GetScale() const { return scale; }

	void SetPivot(UIPivot _pivot);
	void UpdateCenter();

	LONG GetFullWidth() const { return fullWidth; }
	LONG GetFullHeight() const { return fullHeight; }

	void SetVisible(bool visible) { isVisible = visible; }
	bool IsVisible() const { return isVisible; }

	void SetRenderType(UIRenderType type) { renderType = type; }
	UIRenderType GetRenderType() const { return renderType; }

	static void SetCurRenderType(UIRenderType type) { curRenderType = type; }
	static UIRenderType GetCurRenderType() { return curRenderType; }

private:
	void Free()override;
	static UIRenderType curRenderType;
	UIRenderType renderType = UIRenderType::MainGame;
	LPDIRECT3DBASETEXTURE9 texture = nullptr;
	IDirect3DTexture9* tex2D = nullptr;
	RECT  srcRect{}; 
	_vec3 center{}, pos{};

	float curRatio = 1.f, targetRatio = 1.f;
	float lerpSpeed = 6.f;

	LONG fullWidth = 0, fullHeight = 0;
	int layerIdx = 0;

	bool isVisible = true;

	UIPivot pivot = UIPivot::Center;
	_vec2 scale = {1.f, 1.f};
};

END