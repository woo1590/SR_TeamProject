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
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    return S_OK;
}
void Player::Update(_float dt)
{
    BaseCharacter::Update(dt);

    PickingTerrain();
    KeyInput(dt); // Set State by key
    switch (State) { // Update by State
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
    auto input = EngineCore::GetInstance()->GetInputSystem();
    auto curScene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto mainCam = curScene->GetCameraManager()->GetMainCamera();
    auto collision = curScene->GetCollisionSystem();
    const float yOffset = 3.7f;

    if (input->IsKeyPressed(RBUTTON))
    {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        
        if (hit.IsHit)
        {
            if (State == ePlayerState::IDLE) {
                State = ePlayerState::WALK;
                WalkTime = 0.f;
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetPosition();
            destinationPos = hit.Position;
            destinationPos.y += yOffset;
            PlayerDirection = destinationPos - curPos;
        }
    }
}
void Player::MovePlayer(_vec3 moveVec)
{
    //move player with move Value Vector
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition()+moveVec);
}
void Player::RotatePlayer(_vec3 rotateVec)
{
    //rotate player with rotate Value Vector
    auto transform = GetComponent<TransformComponent>();
    transform->SetRotate(transform->GetRotate()+rotateVec);
}
void Player::SaveStartRotation()
{
    //save all bones rotation value
    StartRotations["Player"] = GetComponent<TransformComponent>()->GetRotate();
    StartRotations["Body"] = Bones["Body"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["Head"] = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["LHand"] = Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["RHand"] = Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["LLeg"] = Bones["LLeg"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["RLeg"] = Bones["RLeg"]->GetComponent<TransformComponent>()->GetRotate();
}
void Player::FixCursorToCenter()
{
    //get window handle
    HWND hWnd = EngineCore::GetInstance()->GetWindowHandle();
    // get center
    RECT rcClient;
    GetClientRect(hWnd, &rcClient);
    POINT ptCenter;
    ptCenter.x = (rcClient.right - rcClient.left) / 2;
    ptCenter.y = (rcClient.bottom - rcClient.top) / 2;
    // client to screen
    ClientToScreen(hWnd, &ptCenter);
    // current cursor position
    POINT ptMouse;
    GetCursorPos(&ptMouse);
    // distance > limit -> move cursor to center
    const int iThreshold = 5; // limit distance
    if (abs(ptMouse.x - ptCenter.x) > iThreshold || abs(ptMouse.y - ptCenter.y) > iThreshold) {
        SetCursorPos(ptCenter.x, ptCenter.y);
    }
}
void Player::UpdateIdle(_float dt)
{
    auto SmoothReset = [dt](TransformComponent* pTransform) {
        _vec3 vCurrentRot = pTransform->GetRotate();
        _vec3 vTargetRot = { 0.f, 0.f, 0.f };
        float fSpeed = 5.f; // speed to target rotation;

        _vec3 vLerpedRot = vCurrentRot + (vTargetRot - vCurrentRot) * dt * fSpeed;
        pTransform->SetRotate(vLerpedRot);
    };

    SmoothReset(Bones["Head"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["LLeg"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RLeg"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["LHand"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RHand"]->GetComponent<TransformComponent>());
}
void Player::UpdateWalk(_float dt) {
    //walk time
    WalkTime += dt;
    //walk speed
    float walkSpeed = 10.f;
    //walk angle
    float fAngle = sinf(WalkTime * walkSpeed);
    SetRotation({ fAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "RLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "LHand");
    SetRotation({ fAngle, 0.f, 0.f }, "RHand");
    //normalize direction vector
    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);
    // move
    _vec3 moveVec = {
        vDir.x * Speed * Scale * dt,
        0,
        vDir.z * Speed * Scale * dt
    };
    MovePlayer(moveVec);
    // rotate duration
    const float fRotateDuration = 0.05f;
    // direction to angle
    float TargetAngle = atan2f(vDir.x, vDir.z);
    // current direction
    _vec3 vCurRot = GetComponent<TransformComponent>()->GetRotate();
    //angle normalize for fast spin
    auto NormalizeAngle = [](float angle) {
        while (angle > D3DX_PI) angle -= D3DX_PI * 2.f;
        while (angle < -D3DX_PI) angle += D3DX_PI * 2.f;
        return angle;
    };
    float curAngle = NormalizeAngle(vCurRot.y);
    float deltaAngle = NormalizeAngle(TargetAngle - curAngle);
    //get rotation value
    float rotValue = deltaAngle * (dt / fRotateDuration);
    //do rotate
    RotatePlayer({ 0.f, rotValue, 0.f });

    auto transform = GetComponent<TransformComponent>();
    auto curPos = transform->GetPosition();
    auto posGap = curPos - destinationPos;
    auto distance = sqrtf(posGap.x * posGap.x + posGap.z * posGap.z);
    if (distance < 1.f) {
        State = ePlayerState::IDLE;
        transform->SetPosition(curPos.x, destinationPos.y, curPos.z);
    }
}
void Player::UpdateRoll(_float dt)
{
    //roll duration
    const float fRollDuration = 0.5f;
    //roll time
    RollTime += dt;
    //roll progress
    float fProgress = RollTime / fRollDuration;
    //progress value clamping
    fProgress = std::clamp(fProgress, 0.f, 1.f);
    //set lerp ratio
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
    //lerp rotate value
    auto LerpRot = [](const _vec3& start, const _vec3& offset, float ratio) {
        return start + offset * ratio;
    };
    //set rotate
    SetRotation(LerpRot(StartRotations["Head"], { 1.f, 0.f, 0.f }, fLerpRatio), "Head");
    SetRotation(LerpRot(StartRotations["LLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "LLeg");
    SetRotation(LerpRot(StartRotations["RLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "RLeg");
    SetRotation(LerpRot(StartRotations["LHand"], { -2.f, 0.f, 0.f }, fLerpRatio), "LHand");
    SetRotation(LerpRot(StartRotations["RHand"], { -2.f, 0.f, 0.f }, fLerpRatio), "RHand");
    //set current angle with duration
    float fTotalRollAngle = D3DX_PI * 2.f;
    float fCurrentAngle = fTotalRollAngle * dt / fRollDuration;
    //control y value while rolling
    float fYOffset = sinf(fProgress * D3DX_PI) * -15.f;
    auto transform = GetComponent<TransformComponent>();
    _vec3 vCurPos = transform->GetPosition();
    vCurPos.y = fYOffset;
    transform->SetPosition(vCurPos);
    //
    _vec3 moveVec = { 0.f, 0.f, 2 * Speed * Scale * dt };
    MovePlayer(moveVec);

    _vec3 rotateVec = { fCurrentAngle, 0.f, 0.f };
    RotatePlayer(rotateVec);

    if (RollTime >= fRollDuration) {
        RollTime = 0.f;
        State = ePlayerState::WALK;

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
    AttackTime += dt;
    float fProgress = AttackTime / fAttackDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);
    //lerp
    auto Lerp = [](const _vec3& a, const _vec3& b, float t) {
        return a + (b - a) * t;
    };
    auto LerpRot = [&](const _vec3& startDeg, const _vec3& endDeg, float ratio) {
        _vec3 startRad = {
            D3DXToRadian(startDeg.x),
            D3DXToRadian(startDeg.y),
            D3DXToRadian(startDeg.z)
        };
        _vec3 endRad = {
            D3DXToRadian(endDeg.x),
            D3DXToRadian(endDeg.y),
            D3DXToRadian(endDeg.z)
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
        StartRotations["RHand"],
        { -210.f, 10.f, -30.f },
        { -150.f, 10.f, -30.f },
        { -60.f, 10.f, 60.f }
    );
    ApplyPhasedRotation("LHand",
        StartRotations["LHand"],
        { -30.f, 10.f, -10.f },
        { 0.f, 0.f, -10.f },
        { 30.f, -10.f, -10.f }
    );
    ApplyPhasedRotation("LLeg", StartRotations["LLeg"],
        { -10.f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { 10.f, 0.f, 0.f }
    );
    ApplyPhasedRotation("RLeg", StartRotations["RLeg"],
        { 10.f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { -10.f, 0.f, 0.f }
    );
    //rotatePlayer
    _vec3 vPlayerStartRot = StartRotations["Player"];
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
    if (AttackTime >= fAttackDuration) {
        AttackTime = 0.f;
        State = ePlayerState::WALK;
    }
}
void Player::KeyInput(_float dt)
{
    CheckStateAttack(dt);
    CheckStateRoll(dt);
    //CheckStateWalk(dt);
    //CheckStateIdle(dt);
}
void Player::CheckStateAttack(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static KEY keyAttack = LBUTTON;

    //PRESS LBUTTON -> ATTACK
    if (input->IsKeyPressed(keyAttack)) {
        switch (State) {
        case ePlayerState::IDLE: case ePlayerState::WALK:
            State = ePlayerState::ATTACK;
            AttackTime = 0.f;
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
        switch (State) {
        case ePlayerState::IDLE:
            WalkTime = 0.f;
            State = ePlayerState::WALK;
            break;
        }
    }
    if (input->IsKeyPressed(keyWalkFront)) {
        PlayerDirection += { 0.f,0.f,1.f };
    }
    if (input->IsKeyPressed(keyWalkBack)) {
        PlayerDirection += { 0.f, 0.f, -1.f };
    }
    if (input->IsKeyPressed(keyWalkLeft)) {
        PlayerDirection += { -1.f, 0.f, 0.f };
    }
    if (input->IsKeyPressed(keyWalkRight)) {
        PlayerDirection += { 1.f, 0.f, 0.f };
    }

    if (input->IsKeyRelease(keyWalkFront)) {
        PlayerDirection -= { 0.f, 0.f, 1.f };
    }
    if (input->IsKeyRelease(keyWalkBack)) {
        PlayerDirection -= { 0.f, 0.f, -1.f };
    }
    if (input->IsKeyRelease(keyWalkLeft)) {
        PlayerDirection -= { -1.f, 0.f, 0.f };
    }
    if (input->IsKeyRelease(keyWalkRight)) {
        PlayerDirection -= { 1.f, 0.f, 0.f };
    }
}
void Player::CheckStateRoll(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static KEY keyRoll = B;

    //ISMOVING TRUE && PRESS SPACE -> ROLL
    if (input->IsKeyPressed(keyRoll)) {
        switch (State) {
        case ePlayerState::WALK:
            State = ePlayerState::ROLL;
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
        switch (State) {
        case ePlayerState::WALK:
            State = ePlayerState::IDLE;
            PlayerDirection = { 0.f,0.f,0.f };
            break;
        }
    }
}
