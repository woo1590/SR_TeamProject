#include "pch.h"
#include "StatikkEffect.h"

#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "Material.h"

StatikkEffect::StatikkEffect(ObjectManager* owner, ObjectType objType, _vec3 _startPos, _vec3 _endPos) : Effect(owner,objType)
{
	startPos = _startPos;
	endPos = _endPos;
}

StatikkEffect::~StatikkEffect() {}

StatikkEffect* StatikkEffect::Create(ObjectManager* owner, ObjectType objType, _vec3 _startPos, _vec3 _endPos)
{
	StatikkEffect* Instance = new StatikkEffect(owner, objType,_startPos,_endPos);
	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT StatikkEffect::Ready_Object()
{
	SetDeadTime(0.3f);
	
	Scale = 0.1f;
	auto transform = AddComponent<TransformComponent>();

	_vec3 dirVec = endPos - startPos;

	_vec3 middlePos = startPos + dirVec * 0.5f;
	transform->SetPosition(middlePos);

	_float distance = sqrtf(dirVec.x * dirVec.x + dirVec.z * dirVec.z);
	transform->SetScale(_vec3(distance * Scale, distance * Scale, 0.1f * Scale));

	D3DXVec3Normalize(&dirVec, &dirVec);

	float fAngle = D3DXToRadian(90.f) + atan2f(dirVec.x, dirVec.z);
	transform->SetRotate(_vec3(0.f, fAngle, 0.f));

	auto renderer = AddComponent<MeshRenderer>(RENDER_ID::Render_Alpha);
	renderer->SetMesh("Quad_Mesh");
	renderer->SetMaterial("Statikk_Mtrl");
	auto mtrl = renderer->GetMaterial();
	mtrl->SetVec3("color", _vec3(0.f, 1.f, 1.f));

	return S_OK;
}

void StatikkEffect::Update(_float dt)
{
	Effect::Update(dt);
}

void StatikkEffect::Free()
{
	Effect::Free();
}
