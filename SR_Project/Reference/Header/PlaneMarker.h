#pragma once
#include "RendererComponent.h"

BEGIN(Engine)

class ENGINE_DLL PlaneMarker :
    public RendererComponent
{
private:
    PlaneMarker(Object* owner, RENDER_ID renderid);
    virtual ~PlaneMarker();
public:
    static PlaneMarker* Create(Object* owner, RENDER_ID renderID);
    void Render()override;

    _bool IsShadow()const;
    _bool IsMirror()const;
    void SetStencilRef(_ulong ref);
    void SetPlane(D3DXPLANE plane);
    void SetNormal(_vec3 normal);
    D3DXPLANE GetWorldPlane()const;
private:
    D3DXPLANE Plane;
    _vec3 Normal;
    _ulong Ref = 0;
};

END