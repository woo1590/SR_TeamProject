#include "EnginePCH.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "TransformComponent.h"

UIRenderer::UIRenderer(Object* owner, const wstring& key)
	:RendererComponent(owner,RENDER_ID::Render_UI)
{
	auto resourceMgr = EngineCore::GetInstance()->GetResourceManager();
	texture = resourceMgr->GetTexture(key);

	D3DSURFACE_DESC desc;
	texture->GetLevelDesc(0, &desc);
	srcRect = {0, 0, LONG(desc.Width), LONG(desc.Height)};
	center = {desc.Width * 0.5f, desc.Height * 0.5f, 0.f};

	// 일단 기본설정
	pos = center;
}

UIRenderer* UIRenderer::Create(Object* owner, const wstring& key)
{
	auto resourceMgr = EngineCore::GetInstance()->GetResourceManager();
	if (!resourceMgr->GetTexture(key))
		return nullptr;

	UIRenderer* instance = new UIRenderer(owner, key);
	
	if (FAILED(instance->Ready_Component()))
	{
		Safe_Release(instance);
		instance = nullptr;
	}

	EngineCore::GetInstance()->GetRenderSystem()->RegisterRenderer(RENDER_ID::Render_UI, instance);
	
	return instance;
}

void UIRenderer::Render()
{

	auto sprite = EngineCore::GetInstance()->GetRenderSystem()->GetSpriteBatch();

	sprite->Draw(texture, &srcRect, &center, &pos, D3DCOLOR_ARGB(255, 255, 255, 255));
}
