#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class Shader;
class ENGINE_DLL OverlayRenderer :
    public RendererComponent
{
private:
    OverlayRenderer(Object* owner, RENDER_ID id);
    virtual ~OverlayRenderer();

public:
    static OverlayRenderer* Create(Object* owner, RENDER_ID id);
    HRESULT Ready_Component()override;

    void Late_Update(_float dt)override;
    void Render()override;

    void SetShader(const std::string& key);
    Shader* GetShader()const { return shader; }
    void SetAlpha(_float alpha) { this->alpha = alpha; }
private:
    void Free()override;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;
    LPDIRECT3DVERTEXDECLARATION9 Decl = nullptr;

    Shader* shader = nullptr;

    _float alpha = 1.f;
};

END