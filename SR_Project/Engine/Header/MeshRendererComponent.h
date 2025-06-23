#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class Mesh;
class Material;
class ENGINE_DLL MeshRenderer :
    public RendererComponent
{
private:
    MeshRenderer(Object* owner, RENDER_ID renderId);
    virtual ~MeshRenderer();
public:
    static MeshRenderer* Create(Object* owner, RENDER_ID renderId);
    void Late_Update(_float dt)override;
    void Render()override;
    void RenderShadow(_matrix shadowMat);
    void RenderReflect(_matrix reflectMat);

    void SetMesh(const std::wstring& key);
    void SetMaterial(const std::wstring& key);

    void SetShadowCast(_bool shadow);
    _bool IsShadowCast()const;

    void SetReflectCast(_bool reflect);
    _bool IsReflectCast()const;
private:
    void Free()override;

    Mesh* mesh = nullptr;
    Material* mtrl = nullptr;

    _bool ShadowCast = false;
    _bool ReflectCast = false;  
};

END