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
	void Update(_float dt);
	void Reset();

private:
	void Free()override;

	LPDIRECT3DBASETEXTURE9 texture;
	_uint currFrame;
	_uint totalFrame;
	_float timer = 0.f;
	_float speed;
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
	void Update(_float dt)override;
    void Render()override;

private:
	std::unordered_map<std::string, Sprite*> SpriteMap;
	std::string playSprite;
};

END