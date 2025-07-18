#include "pch.h"
#include "ExplodeEffect.h"
#include "EngineCore.h"
#include "ResourceManager.h"
#include "Material.h"

//component
#include "TransformComponent.h"
#include "ParticleSystem.h"
#include "ParticleRenderer.h"

ExplodeEffect::ExplodeEffect(ObjectManager* owner, ObjectType objType)
	:Effect(owner, objType)
{
}

ExplodeEffect::~ExplodeEffect()
{
}

ExplodeEffect* ExplodeEffect::Create(ObjectManager* owner, ObjectType objType)
{
	ExplodeEffect* Instance = new ExplodeEffect(owner, objType);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ExplodeEffect::Ready_Object()
{
	AddComponent<TransformComponent>();

	auto particle = AddComponent<ParticleSystem>();
	Emitter explode;
	explode.burstCount = 4;
	explode.life = 0.5f;
	explode.looping = false;
	explode.followCam = false;
	explode.color = { 1.f,0.4f,0.f,1.f };
	explode.colorFade = { 0.f,0.f,0.f,1.f };
	
	explode.velocityMin = { 0.f,0.f,0.f };
	explode.velocityMax = { 0.1f,0.1f,0.1f };
	explode.spawnAreaMin = { -6.f,-3.f,-6.f };
	explode.spawnAreaMax = { 6.f,3.f,6.f };
	explode.size = 100.f;
	
	particle->AddEmitter(explode, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;

			p.color.w -= p.colorFade.w * dt;
			if (p.color.w <= 0.f)
				p.color.w = 0.f;
			_float t = p.age / p.life;
			t = std::clamp(t, 0.f, 1.f);
			float tFast = std::pow(t, 0.3f);

			p.size = std::lerp(100.f, 400.f, tFast);
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("Explode_Mtrl");

	return S_OK;
}

void ExplodeEffect::Update(_float dt)
{
	Effect::Update(dt);
}

void ExplodeEffect::Free()
{
}
