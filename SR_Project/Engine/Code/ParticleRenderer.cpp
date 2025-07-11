#include "EnginePCH.h"
#include "ParticleRenderer.h"

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

void ParticleRenderer::Render()
{

}
