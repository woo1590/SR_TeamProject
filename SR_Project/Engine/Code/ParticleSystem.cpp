#include "EnginePCH.h"
#include "ParticleSystem.h"
#include "EngineCore.h"
#include "Random.h"

ParticleSystem::ParticleSystem(Object* owner)
	:ObjectComponent(owner)
{
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::Update(_float dt)
{
	if (emitter.alive)
		emitter.Spawn(particles, dt);

	for (auto& p : particles)
	{
		if (!p.isActive) continue;

		//Life End
		p.age += dt;	
		if (p.age >= p.life)
		{
			p.isActive = false;
			continue;
		}

		particleUpdate(p, dt);
	}
}

void ParticleSystem::AddEmitter(Emitter emitter, UpdateFunc update)
{
	this->emitter = emitter;
	particleUpdate = update;
}

void ParticleSystem::Free()
{

}

ParticleSystem* ParticleSystem::Create(Object* owner)
{
	ParticleSystem* Instance = new ParticleSystem(owner);
	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ParticleSystem::Ready_Component()
{
	particles.resize(MAX_PARTICLES);

	return S_OK;
}
