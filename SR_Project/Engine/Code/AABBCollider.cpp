#include "EnginePCH.h"
#include "AABBCollider.h"
#include "OBBCollider.h"
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
	OBB obbA;
	OBB obbB = other->GetWorldOBB();

	AABB worldAABB = GetWorldAABB();

	obbA.center = worldAABB.center;
	obbA.half = worldAABB.half;
	D3DXMatrixIdentity(&obbA.axis);

	_vec3 a_axis[3] = {
		_vec3(obbA.axis._11, obbA.axis._12, obbA.axis._13),
		_vec3(obbA.axis._21, obbA.axis._22, obbA.axis._23),
		_vec3(obbA.axis._31, obbA.axis._32, obbA.axis._33)
	};
	_vec3 b_axis[3] = {
		_vec3(obbB.axis._11, obbB.axis._12, obbB.axis._13),
		_vec3(obbB.axis._21, obbB.axis._22, obbB.axis._23),
		_vec3(obbB.axis._31, obbB.axis._32, obbB.axis._33)
	};
	_vec3 a_half = obbA.half;
	_vec3 b_half = obbB.half;

	_vec3 T = obbB.center - obbA.center;

	_matrix R;
	_matrix R_abs;
	_float epsilon = 1e-6f;

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			R.m[i][j] = D3DXVec3Dot(&a_axis[i], &b_axis[j]);
			R_abs.m[i][j] = std::abs(R.m[i][j]) + epsilon;
		}
	}

	for (int i = 0; i < 3; ++i) {
		float ra = a_half[i];
		float rb = b_half.x * R_abs.m[i][0] + b_half.y * R_abs.m[i][1] + b_half.z * R_abs.m[i][2];
		float dist = std::abs(D3DXVec3Dot(&T, &a_axis[i]));

		if (dist > ra + rb) {
			return false;
		}
	}

	for (int i = 0; i < 3; ++i) {
		float ra = a_half.x * R_abs.m[0][i] + a_half.y * R_abs.m[1][i] + a_half.z * R_abs.m[2][i];
		float rb = b_half[i];
		float dist = std::abs(D3DXVec3Dot(&T, &b_axis[i]));

		if (dist > ra + rb) {
			return false;
		}
	}

	// Axis: A.x X B.x
	float ra = a_half.y * R_abs._31 + a_half.z * R_abs._21;
	float rb = b_half.y * R_abs._13 + b_half.z * R_abs._12;
	float dist = std::abs(D3DXVec3Dot(&T, &a_axis[2]) * R._21 - D3DXVec3Dot(&T, &a_axis[1]) * R._31);
	if (dist > ra + rb) return false;

	// Axis: A.x X B.y
	ra = a_half.y * R_abs._32 + a_half.z * R_abs._22;
	rb = b_half.x * R_abs._13 + b_half.z * R_abs._11;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[2]) * R._22 - D3DXVec3Dot(&T, &a_axis[1]) * R._32);
	if (dist > ra + rb) return false;

	// Axis: A.x X B.z
	ra = a_half.y * R_abs._33 + a_half.z * R_abs._23;
	rb = b_half.x * R_abs._12 + b_half.y * R_abs._11;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[2]) * R._23 - D3DXVec3Dot(&T, &a_axis[1]) * R._33);
	if (dist > ra + rb) return false;

	// Axis: A.y X B.x
	ra = a_half.x * R_abs._31 + a_half.z * R_abs._11;
	rb = b_half.y * R_abs._23 + b_half.z * R_abs._22;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[0]) * R._31 - D3DXVec3Dot(&T, &a_axis[2]) * R._11);
	if (dist > ra + rb) return false;

	// Axis: A.y X B.y
	ra = a_half.x * R_abs._32 + a_half.z * R_abs._12;
	rb = b_half.x * R_abs._23 + b_half.z * R_abs._21;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[0]) * R._32 - D3DXVec3Dot(&T, &a_axis[2]) * R._12);
	if (dist > ra + rb) return false;

	// Axis: A.y X B.z
	ra = a_half.x * R_abs._33 + a_half.z * R_abs._13;
	rb = b_half.x * R_abs._22 + b_half.y * R_abs._21;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[0]) * R._33 - D3DXVec3Dot(&T, &a_axis[2]) * R._13);
	if (dist > ra + rb) return false;

	// Axis: A.z X B.x
	ra = a_half.x * R_abs._21 + a_half.y * R_abs._11;
	rb = b_half.y * R_abs._33 + b_half.z * R_abs._32;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[1]) * R._11 - D3DXVec3Dot(&T, &a_axis[0]) * R._21);
	if (dist > ra + rb) return false;

	// Axis: A.z X B.y
	ra = a_half.x * R_abs._22 + a_half.y * R_abs._12;
	rb = b_half.x * R_abs._33 + b_half.z * R_abs._31;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[1]) * R._12 - D3DXVec3Dot(&T, &a_axis[0]) * R._22);
	if (dist > ra + rb) return false;

	// Axis: A.z X B.z
	ra = a_half.x * R_abs._23 + a_half.y * R_abs._13;
	rb = b_half.x * R_abs._32 + b_half.y * R_abs._31;
	dist = std::abs(D3DXVec3Dot(&T, &a_axis[1]) * R._13 - D3DXVec3Dot(&T, &a_axis[0]) * R._23);
	if (dist > ra + rb) return false;

	return true;

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
