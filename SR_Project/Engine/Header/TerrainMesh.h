#pragma once
#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL TerrainMesh :
    public Mesh
{
private:
    TerrainMesh();
    virtual ~TerrainMesh();

public:
    static TerrainMesh* Create();
    HRESULT Ready_TerrainMesh();
    void Draw();

    HRESULT LoadTerrain(const std::wstring& filePath, _float cellSpacing, _float heightScale);
    _float GetHeight(_float x, _float z);
private:
    HRESULT CreateBuffer();
    _float GetHeightmapEntry(_float row, _float col);
    void Free()override;

    _uint VertexCol = 0;
    _uint VertexRow = 0;
    
    _uint CellCol = 0;
    _uint CellRow = 0;

    _uint VertexCnt = 0;
    _uint CellCnt = 0;

    _float CellSpacing = 0.f;
    _float HeightScale = 0.f;
    _float Width = 0.f;
    _float Depth = 0.f;

    std::vector<_ulong> HeightMap;

    LPDIRECT3DVERTEXBUFFER9 VB = nullptr;
    LPDIRECT3DINDEXBUFFER9 IB = nullptr;
};

END