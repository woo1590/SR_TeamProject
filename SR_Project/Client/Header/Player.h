#pragma once
#include "BaseCharacter.h"
class Player : public BaseCharacter
{   
    enum class ePlayerState {
        IDLE,
        WALK,
        ROLL,
        ATTACK
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

    void CheckStateAttack(_float dt);
    void CheckStateWalk(_float dt);
    void CheckStateRoll(_float dt);
    void CheckStateIdle(_float dt);

    void UpdateIdle(_float dt);
    void UpdateWalk(_float dt);
    void UpdateRoll(_float dt);
    void UpdateAttack(_float dt);

    void MovePlayer(_vec3 moveVec);
    void RotatePlayer(_vec3 rotateVec);
    void SaveStartRotation();

    void FixCursorToCenter();
private:
    ePlayerState m_eState = ePlayerState::IDLE;
    float m_fWalkTime = 0.f;
    float m_fRollTime = 0.f;
    float m_fAttackTime = 0.f;

    std::unordered_map<std::string, _vec3> m_mapStartRotations;
    float m_fSpeed = 100.f;

    _vec3 m_playerDirection = { 0.f, 0.f , 0.f };
};