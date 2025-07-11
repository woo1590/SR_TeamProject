#pragma once

BEGIN(Engine)

class RendererComponent;
class CollisionComponent;
class Object;
class CameraComponent;
class UIRenderer;
class Shader;
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

    ID3DXSprite* GetSpriteBatch() const { return spriteBatch; }
    const _matrix& GetCachedViewMatrix() const { return cachedView; }
    const _matrix& GetCachedProjMatrix() const { return cachedProj; }

private:
    void PriorityPass();
    void NonAlphaPass();
    void AlphaPass();
    void UIPass();
    void DebugPass();
    void Reset();   

    void Free()override;

    std::vector<std::list<RendererComponent*>> RenderList;
    CameraComponent* Camera;
   
    ID3DXSprite* spriteBatch = nullptr;
    std::list<CollisionComponent*> DebugRender; //����׿�

    _matrix CurrView;
    _matrix CurrProj;
    Shader* CurrShader = nullptr;

    LPDIRECT3DDEVICE9 Device;

    _matrix cachedView;
    _matrix cachedProj;
};

END