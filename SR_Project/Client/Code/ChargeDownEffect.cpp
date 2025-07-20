#include "pch.h"
#include "ChargeDownEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "ObjectManager.h"
#include "Player.h"

ChargeDownEffect::ChargeDownEffect(ObjectManager* owner, ObjectType objType, Object* _onPos) : Effect(owner, objType)
{
	effectOwner = _onPos->GetComponent<TransformComponent>();
}

ChargeDownEffect::~ChargeDownEffect() {}

ChargeDownEffect* ChargeDownEffect::Create(ObjectManager* owner, ObjectType objType, Object* _onPos)
{
	ChargeDownEffect* Instance = new ChargeDownEffect(owner, objType, _onPos);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT ChargeDownEffect::Ready_Object()
{
	SetDeadTime(1.0f);

	Scale = 0.3f;
	auto transform = AddComponent<TransformComponent>();
	auto pos = effectOwner->GetWorldPosition();
	transform->SetPosition(pos);

	transform->SetScale(_vec3(Scale, Scale, 0.1f));
	
	auto fAngle = D3DXToRadian(-45.f);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("ChargeDown_Mtrl");
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(1.f, 1.f, 1.f));

	return S_OK;
}

void ChargeDownEffect::Update(_float dt)
{
    Effect::Update(dt);

	auto fProgress = timer / deadTime;
	fProgress = std::clamp(fProgress, 0.f, 1.f);

	auto renderer = GetComponent<MeshRenderer>();
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(1.f, 1-fProgress, 1-fProgress));

	auto transform = GetComponent<TransformComponent>();
	auto pos = effectOwner->GetWorldPosition();
	transform->SetPosition(pos);

	auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));;
	if (!player->IsCharge())
	{
		SetDead();
		return;
	}

	fProgress *= 0.6;
	auto curScale = Scale * (1.f - fProgress);
	transform->SetScale(_vec3(curScale, curScale, 0.1f));
}

void ChargeDownEffect::Free()
{
    Effect::Free();
}
