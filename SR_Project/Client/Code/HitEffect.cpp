#include "pch.h"
#include "HitEffect.h"

//component
#include "TransformComponent.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

HitEffect::HitEffect(ObjectManager* owner, ObjectType objType)
	:Effect(owner,objType)
{
}

HitEffect::~HitEffect()
{
}

HitEffect* HitEffect::Create(ObjectManager* owner, ObjectType objType)
{
	HitEffect* Instance = new HitEffect(owner, objType);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT HitEffect::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();

	auto particle = AddComponent<ParticleSystem>();
	Emitter hit;
	hit.burstCount = 10;
	hit.looping = false;
	hit.followCam = false;

	hit.life = 0.2f;
	hit.size = 20.f;

	hit.velocityMin = { -20.f,-20.f,-20.f };
	hit.velocityMax = { 20.f,20.f,20.f };
	hit.spawnAreaMin = { -0.1f,-0.1f,-0.1f };
	hit.spawnAreaMax = { 0.1f,0.1f,0.1f };

	hit.color = { 1.f,0.f,0.f,1.f };

	particle->AddEmitter(hit, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("Hit_Mtrl");

	return S_OK;
}

void HitEffect::Update(_float dt)
{
	Object::Update(dt);
}

void HitEffect::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void HitEffect::Free()
{
	Object::Free();
}
