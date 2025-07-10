#include "EnginePCH.h"
#include "UIRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "RenderSystem.h"
#include "TransformComponent.h"
#include "FontComponent.h"
#include "Object.h"

UIRenderType UIRenderer::curRenderType = UIRenderType::MainGame;

UIRenderer* UIRenderer::Create(Object* owner)
{
    auto* instance = new UIRenderer(owner);
	
    if (FAILED(instance->Ready_Component()))
        return Safe_Release(instance), nullptr;

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

void UIRenderer::SetSrcRect(const RECT& rect)
{
    srcRect = rect;
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

void UIRenderer::Free()
{
    Safe_Release(tex2D);
    Safe_Release(texture);
}

void UIRenderer::Render()
{
    if (!isVisible) return;

    auto sprite = EngineCore::GetInstance()->GetRenderSystem()->GetSpriteBatch();
    auto transform = owner->GetComponent<TransformComponent>();
    assert(transform);

    _vec3 worldPos = transform->GetWorldPosition();
    _vec2 scale2D = transform->GetScale2D();
    if (scale.x != 1.f || scale.y != 1.f)
        scale2D = scale;

    _matrix oldMatrix, newMatrix;
    sprite->GetTransform(&oldMatrix);

    _vec2 anchor = {worldPos.x, worldPos.y};
    _vec2 pivot2D = {center.x, center.y};

    D3DXMatrixTransformation2D(&newMatrix, &anchor, 0.f, &scale2D, &pivot2D, 0.f, nullptr);

    sprite->SetTransform(&newMatrix);
    int a = static_cast<int>(alpha * 255.f);
    sprite->Draw(tex2D, &srcRect, &center, &worldPos, D3DCOLOR_ARGB(a, 255, 255, 255));
    sprite->SetTransform(&oldMatrix);
}

void UIRenderer::RenderFont()
{
    if (auto font = owner->GetComponent<FontComponent>())
        font->Render();
}