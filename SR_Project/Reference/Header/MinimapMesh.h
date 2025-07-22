#pragma once
#include "Mesh.h"

BEGIN(Engine)
class ENGINE_DLL MinimapMesh : public Mesh
{
private:
    MinimapMesh();
    virtual ~MinimapMesh();

public:
    static MinimapMesh* Create();
    HRESULT Ready_Mesh();
    HRESULT CreateBuffer();
    void Draw() override;

    void AddTile(_vec3 vPos, _ulong dwColor);

private:
    void Free() override;

    _ulong VertexCnt = 0;
    _ulong IndexCnt = 0;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;

    std::vector<VTXCOL> Vertices;
    std::vector<INDEX32> Indices;
};
END