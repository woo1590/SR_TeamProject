#include "pch.h"
#include "HitEffect.h"
#include "Scene.h"
#include "ObjectManager.h"

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
	hit.ratePerSec = 20.f;
	hit.looping = true;
	hit.followCam = false;

	hit.life = 5.f;
	hit.size = 15.f;

	hit.velocityMin = { -10.f,-10.f,-10.f };
	hit.velocityMax = { 10.f,10.f,10.f };
	hit.spawnAreaMin = { -0.1f,-0.1f,-0.1f };
	hit.spawnAreaMax = { 0.1f,0.1f,0.1f };

	hit.color = { 1.f,0.f,0.f,1.f };
	hit.colorFade = { 0.f,0.f,0.f,1.f };

	particle->AddEmitter(hit, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;
			p.color.w -= p.colorFade.w * dt;
			if (p.color.w <= 0.f)
				p.color.w = 0.f;
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("Hit_Mtrl");

	return S_OK;
}

void HitEffect::Update(_float dt)
{
	Object::Update(dt);

	auto player = owner->GetFrontObject(ObjectType::Player);
	_vec3 pos = player->GetComponent<TransformComponent>()->GetPosition();
	GetComponent<TransformComponent>()->SetPosition(pos);

}

void HitEffect::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void HitEffect::Free()
{
	Object::Free();
}
