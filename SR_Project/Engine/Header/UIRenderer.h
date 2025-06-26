#pragma once

#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL UIRenderer : public RendererComponent
{
public:
	UIRenderer(Object* owner, const wstring& key);
	virtual ~UIRenderer() = default;

public:
	static UIRenderer* Create(Object* owner, const wstring& key);

	void Render() override;

private:
	LPDIRECT3DTEXTURE9 texture = nullptr;
	RECT srcRect{};
	_vec3 pos{};
	_vec3 center{};
};

END