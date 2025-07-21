#include "pch.h"
#include "FireworkEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "EngineCore.h"
#include "Random.h"

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

	Scale = 0.02f;
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(_vec3(Scale, Scale, 0.1f));
	auto random = EngineCore::GetInstance()->GetRandom();
	moveDir =
	{
		random->get<float>(-1.f, 1.f),
		random->get<float>(-1.f, 1.f),
		random->get<float>(-1.f, 1.f)
	};
	D3DXVec3Normalize(&moveDir, &moveDir);

	moveDir *= moveSpeed;

	auto fAngle = D3DXToRadian(-45.f);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("Tikkle_Mtrl");

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

	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", finalColor);

	return S_OK;
}

void FireworkEffect::Update(_float dt)
{
	Effect::Update(dt);

	auto fProgress = timer / deadTime;
	fProgress = std::clamp(fProgress, 0.f, 1.f);
	auto pos = startPos + moveDir * fProgress;

	auto transform = GetComponent<TransformComponent>();
	transform->SetPosition(pos);
}

void FireworkEffect::Free()
{
	Effect::Free();
}
