#include "pch.h"
#include "IsRandomMoveCheck.h"
#include "Ender.h"
#include "Object.h"
#include "TransformComponent.h"
#include "PhysicsComponent.h"

IsRandomMoveCheck::IsRandomMoveCheck(BTNode* child)
	:DecoratorNode(child)
{
}

IsRandomMoveCheck::~IsRandomMoveCheck()
{
}

BTStatus IsRandomMoveCheck::Initialize(float dt)
{
	return BTStatus();
}

BTStatus IsRandomMoveCheck::Tick(float dt, BlackBoard* bb)
{
	if (bb == nullptr) return BTStatus::Failure;

	Object* self = static_cast<Object*>(bb->GetValue("Self"));
	_vec3* targetPos = static_cast<_vec3*>(bb->GetValue("targetPos"));

	if (self == nullptr) return BTStatus::Failure;

	(*targetPos).y = 0;
	auto transform = self->GetComponent<TransformComponent>();
	_vec3 pos = transform->GetPosition();
	pos.y = 0;

	_vec3 length = *targetPos - pos;
	if (D3DXVec3Length(&length) < 0.1)
		return Child->Tick(dt, bb);
	else
	{
		if (self->GetComponent<PhysicsComponent>()->IsGrounded())
		{
			static_cast<Ender*>(self)->MoveTo(*targetPos, dt);
			return BTStatus::Running;
		}
		else
			return BTStatus::Success;
	}
}

void IsRandomMoveCheck::Free()
{
	DecoratorNode::Free();
}
