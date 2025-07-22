#pragma once
#include "CameraActor.h"

class WayPointCam :
    public CameraActor
{
private:
    WayPointCam(ObjectManager* owner);
    virtual ~WayPointCam();

public:
    static WayPointCam* Create(ObjectManager* owner);
    HRESULT Ready_Object()override;
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void AddWaypoint(WayPoint point) { waypoints.push_back(point); }
    void SetDuration(_float duration) { this->duration = duration; }
    void Start();
    void Stop();
    _bool IsEnd()const { return isEnd; }
    void Clear() { waypoints.clear(); start = false; }

    _float* GetDuration() { return &duration; }
    std::vector<WayPoint>& GetWaypoints() { return waypoints; }
    void SetWaypoints(std::vector<WayPoint>& points) { waypoints = points; }
    void RemoveWaypoint(int idx);

    void SaveWaypoints(const std::string& filePath);
    void LoadWaypoints(const std::string& filePath);
private:
    _vec3 CatmullRom(_vec3 p0, _vec3 p1, _vec3 p2, _vec3 p3, _float t);
    void Free()override;

    std::vector<WayPoint> waypoints;
    _float timer = 0.f;
    _float duration = 0.f;
    _float timePerSection = 0.f;

    _bool start = false;
    _bool isEnd = false;
};

