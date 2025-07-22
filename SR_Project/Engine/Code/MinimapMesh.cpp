#include "EnginePCH.h"
#include "MinimapMesh.h"

MinimapMesh::MinimapMesh()
{
}

MinimapMesh::~MinimapMesh()
{
}

MinimapMesh* MinimapMesh::Create()
{
    MinimapMesh* Instance = new MinimapMesh;

    if (FAILED(Instance->Ready_Mesh()))
    {
        Safe_Release(Instance);
        Instance = nullptr;
    }

    return Instance;
}

HRESULT MinimapMesh::Ready_Mesh()
{
    return S_OK;
}

void MinimapMesh::AddTile(_vec3 vPos, _ulong dwColor)
{
    _float halfSize = 1.f;

    _vec3 tl = { vPos.x - halfSize, vPos.y + halfSize, vPos.z };
    _vec3 tr = { vPos.x + halfSize, vPos.y + halfSize, vPos.z };
    _vec3 bl = { vPos.x - halfSize, vPos.y - halfSize, vPos.z };
    _vec3 br = { vPos.x + halfSize, vPos.y - halfSize, vPos.z };

    _ulong baseIdx = Vertices.size();

    Vertices.push_back({ tl, dwColor });
    Vertices.push_back({ tr, dwColor });
    Vertices.push_back({ br, dwColor });
    Vertices.push_back({ bl, dwColor });

    Indices.push_back(INDEX32(baseIdx + 0, baseIdx + 1, baseIdx + 2));
    Indices.push_back(INDEX32(baseIdx + 0, baseIdx + 2, baseIdx + 3));
}

HRESULT MinimapMesh::CreateBuffer()
{
    VertexCnt = Vertices.size();
    IndexCnt = Indices.size() * 3;

    Device->CreateVertexBuffer(VertexCnt * sizeof(VTXCOL),
        D3DUSAGE_WRITEONLY,
        D3DFVF_XYZ | D3DFVF_DIFFUSE,
        D3DPOOL_MANAGED,
        &VB, 0);

    VTXCOL* verts = nullptr;
    VB->Lock(0, 0, (void**)&verts, 0);
    memcpy_s(verts, VertexCnt * sizeof(VTXCOL), &Vertices[0], VertexCnt * sizeof(VTXCOL));
    VB->Unlock();

    Device->CreateIndexBuffer(IndexCnt * sizeof(_ulong),
        D3DUSAGE_WRITEONLY,
        D3DFMT_INDEX32,
        D3DPOOL_MANAGED,
        &IB, 0);

    INDEX32* index = nullptr;
    IB->Lock(0, 0, (void**)&index, 0);
    memcpy_s(index, IndexCnt * sizeof(_ulong), &Indices[0], IndexCnt * sizeof(_ulong));
    IB->Unlock();

    return S_OK;
}

void MinimapMesh::Draw()
{
    if (!isRender) return;

    Device->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);
    Device->SetStreamSource(0, VB, 0, sizeof(VTXCOL));
    Device->SetIndices(IB);

    Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void MinimapMesh::Free()
{
    Safe_Release(VB);
    Safe_Release(IB);
    Mesh::Free();
}