#pragma once
#include "BaseCharacter.h"
#include "Item.h"
#include "QuestTextObj.h"

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
        DEAD,
        REVIVE,
        INTERACT,
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
    enum class ePlayerShootType : int
    {
        ARROW,
        FIREWORK,
        CROSSBOW
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

    void EquipItem(ItemType itemType);
    void UnEquipItem(ItemType itemType);
    Object* GetBone(std::string boneName);
    ePlayerState GetPlayerState();
    void RevivePlayer();
    _vec3 GetAttackDirection();
    void ChangeShootType();
    bool IsMovingToAttack();
    void SetStatikkMode(_bool _statikk);
    _bool IsStatikkMode();
    _float GetAttackDelay();

    void SetOwner(ObjectManager* owner);

    _vec3 GetDir() const
    {
        _vec3 dir = PlayerDirection;
        D3DXVec3Normalize(&dir, &dir);
        return dir;
    }
    _bool IsCharge();
    std::unordered_map<string, Object*> GetBones();
private:
    Player(ObjectManager* owner, ObjectType objType);
    virtual ~Player();
    void Free()override;

    void KeyInput(_float dt);

    void PickingTerrain();
    void CheckStateRoll(_float dt);
    void CheckDead();
    void CheckJump();
    void CheckTargetDead();
    void CheckSkill();
    void ResetWalkTimer();

    void InteractWithNPC(Object* obj);

    void UpdateIdle(_float dt);
    void UpdateWalk(_float dt);
    void UpdateRoll(_float dt);
    void UpdateAttack(_float dt);
    void UpdateShoot(_float dt);
    void UpdateDead(_float dt);
    void UpdateRevive(_float dt); 

    void SaveStartRotation();
    void SetUpSwordFirstAttackPhaseRotations();
    void SetUpSwordSecondAttackPhaseRotations();
    void SetUpSwordLastAttackPhaseRotations();
    void SetUpShootPhaseRotations();
    void SetUpDeadPhaseRotations();
    void SetUpRevivePhaseRotations();
    void SetUpSpearFirstAttackPhaseRotations();
    void SetUpSpearSecondAttackPhaseRotations();
    void SetUpSpearLastAttackPhaseRotations();
    void SetAttackTypeNext();
    void SetUpIdleRotations();
    void UpdateNewIdleRotations();

    void CamRotTest(_float dt);

    _vec3 MatrixToEulerAngles(const _matrix& mat);
    void OnCollisionStay(Object* other);
    void IdleSmoothing(_float dt, ePlayerBone bone);
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
    _float WrapAngle(_float fAngle);
private:
    ePlayerState State = ePlayerState::IDLE;
    ePlayerAttackType attackType = ePlayerAttackType::FIRST;
    ePlayerShootType shootType = ePlayerShootType::ARROW;

    _float IdleTime = 0.f;
    _float IdleSmoothingSpeed = 5.f;

    _float WalkTime = 0.f;
    const _float WalkSwingSpeed = 10.f;

    _float RollTime = 0.f;
    const _float RollDuration = 0.5f;

    _float AttackTime = 0.f;
    const _float AttackDuration = 0.4f;
    const _float ShootDuration = 0.6f;

    _float chargedTime = 0.f;
    const _float MaxChargeTime = 1.f;

    _float DeadTime = 0.f;
    const _float DeadDuration = 1.0f;

    _float comboTime = 0.f;
    const _float comboLimit = 0.5f;

    _float ReviveTime = 0.f;
    const _float reviveDuration = 1.f;

    std::unordered_map<std::string, _vec3> StartRotations;
    _vec3 PlayerDirection = { 0.f, 0.f , 0.f };
    _vec3 DestinationPos = { 0.f, 0.f, 0.f };
    _vec3 AttackDirection = { 0.f, 0.f, 0.f };

    _float MeleeRange = 8.f;
    bool moveToAttack = false;
    Object* moveToObject = nullptr;

    std::unordered_map<std::pair<int, int>, PhaseRotation, PairHash> PhaseRotations;
    std::unordered_map<std::string, _vec3> itemBaseRotOffset;

    _float walkEffectTimer = 0.f;
    std::unordered_map<std::pair<int, int>, _vec3, PairHash> idleRot;

    const float jumpPower = 15.f;

    _bool StatikkMode = false;

    std::string soundBefore = "WalkOnDefault2";
    // ----------------------
    const float interactRange = 5.f;
    bool moveToInteract = false;

    _bool onCharge = false;
    _bool onChargeEnd = false;
};
