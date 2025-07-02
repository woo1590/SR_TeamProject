#include "pch.h"
#include "Player.h"
#include "Player.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InputSystem.h"
#include "PlayerInfoComponent.h"

#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"
#include "InfoComponent.h"

Player::Player(ObjectManager* owner, ObjectType objType) : BaseCharacter(owner, objType){}
Player::~Player(){}

Player* Player::Create(ObjectManager* owner, ObjectType objType){
    Player* Instance = new Player(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))){
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Player::Ready_Object(ObjectManager* owner, ObjectType objType){
    BaseCharacter::Ready_Object(owner, objType);
    //components
    auto collision = AddComponent<CollisionComponent>();
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    auto info = AddComponent<PlayerInfoComponent>();
    //PlayerScale
    SetScale(1.f);
    //PlayerTexture
    SetMaterial(L"playerBody_Mtrl","Body");
    SetMaterial(L"playerHead_Mtrl","Head");
    SetMaterial(L"playerLeftArm_Mtrl","LHand");
    SetMaterial(L"playerRightArm_Mtrl","RHand");
    SetMaterial(L"playerLeftLeg_Mtrl", "LLeg");
    SetMaterial(L"playerRightLeg_Mtrl", "RLeg");

    auto transform = AddComponent<TransformComponent>();
    auto playerInfo = AddComponent<InfoComponent<PlayerInfo>>();

    SetScale(1.f);

    SetMaterial(L"playerBody_Mtrl","Body");
    Bones["Body"]->GetComponent<TransformComponent>()->SetScale(8.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["Body"]->GetComponent<TransformComponent>()->SetPosition(0.f, 0.f, 0.f);

    SetMaterial(L"playerHead_Mtrl","Head");
    Bones["Head"]->GetComponent<TransformComponent>()->SetScale(8.f * Scale, 8.f * Scale, 8.f * Scale);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPosition(0.f, 20.f * Scale, 0.f);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -8.f * Scale, 0.0f));
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    SetMaterial(L"playerLeftArm_Mtrl","LHand");
    Bones["LHand"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPosition(-11.f * Scale, 0.f, 0.f);
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -6.f * Scale, 0.0f));
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    
    SetMaterial(L"playerRightArm_Mtrl","RHand");
    Bones["RHand"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPosition(11.f * Scale, 0.f, 0.f);
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -6.f * Scale, 0.0f));
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    SetMaterial(L"playerLeftLeg_Mtrl", "LLeg");
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPosition(-4.f * Scale, -24.f * Scale, 0.f);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.f * Scale, 0.0f));
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    SetMaterial(L"playerRightLeg_Mtrl", "RLeg");
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPosition(4.f * Scale, -24.f * Scale, 0.f);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.f * Scale, 0.0f));
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    for (auto& pair : Bones) {
        auto pBoneTransform = pair.second->GetComponent<TransformComponent>();
        pBoneTransform->SetParent(transform);
    }

    return S_OK;
}

void Player::Update(_float dt)
{
    BaseCharacter::Update(dt);

    KeyInput(dt);
    switch (m_eState) {
    case ePlayerState::IDLE:
        UpdateIdle(dt);
        break;
    case ePlayerState::WALK:
        UpdateWalk(dt);
        break;
    case ePlayerState::ROLL:
        UpdateRoll(dt);
    }
}

void Player::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Player::Free()
{
    Object::Free();
}

void Player::MovePlayer(_vec3 moveVec)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition()+moveVec);
}

void Player::RotatePlayer(_vec3 rotateVec)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetRotate(transform->GetRotate()+rotateVec);
}

void Player::UpdateIdle(_float dt)
{
}

void Player::UpdateWalk(_float dt)
{
    m_fWalkTime += dt;

    float fAngle = sinf(m_fWalkTime*10.f);
    SetRotation( { fAngle, 0.f, 0.f }, "LLeg" );
    SetRotation( { -fAngle, 0.f, 0.f }, "RLeg");

    SetRotation( { -fAngle, 0.f, 0.f }, "LHand");
    SetRotation( { fAngle, 0.f, 0.f }, "RHand");

    _vec3 moveVec = { 0.f * dt, 0.f * dt, m_fSpeed * Scale * dt };
    MovePlayer(moveVec);
    _vec3 rotateVec = { 0.f * dt, 0.f * dt, 0.f * dt };
    RotatePlayer(rotateVec);
}

void Player::UpdateRoll(_float dt)
{
    const float fRollDuration = 0.5f;
    m_fRollTime += dt;

    float fProgress = m_fRollTime / fRollDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);

    float fLerpRatio = 0.f;
    if (fProgress <= 0.2f) {
        fLerpRatio = fProgress / 0.2f;
    }
    else if (fProgress >= 0.8f) {
        fLerpRatio = (1.f - fProgress) / 0.2f;
    }
    else {
        fLerpRatio = 1.f;
    }

    auto LerpRot = [](const _vec3& start, const _vec3& offset, float ratio) {
        return start + offset * ratio;
    };

    SetRotation(LerpRot(m_mapStartRotations["Head"], { 1.f, 0.f, 0.f }, fLerpRatio), "Head");
    SetRotation(LerpRot(m_mapStartRotations["LLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "LLeg");
    SetRotation(LerpRot(m_mapStartRotations["RLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "RLeg");
    SetRotation(LerpRot(m_mapStartRotations["LHand"], { -2.f, 0.f, 0.f }, fLerpRatio), "LHand");
    SetRotation(LerpRot(m_mapStartRotations["RHand"], { -2.f, 0.f, 0.f }, fLerpRatio), "RHand");

    float fTotalRollAngle = D3DX_PI * 2.f;
    float fCurrentAngle = fTotalRollAngle * dt / fRollDuration;

    float fYOffset = sinf(fProgress * D3DX_PI) * -15.f;
    auto transform = GetComponent<TransformComponent>();
    _vec3 vCurPos = transform->GetPosition();
    vCurPos.y = fYOffset; // �׻� ���ذ� (0.f)���� ���
    transform->SetPosition(vCurPos);

    _vec3 moveVec = { 0.f, 0.f, 3 * m_fSpeed * Scale * dt };
    MovePlayer(moveVec);

    _vec3 rotateVec = { fCurrentAngle, 0.f, 0.f };
    RotatePlayer(rotateVec);

    if (m_fRollTime >= fRollDuration) {
        m_fRollTime = 0.f;
        m_eState = ePlayerState::WALK;
    }
}

void Player::KeyInput(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    if (input->IsKeyPressed(Z)) {
        switch (m_eState) {
        case ePlayerState::IDLE:
            m_eState = ePlayerState::WALK;
            break;
        }
    }
    if (input->IsKeyRelease(Z)) {
        switch (m_eState) {
        case ePlayerState::WALK:
            m_eState = ePlayerState::IDLE;
            break;
        }
    }
    if (input->IsKeyPressed(X)) {
        switch (m_eState) {
        case ePlayerState::WALK:
            m_eState = ePlayerState::ROLL;
            m_mapStartRotations["Head"] = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
            m_mapStartRotations["LLeg"] = Bones["LLeg"]->GetComponent<TransformComponent>()->GetRotate();
            m_mapStartRotations["RLeg"] = Bones["RLeg"]->GetComponent<TransformComponent>()->GetRotate();
            m_mapStartRotations["LHand"] = Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate();
            m_mapStartRotations["RHand"] = Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate();
            break;
        }
    }
}
