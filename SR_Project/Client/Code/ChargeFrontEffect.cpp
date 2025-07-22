#include "pch.h"
#include "ChargeFrontEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "ObjectManager.h"
#include "Player.h"
#include "TikkleEffect.h"

#include "EngineCore.h"
#include "SoundManager.h"

ChargeFrontEffect::ChargeFrontEffect(ObjectManager* owner, ObjectType objType, Object* _onPos) : Effect(owner, objType)
{
    effectOwner = _onPos->GetComponent<TransformComponent>();
}

ChargeFrontEffect::~ChargeFrontEffect() {}

ChargeFrontEffect* ChargeFrontEffect::Create(ObjectManager* owner, ObjectType objType, Object* _onPos)
{
	ChargeFrontEffect* Instance = new ChargeFrontEffect(owner, objType, _onPos);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ChargeFrontEffect::Ready_Object()
{
	SetDeadTime(1000.0f);

	Scale = 0.3f;
	auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));;
	auto playerAttackDirection = player->GetAttackDirection();
	playerAttackDirection.y = 0.f;
	auto pos = effectOwner->GetWorldPosition();

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &playerAttackDirection);

	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(pos + vDir * 2.f);

	transform->SetScale(_vec3(Scale * 0.1f, Scale * 0.1f, 0.1f));

	auto fAngle = D3DXToRadian(-45.f);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("ChargeFrontBall_Mtrl");
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(1.f, 0.f, 0.f));

	EngineCore::GetInstance()->GetSoundManager()->PlaySFX("Charge");

	return S_OK;
}

void ChargeFrontEffect::Update(_float dt)
{
	Effect::Update(dt);

	auto fProgress = timer / 1.f;
	fProgress = std::clamp(fProgress, 0.f, 1.f);

	auto renderer = GetComponent<MeshRenderer>();
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(1.f, 1 - fProgress, 1 - fProgress));

	auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));;
	if (!player->IsCharge())
	{
		SetDead();
		EngineCore::GetInstance()->GetSoundManager()->Stop("Charge");
		return;
	}
	auto playerAttackDirection = player->GetAttackDirection();
	playerAttackDirection.y = 0.f;
	auto pos = effectOwner->GetWorldPosition();

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &playerAttackDirection);

	auto transform = GetComponent<TransformComponent>();
	transform->SetPosition(pos + vDir * 2.f);

	fProgress *= 0.9;
	auto curScale = Scale * (0.1f + fProgress);
	transform->SetScale(_vec3(curScale, curScale, 0.1f));

	tikkleTimer += dt;
	if (tikkleTimer >= tikkleTerm)
	{
		tikkleTimer = 0.f;
		auto tikkle = TikkleEffect::Create(owner, ObjectType::ParticleEffect, this);
		owner->AddObject(ObjectType::ParticleEffect, tikkle);
	}

	soundTimer += dt;
	if (soundTimer > soundTerm)
	{
		EngineCore::GetInstance()->GetSoundManager()->PlaySFX("Charge");
		soundTimer = 0.f;
	}
}

void ChargeFrontEffect::Free()
{
	Effect::Free();
}
