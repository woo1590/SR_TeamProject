#include "EnginePCH.h"
#include "SpriteRenderer.h"

Sprite::Sprite()
{
}

Sprite::~Sprite()
{
}

Sprite* Sprite::Create()
{
	Sprite* Instance = new Sprite();
	if (FAILED(Instance->Ready_Sprite()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT Sprite::Ready_Sprite()
{
	return S_OK;
}

void Sprite::Update(_float dt)
{
	if (currFrame < totalFrame)
	{
		timer += dt * speed;
	}
	
	if (timer >= 1.f)
	{
		currFrame++;
		timer = 0.f;
	}

	if (currFrame >= totalFrame)
	{
		if (isRepeat)
		{
			currFrame = 0.f;	
		}
		else
		{
			currFrame--;
		}
	}

}

void Sprite::Render()
{
	if (shader)
	{

	}
}

void Sprite::Reset()
{
	currFrame = 0;
	timer = 0.f;
}

void Sprite::Free()
{
	Safe_Release(texture);
}

SpriteRenderer::SpriteRenderer(Object* owner, RENDER_ID id)
	:RendererComponent(owner,id)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

SpriteRenderer* SpriteRenderer::Create(Object* owner, RENDER_ID id)
{
	SpriteRenderer* Instance = new SpriteRenderer(owner, id);
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

void SpriteRenderer::Update(_float dt)
{
	SpriteMap[playSprite]->Update(dt);
}

void SpriteRenderer::Render()
{

}

