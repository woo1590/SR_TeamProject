#include "EnginePCH.h"
#include "OBBCollider.h"
#include "GraphicDevice.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "Object.h"

OBBCollider::OBBCollider(CollisionComponent* owner)
	:Collider(owner)
{
	auto device = GraphicDevice::GetInstance()->GetDevice();
	D3DXCreateBox(device, localOBB.half.x * 2.f, localOBB.half.y, localOBB.half.z * 2.f, &debugBox, nullptr);
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
	OBB obbA = GetWorldOBB();
	OBB obbB;

	AABB worldAABB = other->GetWorldAABB();

	obbB.center = worldAABB.center;
	obbB.half = worldAABB.half;
	D3DXMatrixIdentity(&obbB.axis); 

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

bool OBBCollider::CollisionOBB(OBBCollider* other)
{
	OBB obbA = this->GetWorldOBB();
	OBB obbB = other->GetWorldOBB();

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

void OBBCollider::SetSize(_vec3 size)
{
	localOBB.half = size * 0.5f;

	if (debugBox)
		Safe_Release(debugBox);

	auto device = GraphicDevice::GetInstance()->GetDevice();
	D3DXCreateBox(device, localOBB.half.x * 2.f, localOBB.half.y * 2.f, localOBB.half.z * 2.f, &debugBox, nullptr);
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

void OBBCollider::Render()
{
	OBB worldOBB = GetWorldOBB();

	_matrix worldMat;

	D3DXMatrixTranslation(&worldMat, worldOBB.center.x, worldOBB.center.y, worldOBB.center.z);
	worldMat = worldOBB.axis * worldMat;

	auto device = GraphicDevice::GetInstance()->GetDevice();

	device->SetTransform(D3DTS_WORLD, &worldMat);
	debugBox->DrawSubset(0);
}

void OBBCollider::Free()
{
}


