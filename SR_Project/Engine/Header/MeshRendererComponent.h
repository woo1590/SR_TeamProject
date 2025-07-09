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

    void SetMesh(const std::string & key);
    void SetMesh(Mesh* mesh);
    void SetMaterial(const std::string & key);
    Material* GetMaterial();

private:
    void Free()override;

    Mesh* mesh = nullptr;
    Material* mtrl = nullptr;
};

END