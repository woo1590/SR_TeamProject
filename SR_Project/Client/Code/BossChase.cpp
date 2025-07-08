#include "pch.h"
#include "BossChase.h"
#include "Object.h"
#include "TransformComponent.h"
#include "BaseCharacter.h"

BossChase::BossChase()
{
}

BossChase::~BossChase()
{
}

BTStatus BossChase::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus BossChase::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));

	if (self == nullptr || target == nullptr) return BTStatus::Failure;

	_vec3 TargetPos = target->GetComponent<TransformComponent>()->GetPosition();
	_vec3 SelfPos = self->GetComponent<TransformComponent>()->GetPosition();

	_vec3 Dir = TargetPos - SelfPos;

	static_cast<BaseCharacter*>(self)->MoveTo(D3DXVec3Normalize(&Dir, &Dir), dt);
	return BTStatus::Success;
}

void BossChase::Free()
{
}
