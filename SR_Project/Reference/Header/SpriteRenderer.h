#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL Sprite : public Base
{
private:
	Sprite();
	virtual ~Sprite();

public:
	static Sprite* Create();
	HRESULT Ready_Sprite();
	Sprite* Clone();

private:
	void Free()override;

	LPDIRECT3DBASETEXTURE9 texture;
	_uint currFrame;
	_uint totalFrame;
	_float timer = 0.f;
	_bool isRepeat = false;
};

class ENGINE_DLL SpriteRenderer :
    public RendererComponent
{
private:
    SpriteRenderer(Object* owner, RENDER_ID id);
    virtual ~SpriteRenderer();

public:
    static SpriteRenderer* Create(Object* owner, RENDER_ID id);
    void Render()override;

private:
	Sprite* sprite = nullptr;
};

END