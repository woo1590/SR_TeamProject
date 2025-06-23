#pragma once
#include "Base.h"
#include "Engine_Define.h"

BEGIN(Engine)

class RendererComponent;
class CameraComponent;
class ENGINE_DLL RenderSystem :
    public Base
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
    void SetCamera(CameraComponent* cam);
private:
    void PriorityPass();
    void OpaquePass();
    void Reset();

    void Free()override;

    std::vector<std::list<RendererComponent*>> RenderList;
    CameraComponent* Camera;
    LPDIRECT3DDEVICE9 Device;
};

END