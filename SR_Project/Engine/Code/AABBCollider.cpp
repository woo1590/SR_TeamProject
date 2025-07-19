#include "EnginePCH.h"
#include "AABBCollider.h"
#include "GraphicDevice.h"
#include "Object.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

AABBCollider::AABBCollider(CollisionComponent* transform)
	:Collider(transform)
{
}

AABBCollider::~AABBCollider()
{
}

AABBCollider* AABBCollider::Create(CollisionComponent* transform)
{
	AABBCollider* Instance = new AABBCollider(transform);

	return Instance;
}

void AABBCollider::Update()
{
	_vec3 pos = owner->GetOwner()->GetComponent<TransformComponent>()->GetPosition();

	worldAABB.center = localAABB.center + pos;
	worldAABB.half = localAABB.half;
}

AABB AABBCollider::GetWorldAABB()
{
	return worldAABB;
}

bool AABBCollider::CheckCollision(Collider* other)
{
	return other->CollisionAABB(this);
}

bool AABBCollider::CollisionAABB(AABBCollider* other)
{
	AABB a = worldAABB;
	AABB b = other->GetWorldAABB();

	_vec3 aMin = a.center - a.half;
	_vec3 aMax = a.center + a.half;
	_vec3 bMin = b.center - b.half;
	_vec3 bMax = b.center + b.half;

	if (aMin.x > bMax.x || aMax.x < bMin.x) return false;
	if (aMin.y > bMax.y || aMax.y < bMin.y) return false;
	if (aMin.z > bMax.z || aMax.z < bMin.z) return false;

	return true;
}

bool AABBCollider::CollisionOBB(OBBCollider* other)
{
	return false;
}

void AABBCollider::SetSize(_vec3 size)
{
	localAABB.half = size * 0.5f;
}

void AABBCollider::SetOffset(_vec3 offset)
{
	localAABB.center = offset;	
}

void AABBCollider::Render()
{

}

void AABBCollider::Free()
{
}
