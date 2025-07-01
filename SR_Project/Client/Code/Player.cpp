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
#include "EngineCore.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "CameraManager.h"

Player::Player(ObjectManager* owner, ObjectType objType) : BaseCharacter(owner, objType)
{
}

Player::~Player()
{
}

Player* Player::Create(ObjectManager* owner, ObjectType objType)
{
    Player* Instance = new Player(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}
HRESULT Player::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);

    auto transform = AddComponent<TransformComponent>();

    auto info = AddComponent<PlayerInfoComponent>();

    SetScale(0.1f);

    SetMaterial(L"playerBody_Mtrl","Body");
    SetMaterial(L"playerHead_Mtrl","Head");
    SetMaterial(L"playerLeftArm_Mtrl","LHand");
    SetMaterial(L"playerRightArm_Mtrl","RHand");
    SetMaterial(L"playerLeftLeg_Mtrl", "LLeg");
    SetMaterial(L"playerRightLeg_Mtrl", "RLeg");
    
    for (auto& pair : Bones) {
        auto pBoneTransform = pair.second->GetComponent<TransformComponent>();
        pBoneTransform->SetParent(transform);
    }

    return S_OK;
}
void Player::Update(_float dt)
{
    BaseCharacter::Update(dt);

    PickingTerrain();
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
        break;
    case ePlayerState::ATTACK:
        UpdateAttack(dt);
        break;
    }
    //FixCursorToCenter();
}
void Player::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}
void Player::Free()
{
    Object::Free();
}

void Player::PickingTerrain()
{
    auto Input = EngineCore::GetInstance()->GetInputSystem();
    auto cam = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetCameraManager()->GetMainCamera();
    auto collision = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetCollisionSystem();

    if (Input->IsKeyPressed(LBUTTON))
    {
        Ray ray = cam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        
        if (hit.IsHit)
        {
            hit.Position.y += 3.f;
            GetComponent<TransformComponent>()->SetPosition(hit.Position);
        }
    }
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
void Player::SaveStartRotation()
{
    m_mapStartRotations["Player"] = GetComponent<TransformComponent>()->GetRotate();
    m_mapStartRotations["Body"] = Bones["Body"]->GetComponent<TransformComponent>()->GetRotate();
    m_mapStartRotations["Head"] = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
    m_mapStartRotations["LHand"] = Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate();
    m_mapStartRotations["RHand"] = Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate();
    m_mapStartRotations["LLeg"] = Bones["LLeg"]->GetComponent<TransformComponent>()->GetRotate();
    m_mapStartRotations["RLeg"] = Bones["RLeg"]->GetComponent<TransformComponent>()->GetRotate();
}
void Player::FixCursorToCenter()
{
    HWND hWnd = EngineCore::GetInstance()->GetWindowHandle();

    // 클라이언트 중앙 좌표 계산
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);

    POINT ptCenter;
    ptCenter.x = (rcClient.right - rcClient.left) / 2;
    ptCenter.y = (rcClient.bottom - rcClient.top) / 2;

    // 클라이언트 좌표를 화면 좌표로 변환
    ClientToScreen(hWnd, &ptCenter);

    // 현재 마우스 위치
    POINT ptMouse;
    GetCursorPos(&ptMouse);

    // 일정 거리 이상 벗어나면 중앙으로 강제 이동
    const int iThreshold = 5; // 임계 거리 (픽셀)
    if (abs(ptMouse.x - ptCenter.x) > iThreshold || abs(ptMouse.y - ptCenter.y) > iThreshold) {
        SetCursorPos(ptCenter.x, ptCenter.y);
    }
}
void Player::UpdateIdle(_float dt)
{
    auto SmoothReset = [dt](TransformComponent* pTransform) {
        _vec3 vCurrentRot = pTransform->GetRotate();
        _vec3 vTargetRot = { 0.f, 0.f, 0.f };
        float fSpeed = 5.f; // 회복 속도 조절용

        _vec3 vLerpedRot = vCurrentRot + (vTargetRot - vCurrentRot) * dt * fSpeed;
        pTransform->SetRotate(vLerpedRot);
    };

    SmoothReset(Bones["Head"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["LLeg"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RLeg"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["LHand"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RHand"]->GetComponent<TransformComponent>());
}
void Player::UpdateWalk(_float dt)
{
    m_fWalkTime += dt;

    float fAngle = sinf(m_fWalkTime*10.f);
    SetRotation( { fAngle, 0.f, 0.f }, "LLeg" );
    SetRotation( { -fAngle, 0.f, 0.f }, "RLeg");

    SetRotation( { -fAngle, 0.f, 0.f }, "LHand");
    SetRotation( { fAngle, 0.f, 0.f }, "RHand");

    _vec3 dirVec;
    D3DXVec3Normalize(&dirVec, &m_playerDirection);
    _vec3 moveVec = { dirVec.x * m_fSpeed * Scale * dt, dirVec.y * m_fSpeed * Scale * dt, dirVec.z * m_fSpeed * Scale * dt };
    MovePlayer(moveVec);

    const float changeDirDuration = 0.05f;
    float fTargetYawRad = atan2f(dirVec.x, dirVec.z);
    _vec3 vCurRot = GetComponent<TransformComponent>()->GetRotate();
    float fCurYaw = vCurRot.y;
    float fDeltaYaw = fTargetYawRad - fCurYaw;
    if (fDeltaYaw > D3DX_PI)
        fDeltaYaw -= D3DX_PI * 2.f;
    else if (fDeltaYaw < -D3DX_PI)
        fDeltaYaw += D3DX_PI * 2.f;
    float fYawStep = fDeltaYaw * (dt / changeDirDuration);
    _vec3 rotateVec = { 0.f, fYawStep, 0.f };
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
    vCurPos.y = fYOffset;
    transform->SetPosition(vCurPos);

    _vec3 moveVec = { 0.f, 0.f, 2 * m_fSpeed * Scale * dt };
    MovePlayer(moveVec);

    _vec3 rotateVec = { fCurrentAngle, 0.f, 0.f };
    RotatePlayer(rotateVec);

    if (m_fRollTime >= fRollDuration) {
        m_fRollTime = 0.f;
        m_eState = ePlayerState::WALK;

        _vec3 vCurRot = transform->GetRotate();
        vCurRot.x = 0.f;
        vCurRot.z = 0.f;
        transform->SetRotate(vCurRot);
    }
}
void Player::UpdateAttack(_float dt) {
    // Rot : x- back / x+ front /  z- left / z+ right / y- lookRight / y+ lookLeft
    //AttackDelay
    const float fAttackDuration = 0.4f;
    m_fAttackTime += dt;
    float fProgress = m_fAttackTime / fAttackDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);
    //change angle
    auto DegreeToRadian = [](float deg) {
        return deg * D3DX_PI / 180.f;
    };
    //lerp
    auto Lerp = [](const _vec3& a, const _vec3& b, float t) {
        return a + (b - a) * t;
    };
    auto LerpRot = [&](const _vec3& startDeg, const _vec3& endDeg, float ratio) {
        _vec3 startRad = {
            DegreeToRadian(startDeg.x),
            DegreeToRadian(startDeg.y),
            DegreeToRadian(startDeg.z)
        };
        _vec3 endRad = {
            DegreeToRadian(endDeg.x),
            DegreeToRadian(endDeg.y),
            DegreeToRadian(endDeg.z)
        };
        return Lerp(startRad, endRad, ratio);
    };
    // motion phase
    float fPhase1 = 0.1f;
    float fPhase2 = 0.4f;
    float fPhase3 = 0.9f;
    float fPhase4 = 1.0f;
    //rotate function
    float t = 0.f;
    _vec3 vCurrentRot;
    auto ApplyPhasedRotation = [&](const std::string& name,
        _vec3 vStart,
        _vec3 vKey1,
        _vec3 vKey2,
        _vec3 vKey3) {
            if (fProgress < fPhase1) {
                t = fProgress / fPhase1;
                vCurrentRot = LerpRot(vStart, vKey1, t);
            }
            else if (fProgress < fPhase2) {
                t = (fProgress - fPhase1) / (fPhase2 - fPhase1);
                vCurrentRot = LerpRot(vKey1, vKey2, t);
            }
            else if (fProgress < fPhase3) {
                t = (fProgress - fPhase2) / (fPhase3 - fPhase2);
                vCurrentRot = LerpRot(vKey2, vKey3, t);
            }
            else {
                t = (fProgress - fPhase3) / (fPhase4 - fPhase3);
                vCurrentRot = LerpRot(vKey3, vStart, t);
            }

            SetRotation(vCurrentRot, name);
    };
    //rotateBones
    ApplyPhasedRotation("RHand",
        m_mapStartRotations["RHand"],
        { -210.f, 10.f, -30.f },
        { -150.f, 0.f, -30.f },
        { -60.f, -10.f, 60.f }
    );
    ApplyPhasedRotation("LHand",
        m_mapStartRotations["LHand"],
        { -30.f, 10.f, -10.f },
        { 0.f, 0.f, -10.f },
        { 30.f, -10.f, -10.f }
    );
    ApplyPhasedRotation("LLeg", m_mapStartRotations["LLeg"],
        { -10.f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { 10.f, 0.f, 0.f }
    );
    ApplyPhasedRotation("RLeg", m_mapStartRotations["RLeg"],
        { 10.f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { -10.f, 0.f, 0.f }
    );
    //rotatePlayer
    _vec3 vPlayerStartRot = m_mapStartRotations["Player"];
    _vec3 vTwist1 = { 0.f, 30.f, 0.f };
    _vec3 vTwist2 = { 0.f, 0.f, 0.f };
    _vec3 vTwist3 = { 0.f, -60.f, 0.f };
    if (fProgress < fPhase1) {
        t = fProgress / fPhase1;
        vCurrentRot = LerpRot(vPlayerStartRot, vTwist1, t);
    }
    else if (fProgress < fPhase2) {
        t = (fProgress - fPhase1) / (fPhase2 - fPhase1);
        vCurrentRot = LerpRot(vTwist1, vTwist2, t);
    }
    else if (fProgress < fPhase3) {
        t = (fProgress - fPhase2) / (fPhase3 - fPhase2);
        vCurrentRot = LerpRot(vTwist2, vTwist3, t);
    }
    else {
        t = (fProgress - fPhase3) / (fPhase4 - fPhase3);
        vCurrentRot = LerpRot(vTwist3, vPlayerStartRot, t);
    }
    GetComponent<TransformComponent>()->SetRotate(vCurrentRot);
    //Exit State
    if (m_fAttackTime >= fAttackDuration) {
        m_fAttackTime = 0.f;
        m_eState = ePlayerState::WALK;
    }
}

void Player::KeyInput(_float dt)
{
    CheckStateAttack(dt);
    CheckStateRoll(dt);
    CheckStateWalk(dt);
    CheckStateIdle(dt);
}

void Player::CheckStateAttack(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static KEY keyAttack = LBUTTON;

    //PRESS LBUTTON -> ATTACK
    if (input->IsKeyPressed(keyAttack)) {
        switch (m_eState) {
        case ePlayerState::IDLE: case ePlayerState::WALK:
            m_eState = ePlayerState::ATTACK;
            m_fAttackTime = 0.f;
            SaveStartRotation();
            break;
        }
    }
}

void Player::CheckStateWalk(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static KEY keyWalkFront = UP;
    static KEY keyWalkBack = DOWN;
    static KEY keyWalkLeft = LEFT;
    static KEY keyWalkRight = RIGHT;

    if (input->IsKeyDown(keyWalkFront) ||
        input->IsKeyDown(keyWalkBack) ||
        input->IsKeyDown(keyWalkLeft) ||
        input->IsKeyDown(keyWalkRight)) {
        switch (m_eState) {
        case ePlayerState::IDLE:
            m_eState = ePlayerState::WALK;
            break;
        }
    }
    if (input->IsKeyPressed(keyWalkFront)) {
        m_playerDirection += { 0.f,0.f,1.f };
    }
    if (input->IsKeyPressed(keyWalkBack)) {
        m_playerDirection += { 0.f, 0.f, -1.f };
    }
    if (input->IsKeyPressed(keyWalkLeft)) {
        m_playerDirection += { -1.f, 0.f, 0.f };
    }
    if (input->IsKeyPressed(keyWalkRight)) {
        m_playerDirection += { 1.f, 0.f, 0.f };
    }

    if (input->IsKeyRelease(keyWalkFront)) {
        m_playerDirection -= { 0.f, 0.f, 1.f };
    }
    if (input->IsKeyRelease(keyWalkBack)) {
        m_playerDirection -= { 0.f, 0.f, -1.f };
    }
    if (input->IsKeyRelease(keyWalkLeft)) {
        m_playerDirection -= { -1.f, 0.f, 0.f };
    }
    if (input->IsKeyRelease(keyWalkRight)) {
        m_playerDirection -= { 1.f, 0.f, 0.f };
    }
}

void Player::CheckStateRoll(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static KEY keyRoll = B;

    //ISMOVING TRUE && PRESS SPACE -> ROLL
    if (input->IsKeyPressed(keyRoll)) {
        switch (m_eState) {
        case ePlayerState::WALK:
            m_eState = ePlayerState::ROLL;
            break;
        }
    }
}

void Player::CheckStateIdle(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static KEY keyWalkFront = UP;
    static KEY keyWalkBack = DOWN;
    static KEY keyWalkLeft = LEFT;
    static KEY keyWalkRight = RIGHT;

    if (input->IsKeyRelease(keyWalkFront) &&
        input->IsKeyRelease(keyWalkBack)  &&
        input->IsKeyRelease(keyWalkLeft)  &&
        input->IsKeyRelease(keyWalkRight) ) {
        switch (m_eState) {
        case ePlayerState::WALK:
            m_eState = ePlayerState::IDLE;
            m_playerDirection = { 0.f,0.f,0.f };
            SaveStartRotation();
            break;
        }
    }
}
