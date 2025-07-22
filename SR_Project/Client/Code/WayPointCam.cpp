#include "pch.h"
#include "WayPointCam.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

WayPointCam::WayPointCam(ObjectManager* owner)
	:CameraActor(owner,ObjectType::Camera)
{
}

WayPointCam::~WayPointCam()
{
}

WayPointCam* WayPointCam::Create(ObjectManager* owner)
{
	WayPointCam* Instance = new WayPointCam(owner);

	if (FAILED(Instance->Ready_Object()))
	{
		Safe_Release(Instance);
	}

	return Instance;
}

HRESULT WayPointCam::Ready_Object()
{
	CameraActor::Ready_Object();

	return S_OK;
}

void WayPointCam::Update(_float dt)
{
	Object::Update(dt);
	
	if (start)
	{
		if (timer >= duration)
			isEnd = true;

		auto transform = GetComponent<TransformComponent>();

		int currSection = timer / timePerSection;

		currSection = std::clamp(currSection, 0, int(waypoints.size() - 2));

		_float t = (timer - currSection * timePerSection) / timePerSection;
		

		WayPoint p0 = waypoints[(std::max)(0, currSection - 1)];
		WayPoint p1 = waypoints[currSection];
		WayPoint p2 = waypoints[(std::min)((int)waypoints.size() - 1, currSection + 1)];
		WayPoint p3 = waypoints[(std::min)((int)waypoints.size() - 1, currSection + 2)];

		_vec3 newPos = CatmullRom(p0.position, p1.position, p2.position, p3.position, t);
		_vec3 newLook = CatmullRom(p0.lookDir, p1.lookDir, p2.lookDir, p3.lookDir, t);

		D3DXVec3Normalize(&newLook, &newLook);
		transform->SetPosition(newPos);
		transform->SetForward(newLook);
		
		timer += dt;
	}
}

void WayPointCam::Late_Update(_float dt)
{
	Object::Late_Update(dt);
}

void WayPointCam::Start()
{
	start = true;
	timePerSection = duration / (waypoints.size() - 1);
	timer = 0.f;
}

void WayPointCam::Stop()
{
	start = false;
}

void WayPointCam::RemoveWaypoint(int idx)
{
	waypoints.erase(waypoints.begin() + idx);
}

void WayPointCam::SaveWaypoints(const std::string& filePath)
{
	std::ofstream outFile(filePath, std::ios::out | std::ios::binary);

	if (!outFile.is_open()) {

		return;
	}

	size_t numWaypoints = waypoints.size();
	outFile.write(reinterpret_cast<const char*>(&numWaypoints), sizeof(size_t));

	outFile.write(reinterpret_cast<const char*>(waypoints.data()), numWaypoints * sizeof(WayPoint));

	outFile.close();
}

void WayPointCam::LoadWaypoints(const std::string& filePath)
{
	std::ifstream inFile(filePath, std::ios::in | std::ios::binary);

	if (!inFile.is_open()) {
		return;
	}

	size_t numWaypoints = 0;
	inFile.read(reinterpret_cast<char*>(&numWaypoints), sizeof(size_t));

	if (numWaypoints == 0) {
		waypoints.clear();
		inFile.close();
		return;
	}

	waypoints.resize(numWaypoints);

	inFile.read(reinterpret_cast<char*>(waypoints.data()), numWaypoints * sizeof(WayPoint));

	inFile.close();
}

_vec3 WayPointCam::CatmullRom(_vec3 p0, _vec3 p1, _vec3 p2, _vec3 p3, _float t)
{
	_float t2 = t * t;
	_float t3 = t2 * t;

	_vec3 res = 0.5f * (
		(2.0f * p1) +
		(-p0 + p2) * t +
		(2.0f * p0 - 5.0f * p1 + 4.0f * p2 - p3) * t2 +
		(-p0 + 3.0f * p1 - 3.0f * p2 + p3) * t3);

	return res;
}

void WayPointCam::Free()
{
	Object::Free();
}
