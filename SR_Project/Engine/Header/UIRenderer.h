#pragma once

#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL UIRenderer : public RendererComponent
{
public:
	UIRenderer(Object* owner);
	virtual ~UIRenderer() = default;

public:
	static UIRenderer* Create(Object* owner);

	void SetTexture(const wstring& key);
	void Render() override;

private:
	LPDIRECT3DBASETEXTURE9 texture = nullptr;
	IDirect3DTexture9* tex2D = nullptr;
	RECT srcRect{};
	_vec3 pos{};
	_vec3 center{};
};

END