#include "EnginePCH.h"
#include "CollisionComponent.h"

#include "EngineCore.h"
#include "SceneManager.h"
#include "RenderSystem.h"
#include "CollisionSystem.h"
#include "PhysicsSystem.h"
#include "GraphicDevice.h"
#include "Scene.h"
#include "Object.h"

//component
#include "TransformComponent.h"

CollisionComponent::CollisionComponent(Object* owner)
	:ObjectComponent(owner)
{
}

CollisionComponent::~CollisionComponent()
{
}

CollisionComponent* CollisionComponent::Create(Object* owner)
{
	CollisionComponent* Instance = new CollisionComponent(owner);

	if (FAILED(Instance->Ready_Component()))
	{
		Safe_Release(Instance);

		Instance = nullptr;
	}

	return Instance;
}

HRESULT CollisionComponent::Ready_Component()
{
	BBType = BoundingBoxType::Box;

	auto device = GraphicDevice::GetInstance()->GetDevice();
	if (BoundingBox)
		Safe_Release(BoundingBox);

	D3DXCreateBox(device, 2.f, 2.f, 2.f, &BoundingBox, nullptr);
	return S_OK;
}

void CollisionComponent::Update(_float dt)
{
	ObjectComponent::Update(dt);
}

void CollisionComponent::Late_Update(_float dt)
{
	EngineCore::GetInstance()->GetRenderSystem()->RegisterCollision(this);
}

void CollisionComponent::SetOffset(_vec3 offset)
{
	Offset = offset;
}

void CollisionComponent::SetOffset(_float x, _float y, _float z)
{
	SetOffset(_vec3(x, y, z));
}

void CollisionComponent::SetBoudingBox(BoundingBoxType bbType)
{
	if (BoundingBox)
		Safe_Release(BoundingBox);

	auto device = GraphicDevice::GetInstance()->GetDevice();

	BBType = bbType;

}

void CollisionComponent::SetSize(_vec3 size)
{
	_vec3 half = size * 0.5f;

	LocalMin = -half;
	LocalMax = half;

	auto device = GraphicDevice::GetInstance()->GetDevice();
	if (BoundingBox)
		Safe_Release(BoundingBox);

	D3DXCreateBox(device, size.x, size.y, size.z, &BoundingBox, nullptr);
}

void CollisionComponent::SetLayer(CollisionLayer layer)
{
	Collision_Layer = layer;
}

void CollisionComponent::SetMask(_ulong mask)
{
	Collision_Mask = mask;
}

_vec3 CollisionComponent::GetLocalMin() const
{
	return LocalMin;
}

_vec3 CollisionComponent::GetLocalMax() const
{
	return LocalMax;
}

void CollisionComponent::GetWorldAABB(_vec3* worldMin, _vec3* worldMax)
{
	auto transform = owner->GetComponent<TransformComponent>();

	_vec3 pos = transform->GetWorldPosition() + Offset;
	_matrix worldMat;
	D3DXMatrixTranslation(&worldMat, pos.x, pos.y, pos.z);

	D3DXVec3TransformCoord(worldMin, &LocalMin, &worldMat);
	D3DXVec3TransformCoord(worldMax, &LocalMax, &worldMat);
}

void CollisionComponent::GetWorldAABB(_vec3* worldMin, _vec3* worldMax, _vec3 pos)
{
	_matrix worldMat;
	D3DXMatrixTranslation(&worldMat, pos.x, pos.y, pos.z);

	D3DXVec3TransformCoord(worldMin, &LocalMin, &worldMat);
	D3DXVec3TransformCoord(worldMax, &LocalMax, &worldMat);
}

void CollisionComponent::GetWorldX(_float* minX, _float* maxX)
{
	_vec3 min, max;
	GetWorldAABB(&min, &max);

	*minX = min.x;
	*maxX = max.x;
}

_bool CollisionComponent::RayIntersectAABB(Ray ray, HitInfo& hit)
{
	if (Collision_Layer & LAYER_PLAYER) return false;

	auto transform = owner->GetComponent<TransformComponent>();
	_matrix worldmat = transform->GetTranslateMatrix();	//스케일, 회전 적용 x
	
	_vec3 worldMin;
	_vec3 worldMax;

	D3DXVec3TransformCoord(&worldMin, &LocalMin, &worldmat);
	D3DXVec3TransformCoord(&worldMax, &LocalMax, &worldmat);

	_float tmin = (worldMin.x - ray.Origin.x) / ray.Direction.x;
	_float tmax = (worldMax.x - ray.Origin.x) / ray.Direction.x;
	if (tmin > tmax)
		std::swap(tmin, tmax);

	_float tymin = (worldMin.y - ray.Origin.y) / ray.Direction.y;
	_float tymax = (worldMax.y - ray.Origin.y) / ray.Direction.y;
	if (tymin > tymax)
		std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax))
		return false;

	tmin = (std::max)(tmin, tymin);
	tmax = (std::min)(tmax, tymax);

	_float tzmin = (worldMin.z - ray.Origin.z) / ray.Direction.z;
	_float tzmax = (worldMax.z - ray.Origin.z) / ray.Direction.z;
	if (tzmin > tzmax)
		std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax))
		return false;

	tmin = (std::max)(tmin, tzmin);
	tmax = (std::min)(tmax, tzmax);

	_float distance	 = (tmin >= 0.f) ? tmin : tmax;	//매개변수로 들어온 hit.distance보다 현재 계산 값이 짧으면 구조체 갱신

	if (distance < hit.Distance)
	{
		hit.IsHit = true;
		hit.Distance = (tmin >= 0.f) ? tmin : tmax;
		hit.Position = ray.Origin + ray.Direction * hit.Distance;
		hit.Component = this;
	}

	return true;
}

_bool CollisionComponent::CanCollision(CollisionComponent* other)
{
	return Collision_Layer & other->GetMask();
}

_bool CollisionComponent::CheckAABBCollision(CollisionComponent* other)
{
	_vec3 aPos = owner->GetComponent<TransformComponent>()->GetWorldPosition() + Offset;
	_vec3 bPos = other->owner->GetComponent<TransformComponent>()->GetWorldPosition() + Offset;

	_matrix aTransMat;
	_matrix bTransMat;
	D3DXMatrixTranslation(&aTransMat, aPos.x, aPos.y, aPos.z);
	D3DXMatrixTranslation(&bTransMat, bPos.x, bPos.y, bPos.z);

	_vec3 aMin, aMax;
	_vec3 bMin, bMax;

	D3DXVec3TransformCoord(&aMin, &LocalMin, &aTransMat);
	D3DXVec3TransformCoord(&aMax, &LocalMax, &aTransMat);
	D3DXVec3TransformCoord(&bMin, &other->LocalMin, &bTransMat);
	D3DXVec3TransformCoord(&bMax, &other->LocalMax, &bTransMat);

	if (aMin.x > bMax.x || aMax.x < bMin.x) return false;
	if (aMin.y > bMax.y || aMax.y < bMin.y) return false;
	if (aMin.z > bMax.z || aMax.z < bMin.z) return false;

	return true;
}

void CollisionComponent::ResolveAABBColiision(Object* other)
{
	auto transform = owner->GetComponent<TransformComponent>();
	auto otherTransform = other->GetComponent<TransformComponent>();

	_vec3 aPos = transform->GetPosition() + Offset;
	_vec3 bPos = otherTransform->GetPosition() + Offset;

	_matrix aTransMat;
	_matrix bTransMat;
	D3DXMatrixTranslation(&aTransMat, aPos.x, aPos.y, aPos.z);
	D3DXMatrixTranslation(&bTransMat, bPos.x, bPos.y, bPos.z);

	_vec3 aCenter = transform->GetPosition();
	_vec3 bCenter = otherTransform->GetPosition();

	_vec3 aMin, aMax;
	_vec3 bMin = other->GetComponent<CollisionComponent>()->GetLocalMin();
	_vec3 bMax = other->GetComponent<CollisionComponent>()->GetLocalMax();

	D3DXVec3TransformCoord(&aMin, &LocalMin, &aTransMat);
	D3DXVec3TransformCoord(&aMax, &LocalMax, &aTransMat);
	D3DXVec3TransformCoord(&bMin, &bMin, &bTransMat);
	D3DXVec3TransformCoord(&bMax, &bMax, &bTransMat);

	_float overlapX = (std::min)(aMax.x, bMax.x) - (std::max)(aMin.x, bMin.x);
	_float overlapY = (std::min)(aMax.y, bMax.y) - (std::max)(aMin.y, bMin.y);
	_float overlapZ = (std::min)(aMax.z, bMax.z) - (std::max)(aMin.z, bMin.z);

	if (overlapX > 0.0001f && overlapY > 0.0001f && overlapZ > 0.0001f)
	{
		if (overlapX <= overlapY && overlapX <= overlapZ)
			transform->Translate((aCenter.x >= bCenter.x) ? overlapX : -overlapX, 0.f, 0.f);

		else if (overlapY <= overlapX && overlapY <= overlapZ)
			transform->Translate(0.f, (aCenter.y >= bCenter.y) ? overlapY : -overlapY, 0.f);

		else if (overlapZ <= overlapX && overlapZ <= overlapY)
			transform->Translate(0.f, 0.f, (aCenter.z >= bCenter.z) ? overlapZ : -overlapZ);
	}
}

void CollisionComponent::OnCollisionEnter(CollisionComponent* other)
{
	if(onEnter)
		onEnter(other->GetOwner());
}

void CollisionComponent::OnCollisionStay(CollisionComponent* other)
{
	if (onStay)
		onStay(other->GetOwner());
}

void CollisionComponent::OnCollisionExit(CollisionComponent*other)
{
	//if (onExit)
	//	onExit(other->GetOwner());
}

void CollisionComponent::Render()
{
	auto device = GraphicDevice::GetInstance()->GetDevice();
	_vec3 pos = owner->GetComponent<TransformComponent>()->GetWorldPosition() + Offset;
	_matrix worldMat;
	D3DXMatrixTranslation(&worldMat, pos.x, pos.y, pos.z);

	device->SetTransform(D3DTS_WORLD, &worldMat);
	BoundingBox->DrawSubset(0);
}

void CollisionComponent::Free()
{
	ObjectComponent::Free();
	Safe_Release(BoundingBox);
}
