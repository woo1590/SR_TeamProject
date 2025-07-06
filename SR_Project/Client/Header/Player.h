#pragma once
#include "BaseCharacter.h"
#include "Item.h"
class Player : public BaseCharacter
{   
public:
    enum class ePlayerState {
        IDLE,
        WALK,
        ROLL,
        ATTACK,
        SHOOT,
        DEAD
    };
public:
    static Player* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
private:
    Player(ObjectManager* owner, ObjectType objType);
    virtual ~Player();
    void Free()override;

    void KeyInput(_float dt);

    void PickingTerrain();
    void CheckStateRoll(_float dt);
    void CheckDead();

    void UpdateIdle(_float dt);
    void UpdateWalk(_float dt);
    void UpdateRoll(_float dt);
    void UpdateAttack(_float dt);
    void UpdateShoot(_float dt);
    void UpdateDead(_float dt);

    void SaveStartRotation();
    void EquipItem(Item::ItemType itemType);
    void UnEquipItem(Item::ItemType itemType);

    _vec3 MatrixToEulerAngles(const _matrix& mat);
    void OnCollisionStay(Object* other);
    void IdleSmoothing(_float dt, std::string bone);
    float NormalizeAngle(_float angle);
    _vec3 OffsetLerp(const _vec3& start, const _vec3& offset, float ratio);
    _vec3 DegToRadLerp(const _vec3& startDeg, const _vec3& endDeg, float ratio);
    _vec3 GetPhasedRotation(float fProgress, vector<float>& phaseVec, vector<_vec3>& destinations);
    void ApplyPhasedRotation(const std::string& name, float fProgress, vector<float>& phaseVec, vector<_vec3>& destinations);
private:
    ePlayerState State = ePlayerState::IDLE;
    float WalkTime = 0.f;
    float RollTime = 0.f;
    float AttackTime = 0.f;
    float DeadTime = 0.f;

    std::unordered_map<std::string, _vec3> StartRotations;
    _vec3 PlayerDirection = { 0.f, 0.f , 0.f };
    _vec3 destinationPos = { 0.f, 0.f, 0.f };
    _vec3 AttackDirection = { 0.f, 0.f, 0.f };
};
