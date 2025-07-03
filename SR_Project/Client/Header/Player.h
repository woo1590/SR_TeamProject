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
    
    void CheckStateRoll(_float dt);
    void CheckDead();

    void UpdateIdle(_float dt);
    void UpdateWalk(_float dt);
    void UpdateRoll(_float dt);
    void UpdateAttack(_float dt);
    void UpdateShoot(_float dt);
    void UpdateDead(_float dt);

    void SaveStartRotation();
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
    float yOffset = 3.7f;

    _vec3 PlayerDirection = { 0.f, 0.f , 0.f };
    _vec3 destinationPos = { 0.f, 0.f, 0.f };
    _vec3 AttackDirection = { 0.f, 0.f, 0.f };
};
