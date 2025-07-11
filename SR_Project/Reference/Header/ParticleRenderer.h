#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL ParticleRenderer :
    public RendererComponent
{
private:
    ParticleRenderer(Object* owner, RENDER_ID id);
    virtual ~ParticleRenderer();

public:
    static ParticleRenderer* Create(Object* owner, RENDER_ID id);
    void Render()override;
};

END