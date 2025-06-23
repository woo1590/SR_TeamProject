#pragma once
#include "Engine_Typedef.h"
#include <cmath>

namespace math
{
	constexpr float PI = 3.141592f;
	constexpr float TWO_PI = (PI * 2.f);

	inline float NormalizeAngle(float radian)
	{
		if (radian <= -PI)
			radian += TWO_PI;
		else if (radian > PI)
			radian -= TWO_PI;

		return radian;
	}

	inline float Length(const _vec3& a)
	{
		return sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	}

	inline _vec3 Normailize(const _vec3& a) 
	{
		float length = Length(a);

		return _vec3(a.x / length, a.y / length, a.z / length);
	}

	inline float Dot(const _vec3& a, const _vec3& b)
	{
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}

	inline _vec3 Cross(const _vec3& a, const _vec3& b)
	{
		return _vec3(a.y * b.z - a.z * b.y, 
					a.z * b.x - a.x * b.z,
					a.x * b.y - a.y * b.x);
	}

	inline _vec3 TransformCoord(const _matrix& mat, const _vec3& v)
	{
		_vec3 res;
		D3DXVec3TransformCoord(&res, &v, &mat);

		return res;
	}

	inline _vec3 TransformNormal(const _matrix& mat, const _vec3& v)
	{
		_vec3 res;
		D3DXVec3TransformNormal(&res, &v, &mat);
		
		return res;
	}

	inline _matrix MatirxTranslation(float x, float y, float z)
	{
		_matrix res;
		D3DXMatrixTranslation(&res, x, y, x);

		return res;
	}

	inline _matrix MatrixTranslation(const _vec3& v)
	{
		_matrix res;
		D3DXMatrixTranslation(&res, v.x, v.y, v.z);

		return res;
	}

	inline _matrix LookAtLH(const _vec3& eye, const _vec3& at, const _vec3& up)
	{
		_vec3 n = Normailize(at-eye);		//z√‡
		_vec3 v = Normailize(Cross(up, n));	//x√‡
		_vec3 u = Cross(n,v);				//y√‡

		_matrix transMat;
		_matrix rotMat;
		_matrix view;

		D3DXMatrixTranslation(&transMat, -eye.x, -eye.y, -eye.z);
		D3DXMatrixIdentity(&rotMat);

		rotMat._11 = v.x;	rotMat._12 = u.x;	rotMat._13 = n.x;
		rotMat._21 = v.y;	rotMat._22 = u.y;	rotMat._23 = n.y;
		rotMat._31 = v.z;	rotMat._32 = u.z;	rotMat._33 = n.z;

		view = transMat * rotMat;

		return view;
	}


	inline _matrix PerspectiveFovLH(float fov, float aspect, float nearZ, float farZ)
	{
		_matrix proj;
		D3DXMatrixIdentity(&proj);

		float scaleY = 1.f / tan(fov/2.f);
		float scaleX = scaleY * (1.f / aspect);
		
		proj._11 = scaleX;
		proj._22 = scaleY;
		proj._33 = farZ / (farZ - nearZ);
		proj._34 = 1.f;
		proj._43 = -farZ * nearZ / (farZ - nearZ);
		proj._44 = 0.f;

		return proj;
	}

	inline _float Lerp(_float a, _float b, _float t)
	{
		return a - (a * t) + (b * t);
	}
}

#include "Math.inl"