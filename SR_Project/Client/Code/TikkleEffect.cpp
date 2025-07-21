#include "pch.h"
#include "TikkleEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"
#include "EngineCore.h"
#include "Random.h"

TikkleEffect::TikkleEffect(ObjectManager* owner, ObjectType objType, Object* _onPos) : Effect(owner, objType)
{
	destinationPos = _onPos->GetComponent<TransformComponent>()->GetWorldPosition();
}

TikkleEffect::~TikkleEffect() {}

TikkleEffect* TikkleEffect::Create(ObjectManager* owner, ObjectType objType, Object* _onPos)
{
	TikkleEffect* Instance = new TikkleEffect(owner, objType, _onPos);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT TikkleEffect::Ready_Object()
{
	SetDeadTime(0.3f);

	Scale = 0.02f;
	auto transform = AddComponent<TransformComponent>();
	transform->SetScale(_vec3(Scale, Scale, 0.1f));
	auto random = EngineCore::GetInstance()->GetRandom();
	startPos = 
	{ 
		destinationPos.x + random->get<float>(-1.f, 1.f),
		destinationPos.y + random->get<float>(-1.f, 1.f),
		destinationPos.z + random->get<float>(-1.f, 1.f)
	};

	auto fAngle = D3DXToRadian(-45.f);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("Tikkle_Mtrl");
	auto mtrl = renderer->GetMaterial();



	mtrl->SetVec3("color", _vec3(0.8f + random->get<float>(0.f,0.2f), random->get<float>(0.f,0.5f), 0.f));

	return S_OK;
}

void TikkleEffect::Update(_float dt)
{
	Effect::Update(dt);

	auto fProgress = timer / deadTime;
	fProgress = std::clamp(fProgress, 0.f, 1.f);

	moveDir = destinationPos - startPos;
	auto pos = startPos + moveDir * fProgress;

	auto transform = GetComponent<TransformComponent>();
	transform->SetPosition(pos);
}

void TikkleEffect::Free()
{
	Effect::Free();
}
