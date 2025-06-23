#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class DxMesh;
class ENGINE_DLL DxMeshRenderer :
    public RendererComponent
{
private:
    DxMeshRenderer(Object* owner, RENDER_ID renderId);
    virtual ~DxMeshRenderer();

public:
    static DxMeshRenderer* Create(Object* owner, RENDER_ID renderId);
    void Render()override;

    void SetMesh(DxMesh* mesh);

private:
    void Free()override;

    DxMesh* mesh = nullptr;
};

END