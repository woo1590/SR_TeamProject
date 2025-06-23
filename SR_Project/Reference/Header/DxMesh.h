#pragma once
#include "Mesh.h"
#include "Engine_Define.h"

BEGIN(Engine)

class ENGINE_DLL DxMesh :
    public Mesh
{
private:
    DxMesh();
    virtual ~DxMesh();

public:
    static DxMesh* Create();
    HRESULT Ready_DxMesh();

    HRESULT LoadFromX(const std::wstring& filePath);
    void Draw();

private:
    LPD3DXMESH mesh;
    std::vector<D3DMATERIAL9> Mtrls;
    std::vector<IDirect3DTexture9*> Textures;
    _ulong SubsetNum = 0;
};

END