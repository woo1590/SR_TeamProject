#include "pch.h"
#include "Player.h"
#include "Player.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InputSystem.h"
#include "InfoComponent.h"

#include "PhysicsSystem.h"
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
#include "Arrow.h"

Player::Player(ObjectManager* owner, ObjectType objType) : BaseCharacter(owner, objType) {}
Player::~Player() {}
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
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test

    collision->SetLayer(CollisionComponent::LAYER_PLAYER);
    collision->SetMask(CollisionComponent::LAYER_ENEMY | CollisionComponent::LAYER_DEFAULT);
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetCollisionStay([this](Object* other) {this->OnCollisionStay(other);});

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);

    auto playerInfo = AddComponent<InfoComponent<PlayerInfo>>();

    SetScale(1.f);
    
    SetMaterial(L"playerBody_Mtrl","Body");
    SetMaterial(L"playerHead_Mtrl","Head");
    SetMaterial(L"playerLeftArm_Mtrl","LArm");
    SetMaterial(L"playerRightArm_Mtrl","RArm");
    SetMaterial(L"playerLeftLeg_Mtrl", "LLeg");
    SetMaterial(L"playerRightLeg_Mtrl", "RLeg");

    Bones["LHand"] = nullptr;
    Bones["RHand"] = Sword::Create(owner, ObjectType::Item);
    Bones["RHand"]->GetComponent<TransformComponent>()->SetParent(Bones["RArm"]->GetComponent<TransformComponent>());

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

}
void Player::Late_Update(_float dt)
{ 
    BaseCharacter::Late_Update(dt); 
}
void Player::Free() 
{ 
    BaseCharacter::Free(); 
}
void Player::PickingTerrain()
{
    if (State == ePlayerState::DEAD || 
        State == ePlayerState::ROLL || 
        State == ePlayerState::ATTACK || 
        State == ePlayerState::SHOOT) 
        return;

    auto input = EngineCore::GetInstance()->GetInputSystem();
    auto curScene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto mainCam = curScene->GetCameraManager()->GetMainCamera();
    auto collision = curScene->GetCollisionSystem();

    if (input->IsKeyPressed(RBUTTON))
    {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);

        if (hit.IsHit)
        {
            if (State == ePlayerState::IDLE) 
            {
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

    if (State != ePlayerState::ATTACK && input->IsKeyPressed(Z) && Bones["RHand"] != nullptr)
    {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);

        if (hit.IsHit)
        {
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK) 
            {
                State = ePlayerState::ATTACK;
                AttackTime = 0.f;
                SaveStartRotation();
            }
            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetWorldPosition();
            auto attackPos = hit.Position;
            AttackDirection = attackPos - curPos;
        }
    }

    if (State != ePlayerState::SHOOT && input->IsKeyPressed(X) && Bones["LHand"] != nullptr)
    {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);

        if (hit.IsHit)
        {
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK) 
            {
                State = ePlayerState::SHOOT;
                AttackTime = 0.f;
                SaveStartRotation();
                Bones["LHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_Alpha);
                Bones["RHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_None);
            }
            auto transform = GetComponent<TransformComponent>();
            auto collision = GetComponent<CollisionComponent>();
            auto curPos = transform->GetWorldPosition();
            auto attackPos = hit.Position;
            AttackDirection = attackPos - curPos;
        }
    }
}
void Player::SaveStartRotation()
{
    StartRotations["Player"] = GetComponent<TransformComponent>()->GetRotate();

    StartRotations["Body"] = Bones["Body"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["Head"] = Bones["Head"]->GetComponent<TransformComponent>()->GetRotate();

    StartRotations["LArm"] = Bones["LArm"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["RArm"] = Bones["RArm"]->GetComponent<TransformComponent>()->GetRotate();

    StartRotations["LLeg"] = Bones["LLeg"]->GetComponent<TransformComponent>()->GetRotate();
    StartRotations["RLeg"] = Bones["RLeg"]->GetComponent<TransformComponent>()->GetRotate();
}
void Player::EquipItem(Item::ItemType itemType)
{
    switch (itemType) {
    case Item::ItemType::ITEM_BOW:
        Bones["LHand"] = Bow::Create(owner, ObjectType::Item);
        Bones["LHand"]->GetComponent<TransformComponent>()->SetParent(Bones["LArm"]->GetComponent<TransformComponent>());
        break;
    case Item::ItemType::ITEM_SWORD:
        Bones["RHand"] = Sword::Create(owner, ObjectType::Item);
        Bones["RHand"]->GetComponent<TransformComponent>()->SetParent(Bones["RArm"]->GetComponent<TransformComponent>());
        break;
    }
}
void Player::UnEquipItem(Item::ItemType itemType)
{
    switch (itemType) {
    case Item::ItemType::ITEM_BOW:
        Safe_Release(Bones["LHand"]);
        Bones["LHand"] = nullptr;
        break;
    case Item::ItemType::ITEM_SWORD:
        Safe_Release(Bones["RHand"]);
        Bones["RHand"] = nullptr;
        break;
    }
}
Object* Player::GetBone(std::string boneName)
{
    return Bones[boneName];
}
void Player::UpdateIdle(_float dt)
{
    IdleSmoothing(dt, "Head");

    IdleSmoothing(dt, "LArm");
    IdleSmoothing(dt, "RArm");
     
    IdleSmoothing(dt, "LLeg");
    IdleSmoothing(dt, "RLeg");
}
void Player::UpdateWalk(_float dt) {
    WalkTime += dt;

    //Rotate Bones
    float walkSwingSpeed = 10.f;
    float fAngle = sinf(WalkTime * walkSwingSpeed);

    SetRotation({ fAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "RLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "LArm");
    SetRotation({ fAngle, 0.f, 0.f }, "RArm");
    
    //Move Player
    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);
    
    float Speed = GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().speed;
    _vec3 moveVec = 
    {
        vDir.x * Speed * Scale * dt,
        0,
        vDir.z * Speed * Scale * dt
    };

    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition() + moveVec);
    
    //Rotate Player
    const float fRotateDuration = 0.05f;

    float TargetAngle = atan2f(vDir.x, vDir.z);
    _vec3 vCurRot = GetComponent<TransformComponent>()->GetRotate();

    float curAngle = NormalizeAngle(vCurRot.y);
    float deltaAngle = NormalizeAngle(TargetAngle - curAngle);
    
    float rotValue = deltaAngle * (dt / fRotateDuration);
    transform->SetRotate(transform->GetRotate() + _vec3{ 0.f, rotValue, 0.f });
    
    //CheckExit
    auto curPos = transform->GetPosition();
    auto posGap = curPos - destinationPos;
    auto distance = sqrtf(posGap.x * posGap.x + posGap.z * posGap.z);
    if (distance < 0.5f) 
    {
        State = ePlayerState::IDLE;
        WalkTime = 0.f;
    } 
}
void Player::UpdateRoll(_float dt)
{
    RollTime += dt;

    //Rotate Bones
    const float fRollDuration = 0.5f;
    float fProgress = RollTime / fRollDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);

    float fLerpRatio = 0.f;
    if (fProgress <= 0.2f) 
    {
        fLerpRatio = fProgress / 0.2f;
    } 
    else if (fProgress >= 0.8f) 
    {
        fLerpRatio = (1.f - fProgress) / 0.2f;
    }
    else 
    {
        fLerpRatio = 1.f; 
    }

    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);

    SetRotation(OffsetLerp(StartRotations["Head"], { 1, 0.f, 0.f }, fLerpRatio), "Head");
    SetRotation(OffsetLerp(StartRotations["LLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "LLeg");
    SetRotation(OffsetLerp(StartRotations["RLeg"], { -1.5f, 0.f, 0.f }, fLerpRatio), "RLeg");
    SetRotation(OffsetLerp(StartRotations["LArm"], { -2.f, 0.f, 0.f }, fLerpRatio), "LArm");
    SetRotation(OffsetLerp(StartRotations["RArm"], { -2.f, 0.f, 0.f }, fLerpRatio), "RArm");

    //Move Player
    const float fRollSpeed = GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().speed * 3.f;
    _vec3 moveVec = 
    {
        vDir.x * fRollSpeed * Scale * dt,
        0,
        vDir.z * fRollSpeed* Scale * dt
    };
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition() + moveVec);

    //Rotate Player
    float fTotalRollAngle = D3DX_PI * 2.f ;
    float fCurrentAngle = fTotalRollAngle * RollTime / fRollDuration;

    transform->SetForward(vDir);
    _vec3 vAxis = transform->GetRight();
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);

    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    transform->SetRotate(transform->GetRotate() + rotateVec);

    //CheckExit
    if (RollTime >= fRollDuration) 
    {
        RollTime = 0.f;
        State = ePlayerState::WALK;

        _vec3 vCurRot = transform->GetRotate();
        vCurRot.x = 0.f;
        vCurRot.z = 0.f;
        transform->SetRotate(vCurRot);
    }
}
void Player::UpdateAttack(_float dt) {
    AttackTime += dt;

    //Rotate Bones
    const float fAttackDuration = 0.3f;

    float fProgress = AttackTime / fAttackDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);
    vector<float> phaseVec = { 0.1f, 0.4f, 0.9f, 1.f };

    vector<_vec3> RArmRotVec =
    {
        StartRotations["RArm"],
        { -180.f, 10.f, -60.f },
        { -120.f, 10.f, -60.f },
        { -30.f, 10.f,-60.f }
    };
    vector<_vec3> LArmRotVec =
    {
        StartRotations["LArm"],
        { -30.f, 10.f, -10.f },
        { 0.f, 0.f, -10.f },
        { 30.f, -10.f, -10.f }
    };
    vector<_vec3> LLegRotVec =
    {
        StartRotations["LLeg"],
        { -10.f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { 10.f, 0.f, 0.f }
    };
    vector<_vec3> RLegRotVec =
    {
        StartRotations["RLeg"],
        { 10.f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { -10.f, 0.f, 0.f }
    };

    ApplyPhasedRotation("LArm", fProgress, phaseVec, LArmRotVec);
    ApplyPhasedRotation("RArm", fProgress, phaseVec, RArmRotVec);
    ApplyPhasedRotation("LLeg", fProgress, phaseVec, LLegRotVec);
    ApplyPhasedRotation("RLeg", fProgress, phaseVec, RLegRotVec);

    //Rotate Player
    _vec3 vPlayerStartRot = { 0.f, atan2f(-AttackDirection.x, AttackDirection.z), 0.f };
    vector<_vec3> bodyRotVec =
    {
        vPlayerStartRot,
        {0.f, 30.f, 0.f},
        {0.f, 0.f, 0.f},
        {0.f, -60.f, 0.f}
    };

    _vec3 vCurrentRot = GetPhasedRotation(fProgress, phaseVec, bodyRotVec);
    _vec3 vDeltaRot = vCurrentRot - vPlayerStartRot;

    auto transform = GetComponent<TransformComponent>();
    transform->SetRotate(vDeltaRot);

    //CheckExit
    if (AttackTime >= fAttackDuration) 
    {
        AttackTime = 0.f;
        if (WalkTime == 0.f) 
            State = ePlayerState::IDLE;
        else 
        {
            WalkTime = 0.f;
            State = ePlayerState::WALK;
        }
    }
}
void Player::UpdateShoot(_float dt) {
    AttackTime += dt;

    //shoot arrow
    const float fAttackDuration = 0.6f;
    float fProgress = std::clamp(AttackTime / fAttackDuration, 0.f, 1.f);
    vector<float> phaseVec = { 0.15f, 0.4f, 0.9f, 1.f };

    static float prePhase = 0.f;
    if (prePhase < phaseVec.at(0) && fProgress >= phaseVec.at(0)) {
        auto shootDir = AttackDirection ;
        D3DXVec3Normalize(&shootDir, &shootDir);
        Arrow::Create(owner, ObjectType::Projectile, this, shootDir);
    }
    prePhase = fProgress;

    //Rotate Bones
    vector<_vec3> LArmRotVec =
    {
        StartRotations["LArm"],
        { -100.f, 0.f, -30.f },
        { -100.f, 0.f, -30.f },
        { 0.f, 0.f, 0.f }
    };
    vector<_vec3> RArmRotVec =
    {
        StartRotations["RArm"],
        { -100.f, 0.f, 60.f },
        { -100.f, 0.f, 60.f },
        { 0.f, 0.f, 0.f }
    };
    vector<_vec3> LLegRotVec =
    {
        StartRotations["LLeg"],
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };
    vector<_vec3> RLegRotVec =
    {
        StartRotations["RLeg"],
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };
    ApplyPhasedRotation("LArm", fProgress, phaseVec, LArmRotVec);
    ApplyPhasedRotation("RArm", fProgress, phaseVec, RArmRotVec);
    ApplyPhasedRotation("LLeg", fProgress, phaseVec, LLegRotVec);
    ApplyPhasedRotation("RLeg", fProgress, phaseVec, RLegRotVec);

    //Rotate Player
    auto transform = GetComponent<TransformComponent>();
    vector<_vec3> bodyRotVec =
    {
        transform->GetRotate(),
        { 0.f, 10.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, -10.f, 0.f }
    };

    _vec3 vCurrentRot = GetPhasedRotation(fProgress, phaseVec, bodyRotVec);
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

    // CheckExit
    if (AttackTime >= fAttackDuration) {
        AttackTime = 0.f;
        if (WalkTime == 0.f) State = ePlayerState::IDLE;
        else {
            WalkTime = 0.f;
            State = ePlayerState::WALK;
        }
        Bones["LHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_None);
        Bones["RHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_Alpha);
        prePhase = 0.f;
    }
}
void Player::UpdateDead(_float dt) {
    DeadTime += dt;

    //Wait Revive
    const float fDeadDuration = 1.0f;
    if (DeadTime >= fDeadDuration) {
        DeadTime = fDeadDuration; // 시간 고정
        State = ePlayerState::DEAD; // DEAD 상태 유지
        return;
    }
    
    //Rotate Bones
    float fProgress = std::clamp(DeadTime / fDeadDuration, 0.f, 1.f);
    vector<float> phaseVec = { 0.3f , 1.f };

    vector<_vec3> headRotVec =
    {
        StartRotations["Head"],
        { 30.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };
    vector<_vec3> LArmRotVec =
    {
        StartRotations["LArm"],
        { -90.f, 0.f, 0.f },
        { 0.f, 0.f, -90.f }
    };
    vector<_vec3> RArmRotVec =
    {
        StartRotations["RArm"],
        { -90.f, 0.f, 0.f },
        { 0.f, 0.f, 90.f }
    };
    vector<_vec3> LLegRotVec =
    {
        StartRotations["LLeg"],
        { -90.f, 0.f, 0.f },
        { 0.f, 0.f, -10.f }
    };
    vector<_vec3> RLegRotVec =
    {
        StartRotations["RLeg"],
        { -90.f, 0.f, 0.f },
        { 0.f, 0.f, 10.f }
    };

    ApplyPhasedRotation("Head", fProgress, phaseVec, headRotVec);
    ApplyPhasedRotation("LArm", fProgress, phaseVec, LArmRotVec);
    ApplyPhasedRotation("RArm", fProgress, phaseVec, RArmRotVec);
    ApplyPhasedRotation("LLeg", fProgress, phaseVec, LLegRotVec);
    ApplyPhasedRotation("RLeg", fProgress, phaseVec, RLegRotVec);

    //Rotate Player
    float fMaxDeathAngle = D3DXToRadian(90.f);
    float fCurrentAngle = fMaxDeathAngle * fProgress;

    _vec3 moveVec = PlayerDirection;
    moveVec.y = 0.f;
    D3DXVec3Normalize(&moveVec, &moveVec);

    auto transform = GetComponent<TransformComponent>();
    transform->SetForward(moveVec);
    _vec3 vAxis = transform->GetRight();
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, -fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    transform->SetRotate(transform->GetRotate() + rotateVec);

    //lerp dead y position
    auto collision = GetComponent<CollisionComponent>();
    float fSinT = sinf(D3DX_PI / 2.f + (D3DX_PI / 2.f) * fProgress);
    float fLerpY = 2.f + (7.f - 2.f) * fSinT;
    collision->SetSize(_vec3(2.f, fLerpY, 2.f));
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

    //Check State
    if (input->IsKeyPressed(keyRoll)) 
    {
        switch (State) 
        {
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
    if (input->IsKeyPressed(C) || (GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().curHp <= 0.f && DeadTime == 0)) 
    {
        State = ePlayerState::DEAD;
        SaveStartRotation();
    }
}
_vec3 Player::MatrixToEulerAngles(const _matrix& mat) 
{
    _vec3 vAngles = { 0.f, 0.f, 0.f };
    vAngles.x = asinf(-mat._32); // Pitch (X rot)
    float cosPitch = cosf(vAngles.x); // Cosine of pitch

    if (fabs(cosPitch) > 0.0001f) 
    {
        vAngles.y = -atan2f(mat._31, mat._33); // Yaw (Y rot)
        vAngles.z = atan2f(mat._12, mat._22); // Roll (Z rot)
    }
    else 
    {
        vAngles.y = -atan2f(-mat._13, mat._11); // Gimbal lock (pitch = +-90)
        vAngles.z = 0.f;
    }
    return vAngles;
}

void Player::OnCollisionStay(Object* other)
{
    ObjectType objType = other->GetObjectType();
    auto collision = GetComponent<CollisionComponent>();

    /*if (objType == ObjectType::StaticBlock)
        collision->ResolveAABBColiision(other);*/
}

void Player::IdleSmoothing(_float dt, std::string bone)
{
    _vec3 vCurrentRot = Bones[bone]->GetComponent<TransformComponent>()->GetRotate();
    _vec3 vTargetRot = { 0.f, 0.f, 0.f };
    float SmoothingSpeed = 5.f;

    _vec3 vLerpedRot = vCurrentRot + (vTargetRot - vCurrentRot) * dt * SmoothingSpeed;
    Bones[bone]->GetComponent<TransformComponent>()->SetRotate(vLerpedRot);
}

float Player::NormalizeAngle(_float angle)
{
    while (angle > D3DX_PI) 
        angle -= D3DX_PI * 2.f;
    while (angle < -D3DX_PI) 
        angle += D3DX_PI * 2.f;

    return angle;
}

float Player::OffsetLerp(const _float& start, const _float& offset, float ratio)
{
    return start + offset * ratio;
}

_vec3 Player::OffsetLerp(const _vec3& start, const _vec3& offset, float ratio)
{
    return start + offset * ratio;
}

_vec3 Player::DegToRadLerp(const _vec3& startDeg, const _vec3& endDeg, float ratio)
{
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
    _vec3 vDelta = endRad - startRad;
    return OffsetLerp(startRad, vDelta, ratio);
}

_vec3 Player::GetPhasedRotation(float fProgress, vector<float>& phaseVec, vector<_vec3>& destinations)
{
    float t = 0.f;
    _vec3 vCurrentRot;;

    int dSize = destinations.size();
    int pSize = phaseVec.size();
    int d = (dSize < pSize) ? dSize : pSize;

    if (d == 1 || fProgress <= phaseVec.at(0)) {
        t = fProgress / phaseVec.at(0);
        vCurrentRot = DegToRadLerp(destinations.at(0), destinations.at(1), t);
    }
    else if (d == 2 || fProgress <= phaseVec.at(1)) {
        t = (fProgress - phaseVec.at(0)) / (phaseVec.at(1) - phaseVec.at(0));
        vCurrentRot = DegToRadLerp(destinations.at(1), destinations.at(2), t);
    }
    else if (d == 3 || fProgress <= phaseVec.at(2)) {
        t = (fProgress - phaseVec.at(1)) / (phaseVec.at(2) - phaseVec.at(1));
        vCurrentRot = DegToRadLerp(destinations.at(2), destinations.at(3), t);
    }
    else if(d == 4) {
        t = (fProgress - phaseVec.at(2)) / (phaseVec.at(3) - phaseVec.at(2));
        vCurrentRot = DegToRadLerp(destinations.at(3), destinations.at(0), t);
    }
    return vCurrentRot;
}

void Player::ApplyPhasedRotation(const std::string& name, float fProgress, vector<float>& phaseVec, vector<_vec3>& destinations)
{
    _vec3 vCurrentRot = GetPhasedRotation(fProgress, phaseVec, destinations);
    SetRotation(vCurrentRot, name);
}
