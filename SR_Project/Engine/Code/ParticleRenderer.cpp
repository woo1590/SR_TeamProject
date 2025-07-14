#include "EnginePCH.h"
#include "ParticleRenderer.h"
#include "EngineCore.h"
#include "Scene.h"
#include "CameraManager.h"
#include "ResourceManager.h"
#include "Object.h"
#include "Material.h"
#include "Shader.h"

//component
#include "CameraComponent.h"
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
		{ 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT,
										D3DDECLUSAGE_COLOR,   0 },
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
	particleCnt = 0;
	VB->Lock(0, 0, (void**)&verts, 0);
	for (const auto& p : particles)
	{
		if (!p.isActive)
			continue;

		verts[particleCnt].position = p.position;
		verts[particleCnt].size = p.size;
		verts[particleCnt].color = p.color;

		++particleCnt;
	}
	VB->Unlock();
}

void ParticleRenderer::Render()
{
	auto shader = mtrl->GetShader();
	auto cam = owner->GetScene()->GetCameraManager()->GetMainCamera();

	if (!cam)
		return;

	_matrix view = cam->GetViewMatrix();
	_matrix proj = cam->GetProjMatrix();

	shader->Begin(0);

	shader->SetConstant("g_View", view);
	shader->SetConstant("g_Proj", proj);

	mtrl->Apply();
	Device->SetStreamSource(0, VB, 0, sizeof(VTXPOINT));
	Device->SetVertexDeclaration(Decl);

	Device->DrawPrimitive(D3DPT_POINTLIST, 0, particleCnt);

	shader->End();

}

void ParticleRenderer::SetMaterial(const std::string& key)
{
	mtrl = EngineCore::GetInstance()->GetResourceManager()->GetMaterial(key);
}
