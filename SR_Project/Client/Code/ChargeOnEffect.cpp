#include "pch.h"
#include "ChargeOnEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "ObjectManager.h"
#include "Player.h"

ChargeOnEffect::ChargeOnEffect(ObjectManager* owner, ObjectType objType, Object* _onPos) : Effect(owner, objType)
{
    effectOwner = _onPos->GetComponent<TransformComponent>();
}

ChargeOnEffect::~ChargeOnEffect() {}

ChargeOnEffect* ChargeOnEffect::Create(ObjectManager* owner, ObjectType objType, Object* _onPos)
{
	ChargeOnEffect* Instance = new ChargeOnEffect(owner, objType, _onPos);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ChargeOnEffect::Ready_Object()
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
	transform->SetPosition(pos + vDir);

	transform->SetScale(_vec3(Scale, Scale, 0.1f));

	auto fAngle = D3DXToRadian(-45.f);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("ChargeOn_Mtrl");
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(1.f, 0.f, 0.f));

	return S_OK;
}

void ChargeOnEffect::Update(_float dt)
{
	Effect::Update(dt);

	auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));;
	auto playerAttackDirection = player->GetAttackDirection();
	playerAttackDirection.y = 0.f;
	auto pos = effectOwner->GetWorldPosition();

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &playerAttackDirection);

	auto transform = GetComponent<TransformComponent>();
	transform->SetPosition(pos + vDir);

	if (!player->IsCharge())
	{
		SetDead();
		return;
	}

	auto fProgress = timer / 1.f;
	auto offset = -cosf(fProgress * 2 * D3DX_PI);
	auto curScale = Scale * (0.5f - offset * 0.1f);
	transform->SetScale(_vec3(curScale, curScale, 0.1f));
}

void ChargeOnEffect::Free()
{
	Effect::Free();
}
