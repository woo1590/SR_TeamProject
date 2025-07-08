#include "EnginePCH.h"
#include "BlockMesh.h"

BlockMesh::BlockMesh()
{
}

BlockMesh::~BlockMesh()
{
}

BlockMesh* BlockMesh::Create()
{
    BlockMesh* instance = new BlockMesh();

    if (FAILED(instance->Ready_Mesh()))
    {
        Safe_Release(instance);

        instance = nullptr;
    }

    return instance;
}

HRESULT BlockMesh::Ready_Mesh()
{
    Vertices.push_back(VTXCUBE({ -1.f,-1.f,-1.f }));
    Vertices.push_back(VTXCUBE({ -1.f,1.f,-1.f }));
    Vertices.push_back(VTXCUBE({ 1.f,1.f,-1.f }));
    Vertices.push_back(VTXCUBE({ 1.f,-1.f,-1.f }));

    Vertices.push_back(VTXCUBE({ -1.f,-1.f,1.f }));
    Vertices.push_back(VTXCUBE({ -1.f,1.f,1.f }));
    Vertices.push_back(VTXCUBE({ 1.f,1.f,1.f }));
    Vertices.push_back(VTXCUBE({ 1.f,-1.f,1.f }));
  
    CreateBuffer();
    return S_OK;
}

void BlockMesh::BuildIBEntry()
{
	_ulong faceIdx[6][6] =
	{
		{3,2,6,3,6,7,},{4,5,1,4,1,0},//x+, x-
		{1,5,6,1,6,2},{4,0,3,4,3,7},//y+, y-
		{5,4,7,5,7,6},{0,1,2,0,2,3}//z+, z-
	};

    for (_uint mask = 0; mask < 64; ++mask)
    {
        std::vector<UINT32> idx;
        for (_uint j = 0; j < 6; ++j)
        {
            if (mask & (1 << j))
            {
                idx.insert(idx.end(), std::begin(faceIdx[j]), std::end(faceIdx[j]));

                IBTable[mask].tri = idx.size() / 3;

                Device->CreateIndexBuffer(idx.size() * sizeof(_ulong),
                                          D3DUSAGE_WRITEONLY,
                                          D3DFMT_INDEX32,
                                          D3DPOOL_MANAGED,
                                          &IBTable[mask].ib, nullptr);


                void* p;
                IBTable[mask].ib->Lock(0, 0, &p, 0);
                memcpy(p, idx.data(), idx.size() * sizeof(_ulong));
                IBTable[mask].ib->Unlock();
            }
        }
    }
}

HRESULT BlockMesh::CreateBuffer()
{
    VertexCnt = Vertices.size();

    /*----------Vertex Decl-----------------------------*/
    static const D3DVERTEXELEMENT9 kDecl[] =
    {
        { 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                                        D3DDECLUSAGE_POSITION, 0 },
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
                                        D3DDECLUSAGE_NORMAL,   0 },
        { 0, 24,  D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
                                        D3DDECLUSAGE_TEXCOORD, 0 },
        D3DDECL_END()
    };

    if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
        return E_FAIL;
    /*--------------------------------------------------*/

    if (FAILED(Device->CreateVertexBuffer(VertexCnt * sizeof(VTXTEX),
        D3DUSAGE_WRITEONLY,
        0,
        D3DPOOL_MANAGED,
        &VB, nullptr)))
        return E_FAIL;

    VTXCUBE* verts = nullptr;
    if (FAILED(VB->Lock(0, 0, (void**) &verts, 0)))
        return E_FAIL;

    memcpy_s(verts, VertexCnt * sizeof(VTXTEX), Vertices.data(), VertexCnt * sizeof(VTXTEX));
    VB->Unlock();

    BuildIBEntry();

    Mask = 0;

    return S_OK;
}

void BlockMesh::Draw()
{
    if (!Mask) return;

    Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
    Device->SetIndices(IBTable[Mask].ib);
    Device->SetVertexDeclaration(Decl);

    Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IBTable[Mask].tri);
}

void BlockMesh::Free()
{
    Safe_Release(VB);
    Safe_Release(Decl);
    std::for_each(IBTable.begin(), IBTable.end(), [](IBEntry& e) {Safe_Release(e.ib);});
    Mesh::Free();
}