#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL MeshComponent :
    public ObjectComponent
{
private:
    explicit MeshComponent(Object* owner);
    virtual ~MeshComponent();

public:
    static MeshComponent* Create(Object* owner);
    
    void SetMesh(const std::vector<VTXNOR>& vertices, const std::vector<INDEX16>& indices);

    void Draw();
private:
    void Free()override;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    std::vector<VTXNOR> Vertices;
    std::vector<INDEX16> Indices;

    LPDIRECT3DDEVICE9 Device = nullptr;
};

END
