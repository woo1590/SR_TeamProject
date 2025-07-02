#include "EnginePCH.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "TransformComponent.h"
#include "FontComponent.h"
#include "Object.h"

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

    assert(texture && "UIRenderer::SetTexture - texture not found");

    if (auto layer = rm->GetUILayer(key); layer.has_value())
        SetLayer(layer.value());

    Safe_Release(tex2D);
    
    texture->QueryInterface(__uuidof(IDirect3DTexture9), reinterpret_cast<void**>(&tex2D));
   
    assert(tex2D && "QueryInterface failed: not a 2D texture");

    D3DSURFACE_DESC desc;
    tex2D->GetLevelDesc(0, &desc);
   
    fullWidth  = static_cast<LONG>(desc.Width);
    fullHeight = static_cast<LONG>(desc.Height);

    srcRect = {0, 0, fullWidth, fullHeight};
    
    UpdateCenter();
}

void UIRenderer::ApplyRatioVertical(float _ratio)
{
    LONG visible = static_cast<LONG>(fullHeight * _ratio);
    srcRect.top = fullHeight - visible;
    srcRect.bottom = fullHeight;

    UpdateCenter();
}

void UIRenderer::ApplyRatioHorizontal(float _ratio)
{
    LONG visible = static_cast<LONG>(fullWidth * _ratio);
    srcRect.left = 0;
    srcRect.right = visible;

    UpdateCenter();
}


void UIRenderer::SetPivot(UIPivot _pivot)
{
    pivot = _pivot;
    UpdateCenter();
}

void UIRenderer::UpdateCenter()
{
    const float width = static_cast<float>(srcRect.right - srcRect.left);
    const float height = static_cast<float>(srcRect.bottom - srcRect.top);

    switch (pivot)
    {
    case UIPivot::Center:  center = {width * 0.5f, height * 0.5f, 0.f}; break;
    case UIPivot::Bottom:  center = {width * 0.5f, height, 0.f};        break;
    case UIPivot::LeftTop: center = {0.f, 0.f, 0.f};                    break;
    case UIPivot::Left:    center = {0.f, height * 0.5f, 0.f};          break;
    case UIPivot::Right:   center = {width, height * 0.5f, 0.f};        break;
    }
}

void UIRenderer::Render()
{
    if (tex2D)
    {
        auto sprite = EngineCore::GetInstance()->GetRenderSystem()->GetSpriteBatch();
        assert(sprite && "UIRenderer::Render - sprite is null");

        auto transform = owner->GetComponent<TransformComponent>();
        assert(transform && "UIRenderer::Render - TransformComponent missing");

        assert(scale.x > 0.f && scale.y > 0.f && "UIRenderer::Render - Invalid scale");

        _vec3 worldPos = transform->GetPosition();

        _matrix oldMatrix, newMatrix;
        sprite->GetTransform(&oldMatrix);

        _vec2 anchor = {worldPos.x, worldPos.y};
        D3DXMatrixTransformation2D(&newMatrix, &anchor, 0.f, &scale, nullptr, 0.f, nullptr);

        sprite->SetTransform(&newMatrix);
        sprite->Draw(tex2D, &srcRect, &center, &worldPos, D3DCOLOR_ARGB(255, 255, 255, 255));
        sprite->SetTransform(&oldMatrix);
    }

    auto font = owner->GetComponent<FontComponent>();

    if (font)
        font->Render(); 
}