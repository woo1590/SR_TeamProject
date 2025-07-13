#include "pch.h"
#include "Rain.h"

//component
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

Rain::Rain(ObjectManager* owner, ObjectType objType)
	:Effect(owner,objType)
{
}

Rain::~Rain()
{
}

Rain* Rain::Create(ObjectManager* owner, ObjectType objType)
{
	Rain* Instance = new Rain(owner, objType);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT Rain::Ready_Object()
{
	Emitter rain;
	rain.looping = true;
	rain.followCam = true;
	rain.ratePerSec = 100.f;
	rain.size = 50.f;
	rain.life = 10.f;
	rain.spawnAreaMin = { -30.f,20.f,-30.f };
	rain.spawnAreaMax = { 30.f,20.f,30.f };
	rain.velocityMin = { 0.f,-50.f,0.f };
	rain.velocityMax = { 0.f,-60.f,0.f };

	auto particleSys = AddComponent<ParticleSystem>();
	particleSys->AddEmitter(rain, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("Rain_Mtrl");

	return S_OK;
}

void Rain::Update(_float dt)
{
	Object::Update(dt);
}

void Rain::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void Rain::Free()
{
	Object::Free();
}
