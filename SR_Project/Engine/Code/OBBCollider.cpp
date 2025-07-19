#include "EnginePCH.h"
#include "OBBCollider.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Object.h"

OBBCollider::OBBCollider(CollisionComponent* owner)
	:Collider(owner)
{
}

OBBCollider::~OBBCollider()
{
}

OBBCollider* OBBCollider::Create(CollisionComponent* owner)
{
	OBBCollider* Instance = new OBBCollider(owner);

	return Instance;
}

AABB OBBCollider::GetWorldAABB()
{
	OBB worldOBB = GetWorldOBB();

	AABB worldAABB;

	worldAABB.center = worldOBB.center;

	worldAABB.half.x = std::abs(worldOBB.axis._11) * worldOBB.half.x + 
					   std::abs(worldOBB.axis._21) * worldOBB.half.y + 
					   std::abs(worldOBB.axis._31) * worldOBB.half.z;

	worldAABB.half.y = std::abs(worldOBB.axis._12) * worldOBB.half.x +
					   std::abs(worldOBB.axis._22) * worldOBB.half.y + 
					   std::abs(worldOBB.axis._32) * worldOBB.half.z;

	worldAABB.half.z = std::abs(worldOBB.axis._13) * worldOBB.half.x + 
					   std::abs(worldOBB.axis._23) * worldOBB.half.y + 
					   std::abs(worldOBB.axis._33) * worldOBB.half.z;

	return worldAABB;
}

bool OBBCollider::CheckCollision(Collider* other)
{
	return other->CollisionOBB(this);
}

bool OBBCollider::CollisionAABB(AABBCollider* other)
{
	return false;
}

bool OBBCollider::CollisionOBB(OBBCollider* other)
{
	return false;
}

void OBBCollider::SetSize(_vec3 size)
{
	localOBB.half = size * 0.5f;
}

void OBBCollider::SetOffset(_vec3 offset)
{
	localOBB.center = offset;	
}

OBB OBBCollider::GetWorldOBB()
{
	OBB worldOBB;

	auto transform = owner->GetOwner()->GetComponent<TransformComponent>();
	_vec3 pos = transform->GetWorldPosition();
	_matrix worldMat = transform->GetWorldMatrix();
	_matrix rotMat;
	D3DXMatrixIdentity(&rotMat);

	_vec3 x = _vec3(worldMat._11, worldMat._12, worldMat._13);
	_vec3 y = _vec3(worldMat._21, worldMat._22, worldMat._23);
	_vec3 z = _vec3(worldMat._31, worldMat._32, worldMat._33);

	D3DXVec3Normalize(&x, &x);
	D3DXVec3Normalize(&y, &y);
	D3DXVec3Normalize(&z, &z);

	rotMat._11 = x.x; rotMat._12 = x.y; rotMat._13 = x.z;
	rotMat._21 = y.x; rotMat._22 = y.y; rotMat._23 = y.z;
	rotMat._31 = z.x; rotMat._32 = z.y; rotMat._33 = z.z;

	_vec3 rotOffset;
	D3DXVec3TransformNormal(&rotOffset, &localOBB.center, &rotMat);

	worldOBB.center = pos + rotOffset;
	worldOBB.half = localOBB.half;
	worldOBB.axis = rotMat;

	return worldOBB;
}

void OBBCollider::Free()
{
}


