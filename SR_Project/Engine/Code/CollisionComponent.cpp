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
#include "Collider.h"

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

	collider->Update();
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

void CollisionComponent::SetSize(_vec3 size)
{
	collider->SetSize(size);
}

_vec3 CollisionComponent::GetSize() const
{
	return collider->GetSize();
}

void CollisionComponent::SetLayer(CollisionLayer layer)
{
	Collision_Layer = layer;
}

void CollisionComponent::SetMask(_ulong mask)
{
	Collision_Mask = mask;
}

_bool CollisionComponent::RayIntersectAABB(Ray ray, HitInfo& hit)
{
	if (Collision_Layer & LAYER_PLAYER) return false;

	AABB worldAABB = collider->GetWorldAABB();
	
	_vec3 worldMin = worldAABB.center - worldAABB.half;
	_vec3 worldMax = worldAABB.center + worldAABB.half;

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

_bool CollisionComponent::CheckCollision(CollisionComponent* other)
{
	Collider* a = collider;
	Collider* b = other->GetCollider();

	return collider->CheckCollision(b);
}

void CollisionComponent::ResolveAABBColiision(Object* other)
{
	
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
	if (!DebugMode)
		return;

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
