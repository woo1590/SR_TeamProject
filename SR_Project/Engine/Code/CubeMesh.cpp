#include "EnginePCH.h"
#include "CubeMesh.h"

CubeMesh::CubeMesh()
{
}

CubeMesh::~CubeMesh()
{
}

CubeMesh* CubeMesh::Create()
{
	CubeMesh* Instance = new CubeMesh;

	if (FAILED(Instance->Ready_Mesh()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT CubeMesh::Ready_Mesh()
{
	Vertices.push_back(VTXCUBE({ -1.f,-1.f,-1.f }));
	Vertices.push_back(VTXCUBE({ -1.f,1.f,-1.f }));
	Vertices.push_back(VTXCUBE({ 1.f,1.f,-1.f }));
	Vertices.push_back(VTXCUBE({ 1.f,-1.f,-1.f }));

	Vertices.push_back(VTXCUBE({ -1.f,-1.f,1.f }));
	Vertices.push_back(VTXCUBE({ -1.f,1.f,1.f }));
	Vertices.push_back(VTXCUBE({ 1.f,1.f,1.f }));
	Vertices.push_back(VTXCUBE({ 1.f,-1.f,1.f }));

	//¾Õ¸é
	Indices.push_back(INDEX32(0, 1, 2));
	Indices.push_back(INDEX32(0, 2, 3));

	//µÞ¸é
	Indices.push_back(INDEX32(5, 4, 7));
	Indices.push_back(INDEX32(5, 7, 6));

	//¿ÞÂÊ
	Indices.push_back(INDEX32(4, 5, 1));
	Indices.push_back(INDEX32(4, 1, 0));

	//¿À¸¥ÂÊ
	Indices.push_back(INDEX32(3, 2, 6));
	Indices.push_back(INDEX32(3, 6, 7));

	//À­¸é
	Indices.push_back(INDEX32(1, 5, 6));
	Indices.push_back(INDEX32(1, 6, 2));

	//¾Æ·¡¸é
	Indices.push_back(INDEX32(4, 0, 3));
	Indices.push_back(INDEX32(4, 3, 7));

	if (FAILED(CreateBuffer()))
		return E_FAIL;

	return S_OK;
}

HRESULT CubeMesh::CreateBuffer()
{
	VertexCnt = Vertices.size();
	IndexCnt = Indices.size() * 3;

	for (auto& vertex : Vertices)
	{
		vertex.vTexUV = vertex.vPosition;
	}

	//Vertex Buffer
	Device->CreateVertexBuffer(VertexCnt * sizeof(VTXCUBE),
		D3DUSAGE_WRITEONLY,
		FVF_CUBE,
		D3DPOOL_MANAGED,
		&VB, 0);

	VTXCUBE* verts = nullptr;
	VB->Lock(0, 0, (void**)&verts, 0);
	memcpy_s(verts, VertexCnt * sizeof(VTXCUBE), &Vertices[0], VertexCnt * sizeof(VTXCUBE));
	VB->Unlock();

	//Index Buffer
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

void CubeMesh::Draw()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXCUBE));
	Device->SetIndices(IB);
	Device->SetFVF(FVF_CUBE);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, VertexCnt, 0, IndexCnt / 3);
}

void CubeMesh::Free()
{
	Safe_Release(VB);
	Safe_Release(IB);
	Mesh::Free();
}
