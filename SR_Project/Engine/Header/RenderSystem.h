#pragma once

BEGIN(Engine)

class RendererComponent;
class CollisionComponent;
class Object;
class CameraComponent;
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

    ID3DXSprite* GetSpriteBatch() const { return spriteBatch; }

private:
    void PriorityPass();
    void NonAlphaPass();
    void AlphaPass();
    void UIPass();
    void Reset();

    void Free()override;

    std::vector<std::list<RendererComponent*>> RenderList;
    CameraComponent* Camera;
   
    LPDIRECT3DDEVICE9 Device;
    ID3DXSprite* spriteBatch = nullptr;

    std::list<CollisionComponent*> DebugRender; //����׿�
};

END