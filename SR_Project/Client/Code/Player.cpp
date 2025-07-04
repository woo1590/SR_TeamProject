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

#include "MyMath.h"

#include "Sword.h"
#include "Bow.h"

Player::Player(ObjectManager* owner, ObjectType objType) : BaseCharacter(owner, objType) {}
Player::~Player() {}
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
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    auto collision = AddComponent<CollisionComponent>();
    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_DEFAULT);
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetCollisionStay([this](Object* other) {this->OnCollisionStay(other);});

    auto physics = AddComponent<PhysicsComponent>();
    physics->SetMass(1.f);

    auto playerInfo = AddComponent<InfoComponent<PlayerInfo>>();

    //PlayerScale
    SetScale(1.f);
    //PlayerTexture
    SetMaterial(L"playerBody_Mtrl","Body");
    SetMaterial(L"playerHead_Mtrl","Head");
    SetMaterial(L"playerLeftArm_Mtrl","LArm");
    SetMaterial(L"playerRightArm_Mtrl","RArm");
    SetMaterial(L"playerLeftLeg_Mtrl", "LLeg");
    SetMaterial(L"playerRightLeg_Mtrl", "RLeg");
     
    Bones["Sword"] = Bone::Create(owner, objType, _vec3(0.1f * Scale, 2.f * Scale, 2.f * Scale), Bones["RArm"], L"sword_Mtrl");
    Bones["Sword"]->GetComponent<TransformComponent>()->SetPosition(0.f, 0.2f, 1.f * Scale);
    Bones["Sword"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.f * Scale, 0.8f * Scale, 0.f * Scale));
    Bones["Sword"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    Bones["Sword"]->GetComponent<TransformComponent>()->SetRotate({ 0.8f,0.f,0.f });
    SetMaterial(L"sword_Mtrl", "Sword", Engine::RENDER_ID::Render_Alpha);
    owner->AddObject(objType, Bones["Sword"]);
    
    Bones["Bow"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["LArm"], L"bow_Mtrl");
    Bones["Bow"]->GetComponent<TransformComponent>()->SetScale(0.1f * Scale, 1.f * Scale, 1.f * Scale);
    Bones["Bow"]->GetComponent<TransformComponent>()->SetPosition(0.25f * Scale, -1.2f * Scale, -0.2f * Scale);
    Bones["Bow"]->GetComponent<TransformComponent>()->SetPivot(_vec3(-0.25f * Scale, 0.8f * Scale, -0.2f * Scale));
    Bones["Bow"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    Bones["Bow"]->GetComponent<TransformComponent>()->SetRotate({ 2.2f,0.f,0.f });
    SetMaterial(L"bow_Mtrl", "Bow", Engine::RENDER_ID::Render_Alpha);
    owner->AddObject(objType, Bones["Bow"]);

    //Bones["Sword"] = Sword::Create(owner, ObjectType::Item);
    //Bones["Bow"] = Bow::Create(owner, ObjectType::Item);

    return S_OK;
}
void Player::Update(_float dt){
    BaseCharacter::Update(dt);
    KeyInput(dt);
    switch (State) {
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

    //Physics
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
        physics->SetGround(false);
        _vec3 velocity = physics->GetVelocity();
        transform->Translate(velocity * dt);
    }
    
}
void Player::Late_Update(_float dt){ BaseCharacter::Late_Update(dt); }
void Player::Free() { BaseCharacter::Free(); }
void Player::PickingTerrain(){
    if (State == ePlayerState::DEAD || State == ePlayerState::ROLL || State == ePlayerState::ATTACK || State == ePlayerState::SHOOT) return;
    auto input = EngineCore::GetInstance()->GetInputSystem();
    auto curScene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto mainCam = curScene->GetCameraManager()->GetMainCamera();
    auto collision = curScene->GetCollisionSystem();

    if (input->IsKeyPressed(RBUTTON)){
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        if (hit.IsHit){
            if (State == ePlayerState::IDLE) {
                State = ePlayerState::WALK;
                WalkTime = 0.f;
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetPosition();
            destinationPos = hit.Position;
            destinationPos.y = 0.f;
            curPos.y = 0.f;
            PlayerDirection = destinationPos - curPos;
        }
    }

    if (State != ePlayerState::ATTACK && input->IsKeyPressed(Z)) {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        if (hit.IsHit){
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK) {
                State = ePlayerState::ATTACK;
                AttackTime = 0.f;
                SaveStartRotation();
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetPosition();
            auto attackPos = hit.Position;
            AttackDirection = attackPos - curPos;
        }
    }

    if (State != ePlayerState::SHOOT && input->IsKeyPressed(X)) {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);
        if (hit.IsHit){
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK) {
                State = ePlayerState::SHOOT;
                AttackTime = 0.f;
                SaveStartRotation();
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetPosition();
            auto attackPos = hit.Position;
            AttackDirection = attackPos - curPos;
        }
    }
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
    SmoothReset(Bones["LArm"]->GetComponent<TransformComponent>());
    SmoothReset(Bones["RArm"]->GetComponent<TransformComponent>());
}
void Player::UpdateWalk(_float dt) {
    auto transform = GetComponent<TransformComponent>();
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
    transform->SetPosition(transform->GetPosition() + moveVec);
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
    transform->SetRotate(transform->GetRotate() + _vec3{ 0.f, rotValue, 0.f });
    auto curPos = transform->GetPosition();
    auto posGap = curPos - destinationPos;
    auto distance = sqrtf(posGap.x * posGap.x + posGap.z * posGap.z);
    if (distance < 0.5f) {
        State = ePlayerState::IDLE;
        WalkTime = 0.f;
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
    //static float fStartY = 0.f;
    //if (RollTime == dt)
    //    fStartY = transform->GetPosition().y;
    //
    //const float fAmplitudePhase1 = 0.2f * Scale;
    //const float fAmplitudePhase2 = 2.8f * Scale;
    //float t = fProgress;
    //float fYOffset = 0.f;
    //
    //if (t <= 0.3f) {
    //    float fLocalT = t / 0.3f; // 0~1
    //    fYOffset = sinf(fLocalT * D3DX_PI) * fAmplitudePhase1; // 0 ~ π
    //}
    //else {
    //    float fLocalT = (t - 0.3f) / 0.7f; // 0~1
    //    fYOffset = sinf(D3DX_PI + fLocalT * D3DX_PI) * fAmplitudePhase2; // π ~ 2π
    //}
    //_vec3 vCurPos = transform->GetPosition();
    //vCurPos.y = fStartY + fYOffset;
    //transform->SetPosition(vCurPos);
    //movePlayer
    _vec3 moveVec = {
        vDir.x * fRollSpeed * Scale * dt,
        0,
        vDir.z * fRollSpeed* Scale * dt
    };
    transform->SetPosition(transform->GetPosition() + moveVec);
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

    transform->SetRotate(transform->GetRotate() + rotateVec);
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
    const float fAttackDuration = 0.4f;
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
        { -210.f, 10.f, -60.f },
        { -150.f, 10.f, -60.f },
        { -60.f, 10.f,-60.f }
    );
    ApplyPhasedRotation("LArm",
        StartRotations["LArm"],
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
    _vec3 vPlayerStartRot = { 0.f,0.f,0.f };
    vPlayerStartRot.y = atan2f(-AttackDirection.x, AttackDirection.z);
    // 키프레임 절대값으로 변경
    _vec3 vKey1 = vPlayerStartRot + _vec3(0.f, 30.f, 0.f);
    _vec3 vKey2 = vPlayerStartRot + _vec3(0.f, 0.f, 0.f);
    _vec3 vKey3 = vPlayerStartRot + _vec3(0.f, -60.f, 0.f);

    if (fProgress < fPhase1) {
        t = fProgress / fPhase1;
        vCurrentRot = LerpRot(vPlayerStartRot, vKey1, t);
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
        vCurrentRot = LerpRot(vKey3, vPlayerStartRot, t);
    }

    _vec3 vDeltaRot = vCurrentRot - vPlayerStartRot;
    auto transform = GetComponent<TransformComponent>();
    transform->SetRotate(vDeltaRot);
    //Exit State
    if (AttackTime >= fAttackDuration) {
        AttackTime = 0.f;
        if (WalkTime == 0.f) State = ePlayerState::IDLE;
        else {
            WalkTime = 0.f;
            State = ePlayerState::WALK;
        }
    }
}
void Player::UpdateShoot(_float dt) {
    auto transform = GetComponent<TransformComponent>();
    const float fAttackDuration = 0.6f;
    AttackTime += dt;
    float fProgress = std::clamp(AttackTime / fAttackDuration, 0.f, 1.f);

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
            if (fProgress < fPhase1)
                t = fProgress / fPhase1,
                vCurrentRot = LerpRot(vStart, vKey1, t);
            else if (fProgress < fPhase2)
                t = (fProgress - fPhase1) / (fPhase2 - fPhase1),
                vCurrentRot = LerpRot(vKey1, vKey2, t);
            else if (fProgress < fPhase3)
                t = (fProgress - fPhase2) / (fPhase3 - fPhase2),
                vCurrentRot = LerpRot(vKey2, vKey3, t);
            else
                t = (fProgress - fPhase3) / (fPhase4 - fPhase3),
                vCurrentRot = LerpRot(vKey3, vStart, t);

            SetRotation(vCurrentRot, name);
        };

    ApplyPhasedRotation("LArm",
        StartRotations["LArm"],
        { -100.f, 0.f, -30.f },  // 앞으로 뻗고 약간 안쪽으로 회전 (Z축 음수)
        { -100.f, 0.f, -30.f },
        { 0.f, 0.f, 0.f }
    );

    ApplyPhasedRotation("RArm",
        StartRotations["RArm"],
        { -100.f, 0.f, 60.f },   // 앞으로 뻗고 약간 안쪽으로 회전 (Z축 양수)
        { -100.f, 0.f, 60.f },
        { 0.f, 0.f, 0.f }
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

    // 🔁 본체 회전은 별도로 처리 (시선 고정 + 트위스트)
    _vec3 vStartRot = transform->GetRotate();
    _vec3 vTwist1 = { 0.f, 10.f, 0.f };
    _vec3 vTwist2 = { 0.f, 0.f, 0.f };
    _vec3 vTwist3 = { 0.f, -10.f, 0.f };

    if (fProgress < fPhase1)
        t = fProgress / fPhase1,
        vCurrentRot = LerpRot(vStartRot, vTwist1, t);
    else if (fProgress < fPhase2)
        t = (fProgress - fPhase1) / (fPhase2 - fPhase1),
        vCurrentRot = LerpRot(vTwist1, vTwist2, t);
    else if (fProgress < fPhase3)
        t = (fProgress - fPhase2) / (fPhase3 - fPhase2),
        vCurrentRot = LerpRot(vTwist2, vTwist3, t);
    else
        t = (fProgress - fPhase3) / (fPhase4 - fPhase3),
        vCurrentRot = LerpRot(vTwist3, vStartRot, t);

    float fCurrentAngle = D3DXToRadian(vCurrentRot.y);
    _vec3 vForward = AttackDirection;
    vForward.y = 0.f;
    D3DXVec3Normalize(&vForward, &vForward);
    transform->SetForward(vForward);

    _matrix matRot;
    _vec3 vAxis = transform->GetUp();
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    transform->SetRotate(transform->GetRotate() + rotateVec);

    // 종료 조건
    if (AttackTime >= fAttackDuration) {
        AttackTime = 0.f;
        if (WalkTime == 0.f) State = ePlayerState::IDLE;
        else {
            WalkTime = 0.f;
            State = ePlayerState::WALK;
        }
    }
}
void Player::UpdateDead(_float dt) {
    auto transform = GetComponent<TransformComponent>();

    const float fDeadDuration = 1.0f; // 사망 모션 전체 시간
    DeadTime += dt;

    // 누운 상태에서 종료 (모션 유지)
    if (DeadTime >= fDeadDuration) {
        DeadTime = fDeadDuration; // 시간 고정
        State = ePlayerState::DEAD; // DEAD 상태 유지
    }

    float fProgress = std::clamp(DeadTime / fDeadDuration, 0.f, 1.f);

    // 회전 각도는 뒤로 90도만 (x축 기준)
    float fMaxDeathAngle = D3DXToRadian(90.f);
    float fCurrentAngle = fMaxDeathAngle * fProgress;

    // 전방 벡터 정규화
    _vec3 moveVec = PlayerDirection;
    moveVec.y = 0.f;
    D3DXVec3Normalize(&moveVec, &moveVec);
    transform->SetForward(moveVec);

    // 회전 적용: 뒤로 눕기
    _vec3 vAxis = transform->GetRight(); // x축
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, -fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    transform->SetRotate(transform->GetRotate() + rotateVec);

    // Lerp 함수
    auto LerpRot = [](const _vec3& from, const _vec3& to, float ratio) {
        return from + (to - from) * ratio;
    };

    // Folded 상태 (0.3초까지): 숙인 자세
    _vec3 rot_HeadFolded = { 1.f, 0.f, 0.f };
    _vec3 rot_LHandFolded = { -1.5f, 0.f, 0.f };
    _vec3 rot_RHandFolded = { -1.5f, 0.f, 0.f };
    _vec3 rot_LLegFolded = { -2.f, 0.f, 0.f };
    _vec3 rot_RLegFolded = { -2.f, 0.f, 0.f };

    // Flat 상태: 완전히 펼친 자세
    _vec3 rot_HeadFlat = { 0.f, 0.f, 0.f };
    _vec3 rot_LHandFlat = { 0.f, 0.f, -1.5f };
    _vec3 rot_RHandFlat = { 0.f, 0.f, 1.5f };
    _vec3 rot_LLegFlat = { 0.f, 0.f, -0.2f };
    _vec3 rot_RLegFlat = { 0.f, 0.f, 0.2f };

    float fLerpRatio = 0.f;

    if (fProgress <= 0.3f) {
        fLerpRatio = fProgress / 0.3f;
        fLerpRatio = std::clamp(fLerpRatio, 0.f, 1.f);

        SetRotation(LerpRot(StartRotations["Head"], rot_HeadFolded, fLerpRatio), "Head");
        SetRotation(LerpRot(StartRotations["LArm"], rot_LHandFolded, fLerpRatio), "LArm");
        SetRotation(LerpRot(StartRotations["RArm"], rot_RHandFolded, fLerpRatio), "RArm");
        SetRotation(LerpRot(StartRotations["LLeg"], rot_LLegFolded, fLerpRatio), "LLeg");
        SetRotation(LerpRot(StartRotations["RLeg"], rot_RLegFolded, fLerpRatio), "RLeg");
    }
    else {
        fLerpRatio = (fProgress - 0.3f) / 0.7f;
        fLerpRatio = std::clamp(fLerpRatio, 0.f, 1.f);

        SetRotation(LerpRot(rot_HeadFolded, rot_HeadFlat, fLerpRatio), "Head");
        SetRotation(LerpRot(rot_LHandFolded, rot_LHandFlat, fLerpRatio), "LArm");
        SetRotation(LerpRot(rot_RHandFolded, rot_RHandFlat, fLerpRatio), "RArm");
        SetRotation(LerpRot(rot_LLegFolded, rot_LLegFlat, fLerpRatio), "LLeg");
        SetRotation(LerpRot(rot_RLegFolded, rot_RLegFlat, fLerpRatio), "RLeg");
    }
}

void Player::KeyInput(_float dt)
{
    PickingTerrain();
    CheckStateRoll(dt);
    CheckDead();
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
            SaveStartRotation();
            break;
        }
    }
}
void Player::CheckDead()
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    if (input->IsKeyPressed(C) || (GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().curHp <= 0.f && DeadTime == 0)) {
        State = ePlayerState::DEAD;
        SaveStartRotation();
    }
}
_vec3 Player::MatrixToEulerAngles(const _matrix& mat) {
    _vec3 vAngles = { 0.f, 0.f, 0.f };
    vAngles.x = asinf(-mat._32); // Pitch (X rot)
    float cosPitch = cosf(vAngles.x); // Cosine of pitch
    if (fabs(cosPitch) > 0.0001f) {
        vAngles.y = -atan2f(mat._31, mat._33); // Yaw (Y rot)
        vAngles.z = atan2f(mat._12, mat._22); // Roll (Z rot)
    }
    else {
        vAngles.y = -atan2f(-mat._13, mat._11); // Gimbal lock (pitch = +-90)
        vAngles.z = 0.f;
    }
    return vAngles;
}

void Player::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    if (objType == ObjectType::StaticBlock)
        collision->ResolveAABBColiision(other);
}
