#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL DxMeshComponent :
    public ObjectComponent
{
private:
    explicit DxMeshComponent(Object* owner);
    virtual ~DxMeshComponent();

public:
    static DxMeshComponent* Create(Object* owner);

    _ulong GetNumSubset()const;
    HRESULT LoadMesh(const WCHAR* filePath);
    void DrawSubset(_ulong subset);

private:
    void Free()override;

    ID3DXMesh* Mesh = nullptr;
    _ulong NumSubset = 0;
};

END