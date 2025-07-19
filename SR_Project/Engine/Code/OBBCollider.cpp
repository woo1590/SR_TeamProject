#include "EnginePCH.h"
#include "OBBCollider.h"

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

void OBBCollider::Update()
{
}

AABB OBBCollider::GetWorldAABB()
{
	return AABB();
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

void OBBCollider::Free()
{
}


