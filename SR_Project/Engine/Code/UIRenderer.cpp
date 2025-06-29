#include "EnginePCH.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "TransformComponent.h"
#include "Object.h"

UIRenderer::UIRenderer(Object* owner)
	:RendererComponent(owner,RENDER_ID::Render_UI){}

UIRenderer* UIRenderer::Create(Object* owner)
{
	auto* instance = new UIRenderer(owner);
	
    if (FAILED(instance->Ready_Component()))
        return Safe_Release(instance), nullptr;

	EngineCore::GetInstance()->GetRenderSystem()->RegisterRenderer(RENDER_ID::Render_UI, instance);
	return instance;
}

void UIRenderer::SetTexture(const wstring& key)
{
    auto* rm = EngineCore::GetInstance()->GetResourceManager();
    texture = rm->GetTexture(key);

    if (auto layer = rm->GetUILayer(key); layer.has_value())
        SetLayer(layer.value());

    Safe_Release(tex2D);
    if (!texture) return;

    texture->QueryInterface(__uuidof(IDirect3DTexture9), reinterpret_cast<void**>(&tex2D));

    D3DSURFACE_DESC desc;
    tex2D->GetLevelDesc(0, &desc);
   
    fullWidth  = static_cast<LONG>(desc.Width);
    fullHeight = static_cast<LONG>(desc.Height);

    srcRect = {0, 0, fullWidth, fullHeight};
    center = {fullWidth * 0.5f, static_cast<float>(fullHeight),0.f};
    pos = center;
}

void UIRenderer::ApplyRatio(float _ratio)
{
    LONG visible = static_cast<LONG>(fullHeight * _ratio);

    srcRect.top = fullHeight - visible;
    srcRect.bottom = fullHeight;

    center.x = fullWidth * 0.5f;
    center.y = static_cast<float>(visible);
}

void UIRenderer::Render()
{
    auto sprite = EngineCore::GetInstance()->GetRenderSystem()->GetSpriteBatch();
    auto transform = owner->GetComponent<TransformComponent>();

    _vec3 worldPos = transform->GetPosition();

    sprite->Draw(tex2D,&srcRect,&center,&worldPos,
        D3DCOLOR_ARGB(255, 255, 255, 255));
}