#include "pch.h"
#include "DeadEffect.h"
#include "Material.h"
#include "EngineCore.h"
#include "ResourceManager.h"

//component
#include "ParticleRenderer.h"
#include "ParticleSystem.h"
#include "TransformComponent.h"

DeadEffect::DeadEffect(ObjectManager* owner, ObjectType objType)
	:Effect(owner,objType)
{
}

DeadEffect::~DeadEffect()
{
}

DeadEffect* DeadEffect::Create(ObjectManager* owner, ObjectType objType)
{
	DeadEffect* Instance = new DeadEffect(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT DeadEffect::Ready_Object()
{
	AddComponent<TransformComponent>();

	auto particle = AddComponent<ParticleSystem>();
	Emitter dead;
	dead.burstCount = 8;
	dead.looping = false;
	dead.followCam = false;
	
	dead.velocityMin = { 0.f,5.f,0.f };
	dead.velocityMax = { 0.f,6.f,0.f };
	dead.spawnAreaMin = { -6.f,0.f,-6.f };
	dead.spawnAreaMax = { 6.f,0.f,6.f };

	dead.life = 1.5f;
	dead.size = 60.f;

	particle->AddEmitter(dead, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("Dead_Mtrl");

	totalFrame = 8;
	currFrame = 0;

	return S_OK;
}

void DeadEffect::Update(_float dt)
{
	Effect::Update(dt);

	_float ratio = timer / deadTime;
	currFrame = static_cast<int>(ratio * totalFrame);

	if (currFrame >= totalFrame)
		currFrame = totalFrame - 1;

	auto mtrl = GetComponent<ParticleRenderer>()->GetMaterial();
	auto tex = EngineCore::GetInstance()->GetResourceManager()->GetTexture("Dead" + std::to_string(currFrame));
	mtrl->SetTexture("AlbedoMap", tex);
}

void DeadEffect::Free()
{
}
