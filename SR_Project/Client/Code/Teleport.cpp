#include "pch.h"
#include "Teleport.h"
#include "Object.h"
#include "Monster.h"
#include "TransformComponent.h"
#include "EngineCore.h"
#include "SoundManager.h"

TeleportNode::TeleportNode()
{
}

TeleportNode::~TeleportNode()
{
}

BTStatus TeleportNode::Initialize(float dt)
{
	return BTStatus::Running;
}

BTStatus TeleportNode::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	Object* target = static_cast<Object*>(bb->GetValue("Target"));

	if (self == nullptr) return BTStatus::Failure;

	auto targettransform = target->GetComponent<TransformComponent>();
	_vec3 targetpos = targettransform->GetPosition();
	auto selftransform = self->GetComponent<TransformComponent>();
	_vec3 selfpos = selftransform->GetPosition();

	_vec3 Dir = selfpos - targetpos;

	if (D3DXVec3Length(&Dir) < *(static_cast<float*>(bb->GetValue("NearDistance"))))
	{
		Dir.y = 0;

		_vec3 selfforward = selftransform->GetFoward();

		float teleportDistance = 20.f;
		_vec3 teleportPos = selfforward * -teleportDistance;

		_vec3 targetTeleportPos = targetpos - teleportPos;

		targetTeleportPos.y = selfpos.y;

		self->GetComponent<TransformComponent>()->SetPosition(targetTeleportPos);
		self->GetComponent<TransformComponent>()->SetForward(Dir);
		EngineCore::GetInstance()->GetSoundManager()->PlaySFX("IdleBlastling");
		return BTStatus::Success;
	}
	else
		return BTStatus::Success;
}

void TeleportNode::Free()
{
	ActionNode::Free();
}
