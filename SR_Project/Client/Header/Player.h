#pragma once
#include "BaseCharacter.h"
class Player : public BaseCharacter
{   
    enum class ePlayerState {
        IDLE,
        WALK,
        ROLL
    };
private:
    Player(ObjectManager* owner, ObjectType objType);
    virtual ~Player();

public:
    static Player* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

private:
    void Free()override;

private:
    //우석 테스트 코드
    void PickingTerrain();

    void MovePlayer(_vec3 moveVec);
    void RotatePlayer(_vec3 rotateVec);
    void KeyInput(_float dt);

    void UpdateIdle(_float dt);
    void UpdateWalk(_float dt);
    void UpdateRoll(_float dt);
private:
    ePlayerState m_eState = ePlayerState::IDLE;
    float m_fWalkTime = 0.f;
    float m_fRollTime = 0.f;

    std::unordered_map<std::string, _vec3> m_mapStartRotations;
    float m_fSpeed = 100.f;
};
    