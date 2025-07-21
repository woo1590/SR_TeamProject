#include "pch.h"
#include "RollEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "ObjectManager.h"
#include "Player.h"

RollEffect::RollEffect(ObjectManager* owner, ObjectType objType, Object* _onPos, _float _fAngle) : Effect(owner, objType)
{
    effectOwner = _onPos->GetComponent<TransformComponent>();
	moveAngle = _fAngle;
}

RollEffect::~RollEffect() {}

RollEffect* RollEffect::Create(ObjectManager* owner, ObjectType objType, Object* _onPos, _float _fAngle)
{
	RollEffect* Instance = new RollEffect(owner, objType, _onPos, _fAngle);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT RollEffect::Ready_Object()
{
	SetDeadTime(0.3f);

	auto pos = effectOwner->GetWorldPosition();
	_vec3 offset = { 0.f,-3.5f,0.f };
	startPos = pos + offset;

	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(startPos);

	Scale = 0.3f;
	transform->SetScale(_vec3(Scale, Scale, 0.1f));

	auto fAngle = D3DXToRadian(-45.f);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("Roll_Mtrl");
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(0.4f, 0.35f, 0.2f));

	moveVec = { sinf(moveAngle),0.f,cosf(moveAngle)};
	moveVec *= moveSpeed;

	return S_OK;
}

void RollEffect::Update(_float dt)
{
	Effect::Update(dt);

	auto fProgress = timer / deadTime;
	fProgress = std::clamp(fProgress, 0.f, 1.f);
	_vec3 yOffset = { 0.f,sinf(fProgress * D3DX_PI),0.f };

	auto transform = GetComponent<TransformComponent>();
	transform->SetPosition(startPos + moveVec * fProgress + yOffset);
}

void RollEffect::Free()
{
	Effect::Free();
}
