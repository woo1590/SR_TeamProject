#include "EnginePCH.h"
#include "OverlayRenderer.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "Shader.h"

OverlayRenderer::OverlayRenderer(Object* owner, RENDER_ID id)
	:RendererComponent(owner, id)
{
}

OverlayRenderer::~OverlayRenderer()
{
}

OverlayRenderer* OverlayRenderer::Create(Object* owner, RENDER_ID id)
{
	OverlayRenderer* Instance = new OverlayRenderer(owner, id);
	
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT OverlayRenderer::Ready_Component()
{
	std::vector<VTXOVER> vertices;
	std::vector<INDEX32> indices;

	vertices.push_back({ {0.f,(_float)WINCY+50,0.f,1.f },{0.f,0.f,0.f,1.f} });
	vertices.push_back({ {0.f,0.f,0.f,1.f },{0.f,0.f,0.f,1.f} });
	vertices.push_back({ {(_float)WINCX+50,0.f,0.f,1.f},{0.f,0.f,0.f,1.f} });
	vertices.push_back({ {(_float)WINCX+50,(_float)WINCY+50,0.f,1.f},{0.f,0.f,0.f,1.f} });

	indices.push_back({ 0,1,2 });
	indices.push_back({ 0,2,3 });

	/*----------Vertex Decl-----------------------------*/
	static const D3DVERTEXELEMENT9 kDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_POSITIONT, 0 },
		{ 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_COLOR,   0 },
		D3DDECL_END()
	};

	if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
		return E_FAIL;
	/*--------------------------------------------------*/

	Device->CreateVertexBuffer(vertices.size() * sizeof(VTXOVER), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &VB, nullptr);
	VTXOVER* verts = nullptr;
	VB->Lock(0, 0, (void**)&verts, 0);
	memcpy_s(verts, vertices.size() * sizeof(VTXOVER), &vertices[0], vertices.size() * sizeof(VTXOVER));
	VB->Unlock();

	INDEX32* inds = nullptr;
	Device->CreateIndexBuffer(indices.size() * sizeof(INDEX32), D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_MANAGED, &IB, 0);
	IB->Lock(0, 0, (void**)&inds, 0);
	memcpy_s(inds, indices.size() * sizeof(INDEX32), &indices[0], indices.size() * sizeof(INDEX32));
	IB->Unlock();

	return S_OK;
}

void OverlayRenderer::Late_Update(_float dt)
{
	RendererComponent::Late_Update(dt);
}

void OverlayRenderer::Render()
{
	shader->Begin(0);

	shader->SetConstant("alpha", alpha);
	Device->SetStreamSource(0, VB, 0, sizeof(VTXOVER));
	Device->SetVertexDeclaration(Decl);
	Device->SetIndices(IB);

	Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	shader->End();
}

void OverlayRenderer::SetShader(const std::string& key)
{
	shader = EngineCore::GetInstance()->GetResourceManager()->GetShader(key);
}

void OverlayRenderer::Free()
{
	ObjectComponent::Free();
}
