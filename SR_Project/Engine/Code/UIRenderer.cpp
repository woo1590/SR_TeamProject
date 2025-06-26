#include "EnginePCH.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "TransformComponent.h"

UIRenderer::UIRenderer(Object* owner)
	:RendererComponent(owner,RENDER_ID::Render_UI)
{
	
}

UIRenderer* UIRenderer::Create(Object* owner)
{
	UIRenderer* instance = new UIRenderer(owner);
	
	if (FAILED(instance->Ready_Component()))
	{
		Safe_Release(instance);
		instance = nullptr;
	}

	EngineCore::GetInstance()->GetRenderSystem()->RegisterRenderer(RENDER_ID::Render_UI, instance);
	
	return instance;
}

void UIRenderer::SetTexture(const wstring& key)
{
	auto resourceMgr = EngineCore::GetInstance()->GetResourceManager();
	texture = resourceMgr->GetTexture(key);
	/////////////////////////////////////////////////////////////////////
	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	srcRect = {0, 0, LONG(desc.Width), LONG(desc.Height)};
	center = {desc.Width * 0.5f, desc.Height * 0.5f, 0.f};


	pos = center;
}

void UIRenderer::Render()
{
	auto sprite = EngineCore::GetInstance()->GetRenderSystem()->GetSpriteBatch();

	sprite->Draw(texture, &srcRect, &center, &pos, D3DCOLOR_ARGB(255, 255, 255, 255));
}
