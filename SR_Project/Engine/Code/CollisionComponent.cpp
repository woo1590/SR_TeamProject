#include "EnginePCH.h"
#include "CollisionComponent.h"
#include "EngineCore.h"
#include "SceneManager.h"
#include "GraphicDevice.h"
#include "CollisionSystem.h"
#include "Object.h"
#include "Scene.h"

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

	EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetCollisionSystem()->RegisterCollision(this);
	return S_OK;
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
}

bool CollisionComponent::RayIntersectAABB(Ray ray, HitInfo& hit)
{
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



void CollisionComponent::Free()
{
	ObjectComponent::Free();
}
