#pragma once
#include "BaseCharacter.h"
#include "Item.h"
class Player : public BaseCharacter
{
public:
    enum class ePlayerState : int
    {
        IDLE,
        WALK,
        ROLL,
        ATTACK,
        SHOOT,
        DEAD
    };
    enum class ePlayerBone :int
    {
        BODY,
        HEAD,
        LARM,
        RARM,
        LLEG,
        RLEG,
        LHAND,
        RHAND
    };
    enum class ePlayerAttackType : int
    {
        FIRST,
        SECOND,
        LAST,
        COUNT
    };
    struct PhaseRotation
    {
        string name;
        vector<_vec3> destinations;
        vector<float> phaseVec;

        PhaseRotation() = default;
        PhaseRotation(const vector<_vec3>& vecRot, const vector<float>& vecPhase)
            : destinations(vecRot), phaseVec(vecPhase)
        {
        }
    };
    struct PairHash
    {
        size_t operator()(const pair<int, int>& pr) const
        {
            return hash<int>()(pr.first) ^ (hash<int>()(pr.second) << 1);
        } 
    };
public:
    static Player* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void EquipItem(Item::ItemType itemType);
    void UnEquipItem(Item::ItemType itemType);
    Object* GetBone(std::string boneName);
    ePlayerState GetPlayerState();
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
    void SetUpFirstAttackPhaseRotations();
    void SetUpSecondAttackPhaseRotations();
    void SetUpLastAttackPhaseRotations();
    void SetUpShootPhaseRotations();
    void SetUpDeadPhaseRotations();
    void SetAttackTypeNext();

    _vec3 MatrixToEulerAngles(const _matrix& mat);
    void OnCollisionStay(Object* other);
    void IdleSmoothing(_float dt, std::string bone);
    float NormalizeAngle(_float angle);
    float OffsetLerp(const _float& start, const _float& offset, float ratio);
    _vec3 OffsetLerp(const _vec3& start, const _vec3& offset, float ratio);
    _vec3 DegToRadLerp(const _vec3& startDeg, const _vec3& endDeg, float ratio);
    _vec3 GetPhasedRotation(float fProgress, PhaseRotation& phaseRot);
    void ApplyPhasedRotation(float fProgress, PhaseRotation& phaseRot);
    void SetPhaseRotations(const ePlayerState& state, const ePlayerBone& bone, PhaseRotation& phaseRot);
    PhaseRotation& GetPhaseRotations(const ePlayerState& state, const ePlayerBone& bone);
    float GetStringAngleX(const string& frontBack = "", const string& upDown = "", bool clockwise = true, float offset = 0.f);
    float GetStringAngleZ(const string& leftRight = "", const string& upDown = "", bool clockwise = true, float offset = 0.f);
    float GetStringAngleY(const string& leftRight = "", const string& frontBack = "", bool clockwise = true, float offset = 0.f);
private:
    ePlayerState State = ePlayerState::IDLE;
    ePlayerAttackType attackType = ePlayerAttackType::FIRST;

    float IdleTime = 0.f;
    float IdleSmoothingSpeed = 5.f;

    float WalkTime = 0.f;
    const float WalkSwingSpeed = 10.f;

    float RollTime = 0.f;
    const float RollDuration = 0.5f;

    float AttackTime = 0.f;
    const float AttackDuration = 0.3f;
    const float ShootDuration = 0.6f;

    float DeadTime = 0.f;
    const float DeadDuration = 1.0f;

    float comboTime = 0.f;
    const float comboLimit = 0.5f;

    std::unordered_map<std::string, _vec3> StartRotations;
    _vec3 PlayerDirection = { 0.f, 0.f , 0.f };
    _vec3 DestinationPos = { 0.f, 0.f, 0.f };
    _vec3 AttackDirection = { 0.f, 0.f, 0.f };

    _float SwordRange = 4.f;
    bool moveToAttack = false;
    Object* moveToObject = nullptr;

    std::unordered_map<std::pair<int, int>, PhaseRotation, PairHash> PhaseRotations;
    std::unordered_map<std::string, _vec3> itemBaseRotOffset;
};
