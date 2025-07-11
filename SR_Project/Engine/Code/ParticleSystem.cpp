#include "EnginePCH.h"
#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Object* owner)
	:ObjectComponent(owner)
{
}

ParticleSystem::~ParticleSystem()
{
}

ParticleSystem* ParticleSystem::Create(Object* owner)
{
	return nullptr;
}
