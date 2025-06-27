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
    // ResourceManager에서 BaseTexture9 반환
    texture = EngineCore::GetInstance()
        ->GetResourceManager()
        ->GetTexture(key);

    // IBaseTexture9 → ITexture9로 QueryInterface
    if (texture && SUCCEEDED(texture->QueryInterface(
        __uuidof(IDirect3DTexture9),
        reinterpret_cast<void**>(&tex2D))))
    {
        D3DSURFACE_DESC desc;
        tex2D->GetLevelDesc(0, &desc);
        tex2D->Release();

        srcRect = {0, 0, LONG(desc.Width), LONG(desc.Height)};
        center = {desc.Width * 0.5f, desc.Height * 0.5f, 0.f};
        pos = center;
    }
}

void UIRenderer::Render()
{
    auto sprite = EngineCore::GetInstance()
        ->GetRenderSystem()
        ->GetSpriteBatch();

    sprite->Draw(
        tex2D,
        &srcRect,
        &center,
        &pos,
        D3DCOLOR_ARGB(255, 255, 255, 255)
    );

}
