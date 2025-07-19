#include "EnginePCH.h"
#include "AABBCollider.h"
#include "GraphicDevice.h"
#include "Object.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

AABBCollider::AABBCollider(CollisionComponent* transform)
	:Collider(transform)
{
	auto device = GraphicDevice::GetInstance()->GetDevice();
	D3DXCreateBox(device, localAABB.half.x * 2.f, localAABB.half.y, localAABB.half.z * 2.f, &debugBox, nullptr);
}

AABBCollider::~AABBCollider()
{
}

AABBCollider* AABBCollider::Create(CollisionComponent* transform)
{
	AABBCollider* Instance = new AABBCollider(transform);

	return Instance;
}

AABB AABBCollider::GetWorldAABB()
{
	auto transform = owner->GetOwner()->GetComponent<TransformComponent>();
	_vec3 pos = transform->GetWorldPosition();

	AABB worldAABB;
	worldAABB.center = localAABB.center + pos;
	worldAABB.half = localAABB.half;

	return worldAABB;
}

bool AABBCollider::CheckCollision(Collider* other)
{
	return other->CollisionAABB(this);
}

bool AABBCollider::CollisionAABB(AABBCollider* other)
{
	AABB a = GetWorldAABB();
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

	if (debugBox)
		Safe_Release(debugBox);

	auto device = GraphicDevice::GetInstance()->GetDevice();
	D3DXCreateBox(device, localAABB.half.x * 2.f, localAABB.half.y * 2.f, localAABB.half.z * 2.f, &debugBox, nullptr);
}

void AABBCollider::SetOffset(_vec3 offset)
{
	localAABB.center = offset;	
}

void AABBCollider::Render()
{
	auto device = GraphicDevice::GetInstance()->GetDevice();

	auto transform = owner->GetOwner()->GetComponent<TransformComponent>();
	_vec3 pos = transform->GetWorldPosition();

	_matrix worldMat;
	D3DXMatrixTranslation(&worldMat, pos.x, pos.y, pos.z);
	device->SetTransform(D3DTS_WORLD, &worldMat);

	debugBox->DrawSubset(0);
}

void AABBCollider::Free()
{
}
