#pragma once
#include "BaseCharacter.h"
class Player : public BaseCharacter
{   
    //playerState
    enum class ePlayerState {
        IDLE,
        WALK,
        ROLL,
        ATTACK,
        SHOOT,
        DEAD
    };
public: //basic func
    static Player* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;
private:
    Player(ObjectManager* owner, ObjectType objType);
    virtual ~Player();
    void Free()override;
private: //func
    void KeyInput(_float dt);
    
    void CheckStateAttack(_float dt);
    void CheckStateWalk(_float dt);
    void CheckStateRoll(_float dt);
    void CheckStateIdle(_float dt);
    void CheckDead();

    void UpdateIdle(_float dt);
    void UpdateWalk(_float dt);
    void UpdateRoll(_float dt);
    void UpdateAttack(_float dt);
    void UpdateShoot(_float dt);
    void UpdateDead(_float dt);

    void MovePlayer(_vec3 moveVec);
    void RotatePlayer(_vec3 rotateVec);
    void SaveStartRotation();

    void FixCursorToCenter();
    void PickingTerrain();
    _vec3 MatrixToEulerAngles(const _matrix& mat);
private: //member variable
    ePlayerState State = ePlayerState::IDLE;
    float WalkTime = 0.f;
    float RollTime = 0.f;
    float AttackTime = 0.f;
    float DeadTime = 0.f;

    std::unordered_map<std::string, _vec3> StartRotations;
    float Speed = 10.f;

    _vec3 PlayerDirection = { 0.f, 0.f , 0.f };
    _vec3 destinationPos = { 0.f, 0.f, 0.f };
    _vec3 AttackDirection = { 0.f, 0.f, 0.f };
};
