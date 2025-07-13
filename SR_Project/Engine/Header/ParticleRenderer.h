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
    HRESULT Ready_Component()override;
    void Late_Update(_float dt);
    void Render()override;

private:
    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DVERTEXDECLARATION9 Decl = nullptr;
    _uint particleCnt = 0;

};

END