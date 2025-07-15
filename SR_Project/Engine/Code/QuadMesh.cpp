#include "EnginePCH.h"
#include "QuadMesh.h"

QuadMesh::QuadMesh(_float size):size(size)
{
}

QuadMesh::~QuadMesh()
{
}

QuadMesh* QuadMesh::Create(_float size)
{
	QuadMesh* Instance = new QuadMesh(size);
	if (FAILED(Instance->Ready_Mesh()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT QuadMesh::Ready_Mesh()
{
	std::vector<VTXTEX> vertices;
	std::vector<INDEX32> indices;

	vertices.push_back({ { -size * 0.5f, -size * 0.5f,0.f },{0.f,0.f,0.f},{0.f,1.f} });
	vertices.push_back({ { -size * 0.5f, size * 0.5f,0.f },{0.f,0.f,0.f},{0.f,0.f} });
	vertices.push_back({ { size * 0.5f, size * 0.5f,0.f },{0.f,0.f,0.f},{1.f,0.f} });
	vertices.push_back({ { size * 0.5f, -size * 0.5f,0.f },{0.f,0.f,0.f},{1.f,1.f} });

	indices.push_back({ 0,1,2 });
	indices.push_back({ 0,2,3 });

	/*----------Vertex Decl-----------------------------*/
	static const D3DVERTEXELEMENT9 kDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_NORMAL,   0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_TEXCOORD, 0},
		D3DDECL_END()
	};

	if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
		return E_FAIL;
	/*--------------------------------------------------*/

	Device->CreateVertexBuffer(vertices.size() * sizeof(VTXTEX), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &VB, nullptr);
	VTXTEX* verts = nullptr;
	VB->Lock(0, 0, (void**)&verts, 0);
	memcpy_s(verts, vertices.size() * sizeof(VTXTEX), &vertices[0], vertices.size() * sizeof(VTXTEX));
	VB->Unlock();

	INDEX32* inds = nullptr;
	Device->CreateIndexBuffer(indices.size() * sizeof(INDEX32), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &IB, 0);
	IB->Lock(0, 0, (void**)&inds, 0);
	memcpy_s(inds, indices.size() * sizeof(INDEX32), &indices[0], indices.size() * sizeof(INDEX32));
	IB->Unlock();

	return S_OK;
}

void QuadMesh::Draw()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXTEX));
	Device->SetIndices(IB);
	Device->SetVertexDeclaration(Decl);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void QuadMesh::Free()
{
	Safe_Release(VB);
	Safe_Release(IB);
	Safe_Release(Decl);
	Mesh::Free();
}
