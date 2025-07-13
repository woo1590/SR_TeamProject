#include "EnginePCH.h"
#include "ParticleRenderer.h"
#include "EngineCore.h"
#include "Object.h"

//component
#include "ParticleSystem.h"

ParticleRenderer::ParticleRenderer(Object* owner, RENDER_ID id)
	:RendererComponent(owner,id)
{
}

ParticleRenderer::~ParticleRenderer()
{
}

ParticleRenderer* ParticleRenderer::Create(Object* owner, RENDER_ID id)
{
	ParticleRenderer* Instance = new ParticleRenderer(owner, id);
	
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ParticleRenderer::Ready_Component()
{
	/*----------Vertex Decl-----------------------------*/
	static const D3DVERTEXELEMENT9 kDecl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT1, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_PSIZE,   0 },
		D3DDECL_END()
	};

	if (FAILED(Device->CreateVertexDeclaration(kDecl, &Decl)))
		return E_FAIL;
	/*--------------------------------------------------*/

	Device->CreateVertexBuffer(2048 * sizeof(VTXPOINT),
							   D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
							   0,
							   D3DPOOL_DEFAULT,
							   &VB, nullptr);

	return S_OK;
}

void ParticleRenderer::Late_Update(_float dt)
{
	RendererComponent::Late_Update(dt);
	
	auto particles = owner->GetComponent<ParticleSystem>()->GetParticles();
	VTXPOINT* verts = nullptr;

	VB->Lock(0, 0, (void**)&verts, 0);
	for (const auto& p : particles)
	{
		if (!p.isActive)
			continue;

		verts[particleCnt].position = p.position;
		verts[particleCnt].size = p.size;
		verts[particleCnt].color = p.color;
	}
}

void ParticleRenderer::Render()
{
	Device->SetStreamSource(0, VB, 0, sizeof(VTXPOINT));
	Device->SetVertexDeclaration(Decl);
	Device->DrawPrimitive(D3DPT_POINTLIST, 0, particleCnt);
}
