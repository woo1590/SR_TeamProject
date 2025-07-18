#pragma once

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
    void SetUIRenderState(UIRenderType newType);
    UIRenderType GetCurRenderState() const { return uiRenderState; }

    ID3DXSprite* GetSpriteBatch() const { return spriteBatch; }
    const _matrix& GetCachedViewMatrix() const { return cachedView; }
    const _matrix& GetCachedProjMatrix() const { return cachedProj; }

    void ClearSystem();

    // --------------------------------------------------------------------
    void SetMinimapCamera(CameraComponent* cam) { minimapCamera = cam; }
    void SetInventoryCamera(CameraComponent* cam) { inventoryCamera = cam; }
    LPDIRECT3DTEXTURE9 GetMinimapTexture() const { return minimapTexture; }
    LPDIRECT3DTEXTURE9 GetInventoryTexture() const { return inventoryTexture; }
    void RegisterInventoryRenderer(RendererComponent* renderer) { if (renderer) inventoryRenderList.push_back(renderer); }
    void ClearInventoryRenderers() { inventoryRenderList.clear(); }

private:
    void PriorityPass();
    void NonAlphaPass();
    void AlphaPass();
    void DebugPass();
    void Reset();   
// --------------------------------------
    void MinimapPass();
    void InventoryPass();
    void UIPass();
    // ------------------------------------
    void Free()override;

    std::vector<std::list<RendererComponent*>> RenderList;
    CameraComponent* Camera;
   
    
    std::list<CollisionComponent*> DebugRender; //����׿�

    _matrix CurrView;
    _matrix CurrProj;
    Shader* CurrShader = nullptr;

    LPDIRECT3DDEVICE9 Device;

    /*---------------------------------------------------------------*/
    ID3DXSprite* spriteBatch = nullptr;
    _matrix cachedView;
    _matrix cachedProj;
    UIRenderType uiRenderState = UIRenderType::None;
    CameraComponent* minimapCamera = nullptr;
    CameraComponent* inventoryCamera = nullptr;

    LPDIRECT3DTEXTURE9 minimapTexture;
    LPDIRECT3DSURFACE9 minimapSurface;
    LPDIRECT3DTEXTURE9 inventoryTexture;
    LPDIRECT3DSURFACE9 inventorySurface;
    list<RendererComponent*> inventoryRenderList;
};

END