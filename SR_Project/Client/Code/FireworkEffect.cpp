#include "pch.h"
#include "FireworkEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "EngineCore.h"
#include "Random.h"
#include "ParticleRenderer.h"
#include "ParticleSystem.h"

FireworkEffect::FireworkEffect(ObjectManager* owner, ObjectType objType, Object* _onPos) : Effect(owner, objType)
{
    startPos = _onPos->GetComponent<TransformComponent>()->GetWorldPosition();
}

FireworkEffect::~FireworkEffect() {}

FireworkEffect* FireworkEffect::Create(ObjectManager* owner, ObjectType objType, Object* _onPos)
{
	FireworkEffect* Instance = new FireworkEffect(owner, objType, _onPos);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT FireworkEffect::Ready_Object()
{
	SetDeadTime(0.5f);

	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(startPos);

	auto random = EngineCore::GetInstance()->GetRandom();
	
	std::vector<_vec3> baseColors = {
		_vec3(1.f, 0.f, 0.f),
		_vec3(0.f, 1.f, 0.f),
		_vec3(0.f, 0.f, 1.f),
		_vec3(1.f, 1.f, 0.f),
		_vec3(1.f, 0.f, 1.f),
		_vec3(0.f, 1.f, 1.f),
		_vec3(1.f, 1.f, 1.f)
	};

	int iIndex = random->get<int>(0, static_cast<int>(baseColors.size()) - 1);
	_vec3 baseColor = baseColors[iIndex];

	_float fRatio = random->get<float>(0.5f, 1.0f);
	_vec3 finalColor = baseColor * fRatio;

	auto particle = AddComponent<ParticleSystem>();
	Emitter hit;
	hit.burstCount = 10;
	hit.looping = false;
	hit.followCam = false;

	hit.life = 0.5f;
	hit.size = 10.f;

	hit.velocityMin = { -6.f,-6.f,-6.f };
	hit.velocityMax = { 6.f,6.f,6.f };
	hit.spawnAreaMin = { -0.1f,-0.1f,-0.1f };
	hit.spawnAreaMax = { 0.1f,0.1f,0.1f };

	hit.color = _vec4(finalColor.x, finalColor.y, finalColor.z,1.f);
	hit.colorFade = { 0.f,0.f,0.f,1.f };

	particle->AddEmitter(hit, [](Particle& p, _float dt)
		{
			p.position += p.velocity * dt;
			p.color.w -= p.colorFade.w * dt * 1.5f;
			if (p.color.w <= 0.f)
				p.color.w = 0.f;
		});

	auto renderer = AddComponent<ParticleRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMaterial("FireworkTikkle_Mtrl");

	return S_OK;
}

void FireworkEffect::Update(_float dt)
{
	Effect::Update(dt);

	GetComponent<TransformComponent>()->SetPosition(startPos);
}

void FireworkEffect::Free()
{
	Effect::Free();
}
