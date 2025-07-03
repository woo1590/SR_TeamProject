#include "pch.h"
#include "Player.h"
#include "Player.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InputSystem.h"
#include "InfoComponent.h"

#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
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
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_DEFAULT);
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetCollisionStay([this](Object* other) {this->OnCollisionStay(other);});

    auto physics = AddComponent<PhysicsComponent>();

    //PlayerScale
    SetScale(1.f);
    //PlayerTexture
    SetMaterial(L"playerBody_Mtrl","Body");
    SetMaterial(L"playerHead_Mtrl","Head");
    SetMaterial(L"playerLeftArm_Mtrl","LArm");
    SetMaterial(L"playerRightArm_Mtrl","RArm");
    SetMaterial(L"playerLeftLeg_Mtrl", "LLeg");
    SetMaterial(L"playerRightLeg_Mtrl", "RLeg");

    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    auto playerInfo = AddComponent<InfoComponent<PlayerInfo>>();

    return S_OK;
}
void Player::Update(_float dt)
{
    BaseCharacter::Update(dt);

    
    PickingTerrain();
    KeyInput(dt); // Set State by key
    CheckDead();
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
    case ePlayerState::SHOOT:
        UpdateShoot(dt);
        break;
    case ePlayerState::DEAD:
        UpdateDead(dt);
        break;
    }

    //Phyisics Test
    auto transform = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();

    Ray downRay{ transform->GetPosition(),_vec3(0.f,-1.f,0.f) };
    HitInfo hit = GetScene()->GetCollisionSystem()->Raycast(downRay);

    if (hit.IsHit && hit.Distance <= 4.f)
    {
        physics->SetGround(true);
    }
    else
    {
        _vec3 velocity = physics->GetVelocity();
        transform->Translate(velocity * dt);
    }
    ///////////////////////////////////////////////////////////////////////////
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

    if (input->IsKeyPressed(Z)) {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        if (hit.IsHit)
        {
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK) {
                State = ePlayerState::ATTACK;
                AttackTime = 0.f;
                SaveStartRotation();
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetPosition();
            auto attackPos = hit.Position;
            attackPos.y += yOffset;
            AttackDirection = attackPos - curPos;
        }
    }

    if (input->IsKeyPressed(X)) {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        if (hit.IsHit)
        {
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK) {
                State = ePlayerState::SHOOT;
                AttackTime = 0.f;
                SaveStartRotation();
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetPosition();
            auto attackPos = hit.Position;
            attackPos.y += yOffset;
            AttackDirection = attackPos - curPos;
        }
    }
}

void Player::OnCollisionStay(Object* other)
{
    ObjectType otherType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    switch (otherType)
    {
    case Engine::ObjectType::Monster:
        break;
    case Engine::ObjectType::StaticBlock:
        collision->ResolveAABBColiision(other);
        break;
    default:
        break;
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
    StartRotations["LArm"] = Bones["LArm"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["RArm"] = Bones["RArm"]->GetComponent<TransformComponent>()->GetRotate();
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

    SmoothReset(Bones["LLeg"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RLeg"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["LArm"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RArm"]->GetComponent<TransformComponent>());
}
void Player::UpdateWalk(_float dt) {
    //walk time
    WalkTime += dt;
    //walk speed
    float walkSwingSpeed = 10.f;
    //walk angle
    float fAngle = sinf(WalkTime * walkSwingSpeed);
    SetRotation({ fAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "RLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "LArm");
    SetRotation({ fAngle, 0.f, 0.f }, "RArm");
    //normalize direction vector
    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);
    // move
    float Speed = GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().speed;
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
    if (distance < 0.5f) {
        State = ePlayerState::IDLE;
        transform->SetPosition(curPos.x, destinationPos.y, curPos.z);
    }
}
void Player::UpdateRoll(_float dt)
{
    auto transform = GetComponent<TransformComponent>();
    //roll duration
    const float fRollDuration = 0.5f;
    //roll speed value
    const float fRollSpeed = Speed * 2.f;
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
    //direction vector
    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);
    //set rotate
    SetRotation(LerpRot(StartRotations["Head"], { 1, 0.f, 0.f }, fLerpRatio), "Head");
    SetRotation(LerpRot(StartRotations["LLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "LLeg");
    SetRotation(LerpRot(StartRotations["RLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "RLeg");
    SetRotation(LerpRot(StartRotations["LArm"], { -2.f, 0.f, 0.f }, fLerpRatio), "LArm");
    SetRotation(LerpRot(StartRotations["RArm"], { -2.f, 0.f, 0.f }, fLerpRatio), "RArm");
    //control y value while rolling
    static float fStartY = 0.f;
    if (RollTime == dt)
        fStartY = transform->GetPosition().y;

    const float fAmplitudePhase1 = 0.2f * Scale;
    const float fAmplitudePhase2 = 2.8f * Scale;
    float t = fProgress;
    float fYOffset = 0.f;

    if (t <= 0.3f) {
        float fLocalT = t / 0.3f; // 0~1
        fYOffset = sinf(fLocalT * D3DX_PI) * fAmplitudePhase1; // 0 ~ π
    }
    else {
        float fLocalT = (t - 0.3f) / 0.7f; // 0~1
        fYOffset = sinf(D3DX_PI + fLocalT * D3DX_PI) * fAmplitudePhase2; // π ~ 2π
    }
    _vec3 vCurPos = transform->GetPosition();
    vCurPos.y = fStartY + fYOffset;
    transform->SetPosition(vCurPos);
    //movePlayer
    _vec3 moveVec = {
        vDir.x * fRollSpeed * Scale * dt,
        0,
        vDir.z * fRollSpeed* Scale * dt
    };
    MovePlayer(moveVec);
    //set current angle with duration
    float fTotalRollAngle = D3DX_PI * 2.f ;
    float fCurrentAngle = fTotalRollAngle * RollTime / fRollDuration;
    //rotate with right vector
    D3DXVec3Normalize(&moveVec, &moveVec);
    transform->SetForward(moveVec);
    _vec3 vAxis = transform->GetRight();
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    RotatePlayer(rotateVec);
    //finish roll
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
    ApplyPhasedRotation("RArm",
        StartRotations["RArm"],
        { -210.f, 10.f, -30.f },
        { -150.f, 10.f, -30.f },
        { -60.f, 10.f, 60.f }
    );
    ApplyPhasedRotation("LArm",
        StartRotations["LArm"],
        { -30.f, 10.f, -10.f },
        { 0.f, 0.f, -10.f },
        { 30.f, -10.f, -10.f }
    );
    ApplyPhasedRotation("LLeg", StartRotations["LLeg"],
        { 10.f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { -10.f, 0.f, 0.f }
    );
    ApplyPhasedRotation("RLeg", StartRotations["RLeg"],
        { -10.f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { 10.f, 0.f, 0.f }
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
    float fCurrentAngle = D3DXToRadian(vCurrentRot.y);
    //rotate with up vector
    auto transform = GetComponent<TransformComponent>();
    _vec3 attackVec = AttackDirection;
    attackVec.y = 0.f;
    D3DXVec3Normalize(&attackVec, &attackVec);
    transform->SetForward(attackVec);
    _vec3 vAxis = transform->GetUp();
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    RotatePlayer(rotateVec);
    //Exit State
    if (AttackTime >= fAttackDuration) {
        AttackTime = 0.f;
        State = ePlayerState::WALK;
    }
}
void Player::UpdateShoot(_float dt) {
    const float fAttackDuration = 0.6f;
    AttackTime += dt;
    float fProgress = AttackTime / fAttackDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);

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

    float fPhase1 = 0.15f;
    float fPhase2 = 0.4f;
    float fPhase3 = 0.9f;
    float fPhase4 = 1.0f;
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

    // 🎯 활 쏘는 모션 구성
    ApplyPhasedRotation("LHand",
        StartRotations["LHand"],
        { -90.f, 30.f, 0.f },
        { -90.f, 30.f, 0.f },
        { -0.f, 0.f, 0.f }
    );
    ApplyPhasedRotation("RHand",
        StartRotations["RHand"],
        { -90.f, -60.f, 0.f },
        { -90.f, -60.f, 0.f },
        { -0.f, 0.f, 0.f }
    );
    ApplyPhasedRotation("Player",
        StartRotations["Player"],
        { 0.f, 10.f, 0.f },
        { 0.f, 15.f, 0.f },
        { 0.f, 20.f, 0.f }
    );
    ApplyPhasedRotation("LLeg", StartRotations["LLeg"],
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    );
    ApplyPhasedRotation("RLeg", StartRotations["RLeg"],
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    );

    // 🎯 시선 방향 고정 + 트위스트 회전
    auto transform = GetComponent<TransformComponent>();
    _vec3 vStartRot = StartRotations["Player"];
    _vec3 vTwist1 = { 0.f, 10.f, 0.f };
    _vec3 vTwist2 = { 0.f, 0.f, 0.f };
    _vec3 vTwist3 = { 0.f, -10.f, 0.f };

    if (fProgress < fPhase1) {
        t = fProgress / fPhase1;
        vCurrentRot = LerpRot(vStartRot, vTwist1, t);
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
        vCurrentRot = LerpRot(vTwist3, vStartRot, t);
    }

    float fCurrentAngle = D3DXToRadian(vCurrentRot.y);
    _vec3 vForward = AttackDirection;
    vForward.y = 0.f;
    D3DXVec3Normalize(&vForward, &vForward);
    transform->SetForward(vForward);
    _matrix matRot;
    _vec3 vAxis = transform->GetUp();
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    RotatePlayer(rotateVec);

    // 🎯 종료 조건
    if (AttackTime >= fAttackDuration) {
        AttackTime = 0.f;
        State = ePlayerState::WALK;
    }
}
void Player::UpdateDead(_float dt) {
    auto transform = GetComponent<TransformComponent>();

    const float fDeadDuration = 1.0f; // 사망 모션 전체 시간
    DeadTime += dt;
    float fProgress = std::clamp(DeadTime / fDeadDuration, 0.f, 1.f);

    // 회전 각도는 뒤로 90도만 (x축 기준)
    float fMaxDeathAngle = D3DXToRadian(90.f);
    float fCurrentAngle = fMaxDeathAngle * fProgress;

    // 트랜스폼 회전 적용: 뒤로 누움 (x축 회전만)
    _vec3 vAxis = transform->GetRight(); // x축
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    RotatePlayer(rotateVec);

    // 모션 곡선 비율
    float fLerpRatio = 0.f;
    if (fProgress <= 0.3f) {
        fLerpRatio = fProgress / 0.3f; // 숙이기
    }
    else {
        fLerpRatio = 1.f - ((fProgress - 0.3f) / 0.7f); // 펴기
    }
    fLerpRatio = std::clamp(fLerpRatio, 0.f, 1.f);

    // Lerp 함수
    auto LerpRot = [](const _vec3& start, const _vec3& offset, float ratio) {
        return start + offset * ratio;
    };

    // 몸 각 부위 포즈 적용
    SetRotation(LerpRot(StartRotations["Head"], { -1.f, 0.f, 0.f }, fLerpRatio), "Head");
    SetRotation(LerpRot(StartRotations["LHand"], { -2.f, 0.f, 0.f }, fLerpRatio), "LHand");
    SetRotation(LerpRot(StartRotations["RHand"], { -2.f, 0.f, 0.f }, fLerpRatio), "RHand");
    SetRotation(LerpRot(StartRotations["LLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "LLeg");
    SetRotation(LerpRot(StartRotations["RLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "RLeg");

    // 누운 상태에서 종료 (모션 유지)
    if (DeadTime >= fDeadDuration) {
        DeadTime = fDeadDuration; // 시간 고정
        State = ePlayerState::DEAD; // DEAD 상태 유지
    }
}

void Player::KeyInput(_float dt)
{
    //CheckStateAttack(dt);
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
    static KEY keyRoll = SPACE;

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

void Player::CheckDead()
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    if (input->IsKeyPressed(C) || (GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().curHp <= 0.f && DeadTime == 0))
        State = ePlayerState::DEAD;
}

_vec3 Player::MatrixToEulerAngles(const _matrix& mat) {
    _vec3 vAngles = { 0.f, 0.f, 0.f };

    // Pitch (X축 회전)
    vAngles.x = asinf(-mat._32);

    // Cosine of pitch
    float cosPitch = cosf(vAngles.x);

    // 작은 수로 나눠지는 경우 처리
    if (fabs(cosPitch) > 0.0001f) {
        // Yaw (Y축 회전)
        vAngles.y = -atan2f(mat._31, mat._33);
        // Roll (Z축 회전)
        vAngles.z = atan2f(mat._12, mat._22);
    }
    else {
        // Gimbal lock 발생 시 (pitch = +-90도)
        vAngles.y = -atan2f(-mat._13, mat._11);
        vAngles.z = 0.f;
    }

    return vAngles;
}