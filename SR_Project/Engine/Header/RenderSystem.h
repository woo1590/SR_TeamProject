#pragma once

#include "RenderTargetView.h"

BEGIN(Engine)

class RendererComponent;
class CollisionComponent;
class Object;
class CameraComponent;
class UIRenderer;
class Shader;
class MeshRenderer;

class ENGINE_DLL RenderSystem : public Base
{
private:
    explicit RenderSystem();
    virtual ~RenderSystem();

public:
    static RenderSystem* Create();

    HRESULT Ready_RenderSystem();
    void Render_Begin(D3DXCOLOR color);
    void Render();
    void Render_End();

    void RegisterRenderer(RENDER_ID layer, RendererComponent* renderer);
    void RegisterCollision(CollisionComponent* collision);
    void SetCamera(Object* cam);
    void ClearSystem();

    void SetShader(const std::string& key);

    // --------------------------------------------------------------------
    void SetUIRenderState(UIRenderType newType);
    UIRenderType GetCurRenderState() const { return uiRenderState; }

    ID3DXSprite* GetSpriteBatch() const { return spriteBatch; }
    const _matrix& GetCachedViewMatrix() const { return cachedView; }
    const _matrix& GetCachedProjMatrix() const { return cachedProj; }

    void RegisterRTV(RenderTargetView* view) { if (view) rtvs.push_back(view); }
    void UnRegisterRTV(RenderTargetView* view) { if (view) rtvs.remove(view); }

    // ----------------------------------------------------------------------
private:
    HRESULT CreatePostProcessBuffer(D3DVIEWPORT9 vp);
    void PriorityPass();
    void NonAlphaPass();
    void AlphaPass();
    void PostProcessPass();
    void OverlayPass();
    void DebugPass();
    void Reset();   
// --------------------------------------
    void UIPass();
    void RenderOffScreenViews();
    // ------------------------------------
    void Free()override;

    std::vector<std::list<RendererComponent*>> RenderList;
    CameraComponent* Camera;
    std::list<CollisionComponent*> DebugRender; //����׿�
    _matrix CurrView;
    _matrix CurrProj;
    Shader* CurrShader = nullptr;

    LPDIRECT3DDEVICE9 Device;

    /*---------RenderTarget---------*/

    LPDIRECT3DTEXTURE9 targetTexture = nullptr;
    LPDIRECT3DSURFACE9 targetSurface = nullptr;
    LPDIRECT3DSURFACE9 originSurface = nullptr;

    LPDIRECT3DVERTEXBUFFER9 postProcessVB = nullptr;
    LPDIRECT3DINDEXBUFFER9 postProcessIB = nullptr;
    LPDIRECT3DVERTEXDECLARATION9 decl = nullptr;

    Shader* postProcessShader = nullptr;

    /*--------------------------------------------*/
    ID3DXSprite* spriteBatch = nullptr;
    _matrix cachedView;
    _matrix cachedProj;
    UIRenderType uiRenderState = UIRenderType::None;

    list<RenderTargetView*> rtvs;

    _int numPass = 0;
};

END