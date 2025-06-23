#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL RendererComponent :
    public ObjectComponent
{
protected:
    RendererComponent(Object* owner, RENDER_ID id);
    virtual ~RendererComponent();

public:
    //static RendererComponent * Create(Object* owner, RENDER_ID id);

    virtual void Late_Update(float dt)override;
    virtual void Render()PURE;

    void SetRenderID(RENDER_ID id);
protected:
    void Free()override;

    RENDER_ID Id;
    LPDIRECT3DDEVICE9 Device = nullptr;
};

END