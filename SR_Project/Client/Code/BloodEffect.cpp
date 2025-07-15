#include "pch.h"
#include "BloodEffect.h"
#include "Scene.h"
#include "ObjectManager.h"

//component
#include "TransformComponent.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

BloodEffect::BloodEffect(ObjectManager* owner, ObjectType objType)
	:Effect(owner,objType)
{
}

BloodEffect::~BloodEffect()
{
}

BloodEffect* BloodEffect::Create(ObjectManager* owner, ObjectType objType)
{
	BloodEffect* Instance = new BloodEffect(owner, objType);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT BloodEffect::Ready_Object()
{
	auto transform = AddComponent<TransformComponent>();

	auto particle = AddComponent<ParticleSystem>();
	Emitter hit;
	hit.burstCount = 10;
	hit.looping = true;
	hit.followCam = false;

	hit.life = 0.5f;
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

void BloodEffect::Update(_float dt)
{
	Object::Update(dt);

	if (effectOwner)
	{
		_vec3 pos = effectOwner->GetPosition();
		GetComponent<TransformComponent>()->SetPosition(pos);
	}

	//if (timer >= deadTime)
	//	SetDead();
	//
	//if (deadTime)
	//	timer += dt;
}

void BloodEffect::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void BloodEffect::Free()
{
	Object::Free();
}
