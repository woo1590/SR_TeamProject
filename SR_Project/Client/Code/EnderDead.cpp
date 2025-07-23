#include "pch.h"
#include "EnderDead.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "Material.h"

//component
#include "TransformComponent.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

EnderDead::EnderDead(ObjectManager* owner, ObjectType objType)
	:DeadEffect(owner,objType)
{
}

EnderDead::~EnderDead()
{
}

EnderDead* EnderDead::Create(ObjectManager* owner, ObjectType objType)
{
	EnderDead* Instance = new EnderDead(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT EnderDead::Ready_Object()
{
	AddComponent<TransformComponent>();

	auto particle = AddComponent<ParticleSystem>();
	Emitter dead;
	dead.ratePerSec = 10.f;
	dead.looping = true;
	dead.followCam = false;

	dead.life = 2.5f;
	dead.velocityMin = { 0.f,10.f,0.f };
	dead.velocityMax = { 0.f,11.f,0.f };
	dead.spawnAreaMin = { -5.f,3.f,-5.f };
	dead.spawnAreaMax = { 5.f,6.f,5.f };

	dead.size = 60.f;	

	particle->AddEmitter(dead, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("Dead_Mtrl");

	auto tex = EngineCore::GetInstance()->GetResourceManager()->GetTexture("Dead3");
	renderer->GetMaterial()->SetTexture("AlbedoMap", tex);

	return S_OK;
}

void EnderDead::Update(_float dt)
{
	Effect::Update(dt);
}

void EnderDead::Late_Update(_float dt)
{
	Effect::Late_Update(dt);
}

void EnderDead::Free()
{
	Object::Free();
}
