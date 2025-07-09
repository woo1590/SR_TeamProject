#include "pch.h"
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

#include "StaticGrid.h"

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

    SetMaterial("playerBody_Mtrl", "Body");
    SetMaterial("playerHead_Mtrl", "Head");
    SetMaterial("playerLeftArm_Mtrl", "LArm");
    SetMaterial("playerRightArm_Mtrl", "RArm");
    SetMaterial("playerLeftLeg_Mtrl", "LLeg");
    SetMaterial("playerRightLeg_Mtrl", "RLeg");

    Bones["LHand"] = nullptr;
    Bones["RHand"] = nullptr;

    return S_OK;
}
void Player::Update(_float dt) {
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

    if (input->IsKeyPressed(LBUTTON))
    {
        Ray ray = mainCam->ScreenPointRay();
        HitInfo hit = collision->Raycast(ray);

        if (hit.IsHit)
        {
            if ((State == ePlayerState::IDLE || State == ePlayerState::WALK) &&
                hit.Component->GetLayer() == CollisionComponent::LAYER_ENEMY &&
                Bones["RHand"] != nullptr)
            {
                if (State == ePlayerState::IDLE)
                {
                    WalkTime = 0.f;
                    State = ePlayerState::WALK;

                    moveToAttack = true;
                    moveToObject = hit.Component->GetOwner();
                    return;
                }

                DestinationPos = hit.Position;
                auto transform = GetComponent<TransformComponent>();
                auto curPos = transform->GetWorldPosition();

                DestinationPos.y = 0.f;
                curPos.y = 0.f;
                PlayerDirection = DestinationPos - curPos;

                auto distance = sqrtf(PlayerDirection.x * PlayerDirection.x + PlayerDirection.z * PlayerDirection.z);
                if (distance <= SwordRange)
                {
                    State = ePlayerState::ATTACK;
                    AttackTime = 0.f;

                    SaveStartRotation();
                    curPos = transform->GetWorldPosition();
                    auto attackPos = hit.Position;
                    AttackDirection = attackPos - curPos;
                }
            }
            if (State == ePlayerState::IDLE || State == ePlayerState::WALK)
            {
                if (State == ePlayerState::IDLE)
                {
                    State = ePlayerState::WALK;
                    WalkTime = 0.f;
                }
                if (moveToAttack)
                {
                    moveToAttack = false;
                    moveToObject = nullptr;
                }
                auto transform = GetComponent<TransformComponent>();
                auto curPos = transform->GetWorldPosition();
                DestinationPos = hit.Position;

                DestinationPos.y = 0.f;
                curPos.y = 0.f;
                PlayerDirection = DestinationPos - curPos;
            }
        }
    }

    if (State != ePlayerState::SHOOT && input->IsKeyPressed(RBUTTON) && Bones["LHand"] != nullptr)
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

    if (Bones["RHand"])
        StartRotations["RHand"] = Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate();
    if (Bones["LHand"])
        StartRotations["LHand"] = Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate();
}
void Player::SetUpFirstAttackPhaseRotations()
{
    vector<float> phase = { 0.1f,0.45f,1.f };

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        { 0.f, -atan2f(-AttackDirection.x, AttackDirection.z), 0.f },
        {0.f, -GetStringAngleY("right","front"), 0.f},
        {0.f, 0.f, 0.f},
        {0.f, -GetStringAngleY("left","front"), 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations =
    {
        StartRotations["LArm"],
        { GetStringAngleX("front","down"),   0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("","down"),                  0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("","down"),    0.f, GetStringAngleZ("left","down")}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
        StartRotations["RArm"],
        { GetStringAngleX("","up"),      0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("front","up"),      0.f, GetStringAngleZ("left","down") },
        { GetStringAngleX("front","down"),  0.f, GetStringAngleZ("left","down") }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        StartRotations["LLeg"],
        { GetStringAngleX("front","down") * 0.5f,   0.f, 0.f },
        { 0.f,                                      0.f, -5.f },
        { GetStringAngleX("back","down") * 0.5f,     0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations =
    {
        StartRotations["RLeg"],
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        { 0.f, atan2f(-AttackDirection.x, AttackDirection.z), 0.f },
        {0.f, GetStringAngleY("right","front"), 0.f},
        {0.f, 0.f, 0.f},
        {0.f, GetStringAngleY("left","front"), 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    _vec3 baseOffset = D3DXToDegree(itemBaseRotOffset.at("sword"));
    PhaseRotation RightHandRot;
    RightHandRot.name = "RHand";
    RightHandRot.phaseVec = phase;
    RightHandRot.destinations =
    {
        {baseOffset.x, 0.f, 0.f },
        {baseOffset.x + GetStringAngleX("front","down") * 0.5f, 0.f, 0.f},
        {baseOffset.x, 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND, RightHandRot);
}
void Player::SetUpSecondAttackPhaseRotations() {
    vector<float> phase = { 0.1f, 0.45f, 1.f };

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        {0.f, -GetStringAngleY("left","front"), 0.f},
        { 0.f, -GetStringAngleY("left","front"), 0.f },
        { 0.f,  -GetStringAngleY("","front"), 0.f },
        { 0.f,  -GetStringAngleY("right","front"), 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations = 
    {
        { GetStringAngleX("back","down"),      0.f, GetStringAngleZ("left","down") * 0.2f},
        { GetStringAngleX("","down"),    0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down"),            0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down"),    0.f, GetStringAngleZ("left","")}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations = 
    {
        { GetStringAngleX("front","down"),  0.f,                            GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down"), GetStringAngleY("","back"),      GetStringAngleZ("left","down") },
        { GetStringAngleX("front","up") *0.75f, GetStringAngleY("","back"),   GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down") + GetStringAngleX("","up"), GetStringAngleY("","back"),          GetStringAngleZ("left","down") }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations = {
        { GetStringAngleX("back","down") * 0.5f,     0.f, 0.f },
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations = {
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations = {
        {0.f, GetStringAngleY("left","front"), 0.f},
        { 0.f, GetStringAngleY("left","front"), 0.f },
        { 0.f,  GetStringAngleY("","front"), 0.f },
        { 0.f,  GetStringAngleY("right","front"), 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    _vec3 baseOffset = D3DXToDegree(itemBaseRotOffset.at("sword"));
    PhaseRotation RightHandRot;
    RightHandRot.name = "RHand";
    RightHandRot.phaseVec = phase;
    RightHandRot.destinations =
    {
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND, RightHandRot);
}
void Player::SetUpLastAttackPhaseRotations() {
    vector<float> phase = { 0.1f, 0.45f, 1.f };

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        { 0.f,  -GetStringAngleY("right","front"), 0.f },
        { 0.f, -GetStringAngleY("right","front"), 0.f},
        { 0.f, -GetStringAngleY("", "front"), 0.f},
        { 0.f, -GetStringAngleY("left", "front"), 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations = 
    {
        { GetStringAngleX("front",""),      0.f, GetStringAngleZ("left","down") * 0.2f},
        { GetStringAngleX("front", "down"), 0.f, GetStringAngleZ("left","down") * 0.5f},
        { GetStringAngleX("","down"),       0.f, GetStringAngleZ("left","down") },
        { GetStringAngleX("", "down"),      0.f, GetStringAngleZ("left","") }
    };  
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations = 
    {
        { GetStringAngleX("front","down") + GetStringAngleX("","up"), GetStringAngleY("","back"), GetStringAngleZ("left","down") },
        { GetStringAngleX("back","down"), 0.f, GetStringAngleZ("right","down") * 0.5f},
        { GetStringAngleX("","down"), 0.f, GetStringAngleZ("right","down") },
        { GetStringAngleX("front","down"), GetStringAngleY("right","up"), GetStringAngleZ("right","")}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations = 
    {
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations = {
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f },
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations = {
        { 0.f,  GetStringAngleY("right","front"), 0.f },
        { 0.f, GetStringAngleY("right","front"), 0.f},
        { 0.f, GetStringAngleY("", "front"), 0.f},
        { 0.f, GetStringAngleY("left", "front"), 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    _vec3 baseOffset = itemBaseRotOffset.at("sword");
    PhaseRotation RightHandRot;
    RightHandRot.name = "RHand";
    RightHandRot.phaseVec = phase;
    RightHandRot.destinations =
    {
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("","down"), 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back","down"), 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back","up"), 0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND, RightHandRot);

}
void Player::SetUpShootPhaseRotations()
{
    vector<float> phase = { 0.15f, 0.4f, 0.9f, 1.f };

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations =
    {
        StartRotations["LArm"],
        { -100.f, 0.f, -30.f },
        { -100.f, 0.f, -30.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
        StartRotations["RArm"],
        { -100.f, 0.f, 60.f },
        { -100.f, 0.f, 60.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        StartRotations["LLeg"],
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations =
    {
        StartRotations["RLeg"],
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::RLEG, RightLegRot);

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        GetComponent<TransformComponent>()->GetRotate(),
        { 0.f, 10.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, -10.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::BODY, bodyRot);
}
void Player::SetUpDeadPhaseRotations()
{
    vector<float> phase = { 0.3f, 1.f };

    PhaseRotation headPhaseRot;
    headPhaseRot.name = "Head";
    headPhaseRot.phaseVec = phase;
    headPhaseRot.destinations =
    {
        StartRotations["Head"],
        { 30.f, 0.f, 0.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::DEAD, ePlayerBone::HEAD, headPhaseRot);

    PhaseRotation LeftArmPhaseRot;
    LeftArmPhaseRot.name = "LArm";
    LeftArmPhaseRot.phaseVec = phase;
    LeftArmPhaseRot.destinations =
    {
        StartRotations["LArm"],
        { -90.f, 0.f, 0.f },
        { 0.f, -90.f, -90.f }
    };
    SetPhaseRotations(ePlayerState::DEAD, ePlayerBone::LARM, LeftArmPhaseRot);

    PhaseRotation RightArmPhaseRot;
    RightArmPhaseRot.name = "RArm";
    RightArmPhaseRot.phaseVec = phase;
    RightArmPhaseRot.destinations =
    {
        StartRotations["RArm"],
        { -90.f, 0.f, 0.f },
        { 0.f, 90.f, 90.f }
    };
    SetPhaseRotations(ePlayerState::DEAD, ePlayerBone::RARM, RightArmPhaseRot);

    PhaseRotation LeftLegPhaseRot;
    LeftLegPhaseRot.name = "LLeg";
    LeftLegPhaseRot.phaseVec = phase;
    LeftLegPhaseRot.destinations =
    {
        StartRotations["LLeg"],
        { -90.f, 0.f, 0.f },
        { 0.f, 0.f, -10.f }
    };
    SetPhaseRotations(ePlayerState::DEAD, ePlayerBone::LLEG, LeftLegPhaseRot);

    PhaseRotation RightLegPhaseRot;
    RightLegPhaseRot.name = "RLeg";
    RightLegPhaseRot.phaseVec = phase;
    RightLegPhaseRot.destinations =
    {
        StartRotations["RLeg"],
        { -90.f, 0.f, 0.f },
        { 0.f, 0.f, 10.f }
    };
    SetPhaseRotations(ePlayerState::DEAD, ePlayerBone::RLEG, RightLegPhaseRot);
}
void Player::SetAttackTypeNext()
{
    switch (attackType)
    {
    case ePlayerAttackType::FIRST:
        attackType = ePlayerAttackType::SECOND;
        break;
    case ePlayerAttackType::SECOND:
        attackType = ePlayerAttackType::LAST;
        break;
    case ePlayerAttackType::LAST:
        attackType = ePlayerAttackType::FIRST;
        break;
    }
}
void Player::EquipItem(Item::ItemType itemType)
{
    switch (itemType) {
    case Item::ItemType::ITEM_BOW:
        Bones["LHand"] = Bow::Create(owner, ObjectType::Item);
        Bones["LHand"]->GetComponent<TransformComponent>()->SetParent(Bones["LArm"]->GetComponent<TransformComponent>());
        itemBaseRotOffset.insert({ "bow", Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate() });
        break;
    case Item::ItemType::ITEM_SWORD:
        Bones["RHand"] = Sword::Create(owner, ObjectType::Item);
        Bones["RHand"]->GetComponent<TransformComponent>()->SetParent(Bones["RArm"]->GetComponent<TransformComponent>());
        itemBaseRotOffset.insert({ "sword", Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate() });
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

Player::ePlayerState Player::GetPlayerState()
{
    return State;
}

void Player::UpdateIdle(_float dt)
{
    if (comboTime < comboLimit)
    {
        comboTime += dt;
        if (comboTime >= comboLimit) attackType = ePlayerAttackType::FIRST;
        return;
    }
    
    IdleSmoothing(dt, "Head");
    IdleSmoothing(dt, "Body");

    IdleSmoothing(dt, "LArm");
    IdleSmoothing(dt, "RArm");

    IdleSmoothing(dt, "LLeg");
    IdleSmoothing(dt, "RLeg");

    IdleSmoothing(dt, "LHand");
    IdleSmoothing(dt, "RHand");
}
void Player::UpdateWalk(_float dt) {
    WalkTime += dt;

    if (comboTime < comboLimit)
    {
        comboTime += dt;
        if (comboTime >= comboLimit) attackType = ePlayerAttackType::FIRST;
    }
    //Rotate Bones
    float fAngle = sinf(WalkTime * WalkSwingSpeed);

    SetRotation({ fAngle, 0.f, 0.f }, "LLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "RLeg");
    SetRotation({ -fAngle, 0.f, 0.f }, "LArm");
    SetRotation({ fAngle, 0.f, 0.f }, "RArm");

    //Move Player
    auto transform = GetComponent<TransformComponent>();
    if (moveToAttack)
    {
        PlayerDirection = moveToObject->GetComponent<TransformComponent>()->GetWorldPosition() - transform->GetWorldPosition();
        PlayerDirection.y = 0.f;
    }
    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);

    float Speed = GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().speed;
    _vec3 moveVec =
    {
        vDir.x * Speed * Scale * dt,
        0,
        vDir.z * Speed * Scale * dt
    };

    _vec3 blockVec;
    D3DXVec3Normalize(&blockVec, &moveVec);
    float blockOffset = sqrtf(moveVec.x * moveVec.x + moveVec.z * moveVec.z);
    auto blockPos = transform->GetWorldPosition() + moveVec + blockVec * blockOffset;

    auto grid = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetStaticGrid();
    auto blockUp = grid->QueryCell(grid->WorldToCell(blockPos.x), grid->WorldToCell(blockPos.y + 1), grid->WorldToCell(blockPos.z));
    auto blockDown = grid->QueryCell(grid->WorldToCell(blockPos.x), grid->WorldToCell(blockPos.y - 1), grid->WorldToCell(blockPos.z));

    if (blockUp == nullptr && blockDown == nullptr)
    {
        transform->Translate(moveVec);
    }
    else if (blockUp == nullptr && blockDown->GetOwner()->GetObjectType() == ObjectType::StaticBlock) {
        transform->Translate(moveVec + _vec3(0.f, 2.f, 0.f));
    }

    auto blockOn1 = grid->QueryCell(grid->WorldToCell(blockPos.x - 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z - 1));
    auto blockOn2 = grid->QueryCell(grid->WorldToCell(blockPos.x - 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z + 1));
    auto blockOn3 = grid->QueryCell(grid->WorldToCell(blockPos.x + 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z - 1));
    auto blockOn4 = grid->QueryCell(grid->WorldToCell(blockPos.x + 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z + 1));

    if (blockOn1 == nullptr && blockOn2 == nullptr && blockOn3 == nullptr && blockOn4 == nullptr) GetComponent<PhysicsComponent>()->SetGround(false);

    //Rotate Player
    const float fRotateDuration = 0.05f;

    float TargetAngle = atan2f(vDir.x, vDir.z);
    _vec3 vCurRot = transform->GetRotate();

    float curAngle = NormalizeAngle(vCurRot.y);
    float deltaAngle = NormalizeAngle(TargetAngle - curAngle);

    float rotValue = deltaAngle * (dt / fRotateDuration);
    transform->SetRotate(transform->GetRotate() + _vec3{ 0.f, rotValue, 0.f });

    //CheckExit
    if (moveToAttack)
    {
        auto curPos = transform->GetWorldPosition();
        auto objectPos = moveToObject->GetComponent<TransformComponent>()->GetWorldPosition();
        AttackDirection = objectPos - curPos;

        AttackDirection.y = 0.f;
        curPos.y = 0.f;

        auto distance = sqrtf(AttackDirection.x * AttackDirection.x + AttackDirection.z * AttackDirection.z);
        if (distance <= SwordRange)
        {
            State = ePlayerState::ATTACK;
            AttackTime = 0.f;

            SaveStartRotation();
        }
        return;
    }

    auto curPos = transform->GetPosition();
    auto posGap = curPos - DestinationPos;

    auto distance = sqrtf(posGap.x * posGap.x + posGap.z * posGap.z);
    if (distance < 1.f)
    {
        State = ePlayerState::IDLE;
        WalkTime = 0.f;
    }
}
void Player::UpdateRoll(_float dt)
{
    RollTime += dt;

    //Rotate Bones
    float fProgress = RollTime / RollDuration;
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
        vDir.z * fRollSpeed * Scale * dt
    };
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition() + moveVec);

    //Rotate Player
    float fTotalRollAngle = D3DX_PI * 2.f;
    float fCurrentAngle = fTotalRollAngle * RollTime / RollDuration;

    transform->SetForward(vDir);
    _vec3 vAxis = transform->GetRight();
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, fCurrentAngle);

    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    transform->SetRotate(transform->GetRotate() + rotateVec);

    //CheckExit
    if (RollTime >= RollDuration)
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

    //Components
    auto transform = GetComponent<TransformComponent>();

    //Rotate Bones

    float fProgress = AttackTime / AttackDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);
    
    if (AttackTime == dt)
    {
        switch (attackType)
        {
        case ePlayerAttackType::FIRST:
            SetUpFirstAttackPhaseRotations();
            break;
        case ePlayerAttackType::SECOND:
            SetUpSecondAttackPhaseRotations();
            break;
        case ePlayerAttackType::LAST:
            SetUpLastAttackPhaseRotations();
            break;
        }
    }

    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND));

    //Rotate Player
    _vec3 vStartRot = _vec3{ 0.f, atan2f(-AttackDirection.x, AttackDirection.z), 0.f };
    _vec3 vCurrentRot = GetPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY));
    _vec3 vDeltaRot = vCurrentRot - vStartRot;

    transform->SetRotate(vDeltaRot);

    //CheckExit
    if (AttackTime >= AttackDuration)
    {
        AttackTime = 0.f;
        comboTime = 0.f;
        SetAttackTypeNext();
        if (moveToAttack)
        {
            moveToAttack = false;
            moveToObject = nullptr;

            State = ePlayerState::IDLE;
            return;
        }
        else if (WalkTime == 0.f)
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

    //Components
    auto transform = GetComponent<TransformComponent>();

    //shoot arrow
    float fProgress = std::clamp(AttackTime / ShootDuration, 0.f, 1.f);


    vector<float> phaseVec = { 0.15f, 0.4f, 0.9f, 1.f };
    static float prePhase = 0.f;
    if (prePhase < phaseVec.at(0) && fProgress >= phaseVec.at(0)) {
        auto shootDir = AttackDirection;
        D3DXVec3Normalize(&shootDir, &shootDir);
        Arrow::Create(owner, ObjectType::Projectile, this, shootDir);
    }
    prePhase = fProgress;

    //Rotate Bones

    if (AttackTime == dt)
    {
        SetUpShootPhaseRotations();
    }

    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::LARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::RARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::LLEG));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::RLEG));

    //Rotate Player

    _vec3 vCurrentRot = GetPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::BODY));
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
    if (AttackTime >= ShootDuration) {
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

    //Components
    auto physics = GetComponent<PhysicsComponent>();
    auto transform = GetComponent<TransformComponent>();

    //Fix Dead State
    if (DeadTime >= DeadDuration) {
        DeadTime = DeadDuration;
        State = ePlayerState::DEAD;
        physics->SetGround(true);
        return;
    }

    //Rotate Bones
    float fProgress = std::clamp(DeadTime / DeadDuration, 0.f, 1.f);

    if (DeadTime == dt)
    {
        SetUpDeadPhaseRotations();
    }

    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::DEAD, ePlayerBone::HEAD));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::DEAD, ePlayerBone::LARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::DEAD, ePlayerBone::RARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::DEAD, ePlayerBone::LLEG));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::DEAD, ePlayerBone::RLEG));

    //Rotate Player
    float fMaxDeathAngle = D3DXToRadian(90.f);
    float fCurrentAngle = fMaxDeathAngle * fProgress;

    _vec3 moveVec = PlayerDirection;
    moveVec.y = 0.f;
    D3DXVec3Normalize(&moveVec, &moveVec);

    transform->SetForward(moveVec);
    _vec3 vAxis = transform->GetRight();
    _matrix matRot;
    D3DXMatrixRotationAxis(&matRot, &vAxis, -fCurrentAngle);
    _vec3 rotateVec = MatrixToEulerAngles(matRot);
    transform->SetRotate(transform->GetRotate() + rotateVec);

    //Lerp y to floor 
    auto collision = GetComponent<CollisionComponent>();
    float fSinT = sinf(D3DX_PI / 2.f + (D3DX_PI / 2.f) * fProgress);
    float fLerpY = 1.f + (7.f - 1.f) * fSinT;
    collision->SetSize(_vec3(2.f, fLerpY, 2.f));
    physics->SetGround(false);
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
    static const KEY keyRoll = SPACE;

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
    if (GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().curHp <= 0.f && DeadTime == 0)
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
    //ObjectType objType = other->GetObjectType();
    //auto collision = GetComponent<CollisionComponent>();
    //
    //if (objType == ObjectType::StaticBlock)
    //    collision->ResolveAABBColiision(other);
}

void Player::IdleSmoothing(_float dt, std::string bone)
{
    if (Bones[bone] == nullptr) return;

    _vec3 vTargetRot;
    if (bone == "RHand" && itemBaseRotOffset.find("sword") != itemBaseRotOffset.end())
    {
        vTargetRot = itemBaseRotOffset.at("sword");
    }
    else if (bone == "LHand" && itemBaseRotOffset.find("bow") != itemBaseRotOffset.end())
    {
        vTargetRot = itemBaseRotOffset.at("bow");
    }
    else
    {
        vTargetRot = { 0.f,0.f,0.f };
    }

    if (bone == "Body")
    {
        vTargetRot = { 0.f,atan2f(PlayerDirection.x,PlayerDirection.z),0.f};
        _vec3 vCurrentRot = GetComponent<TransformComponent>()->GetRotate();
        _vec3 vLerpedRot = vCurrentRot + (vTargetRot - vCurrentRot) * dt * IdleSmoothingSpeed;
        GetComponent<TransformComponent>()->SetRotate(vLerpedRot);
        return;
    }

    _vec3 vCurrentRot = Bones[bone]->GetComponent<TransformComponent>()->GetRotate();
    _vec3 vLerpedRot = vCurrentRot + (vTargetRot - vCurrentRot) * dt * IdleSmoothingSpeed;
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

_vec3 Player::GetPhasedRotation(float fProgress, PhaseRotation& phaseRot)
{
    int iPhaseCount = min(static_cast<int>(phaseRot.phaseVec.size()), static_cast<int>(phaseRot.destinations.size()) - 1);

    if (fProgress <= phaseRot.phaseVec.front())
    {
        float ratio = fProgress / phaseRot.phaseVec.front();
        return DegToRadLerp(phaseRot.destinations[0], phaseRot.destinations[1], ratio);
    }

    for (int i = 1; i < iPhaseCount; ++i)
    {
        if (fProgress <= phaseRot.phaseVec[i])
        {
            float ratio = (fProgress - phaseRot.phaseVec[i - 1]) / (phaseRot.phaseVec[i] - phaseRot.phaseVec[i - 1]);
            return DegToRadLerp(phaseRot.destinations[i], phaseRot.destinations[i + 1], ratio);
        }
    }

    if (static_cast<int>(phaseRot.destinations.size()) > iPhaseCount + 1)
    {
        float ratio = (fProgress - phaseRot.phaseVec[iPhaseCount - 1]) / (1.0f - phaseRot.phaseVec[iPhaseCount - 1]);
        return DegToRadLerp(phaseRot.destinations[iPhaseCount], phaseRot.destinations[iPhaseCount + 1], ratio);
    }
    else
    {
        float ratio = (fProgress - phaseRot.phaseVec[iPhaseCount - 1]) / (1.0f - phaseRot.phaseVec[iPhaseCount - 1]);
        return DegToRadLerp(phaseRot.destinations[iPhaseCount], phaseRot.destinations[0], ratio);
    }
}

void Player::ApplyPhasedRotation(float fProgress, PhaseRotation& phaseRot)
{
    _vec3 vCurrentRot = GetPhasedRotation(fProgress, phaseRot);
    SetRotation(vCurrentRot, phaseRot.name);
}

void Player::SetPhaseRotations(const ePlayerState& state, const ePlayerBone& bone, PhaseRotation& phaseRot)
{
    PhaseRotations[{static_cast<int>(state), static_cast<int>(bone)}] = phaseRot;
}

Player::PhaseRotation& Player::GetPhaseRotations(const ePlayerState& state, const ePlayerBone& bone)
{
    return PhaseRotations.at({ static_cast<int>(state), static_cast<int>(bone) });
}

float Player::GetStringAngleX(const string& frontBack, const string& upDown, bool clockwise, float offset)
{
    float fAngle = 0.f;

    if (frontBack == "front")
    {
        fAngle += -90;
    }
    if (frontBack == "back")
    {
        fAngle += 90;
    }

    if (frontBack == "") {
        if (upDown == "up") fAngle = -180.f;
        else if (upDown == "down") fAngle = 0.f;
    }
    else
    {
        if (upDown == "up")
        {
            fAngle *= 1.5f;
        }
        if (upDown == "down") {
            fAngle /= 2.f;
        }
    }
    fAngle += offset;

    return clockwise ? fAngle : -fAngle;
}

float Player::GetStringAngleZ(const string& leftRight, const string& upDown, bool clockwise, float offset)
{
    float fAngle = 0.f;

    if (leftRight == "left")
    {
        fAngle += -90;
    }
    if (leftRight == "right")
    {
        fAngle += 90;
    }

    if (leftRight == "") {
        if (upDown == "up") fAngle = -180.f;
        else if (upDown == "down") fAngle = 0.f;
    }
    else
    {
        if (upDown == "up")
        {
            fAngle *= 1.5f;
        }
        if (upDown == "down") {
            fAngle /= 2.f;
        }
    }
    fAngle += offset;

    return clockwise ? fAngle : -fAngle;
}

float Player::GetStringAngleY(const string& leftRight, const string& frontBack, bool clockwise, float offset)
{
    float fAngle = 0.f;

    if (leftRight == "left")
    {
        fAngle += -90;
    }
    if (leftRight == "right")
    {
        fAngle += 90;
    }

    if (leftRight == "") {
        if (frontBack == "back") fAngle = -180.f;
        else if (frontBack == "front") fAngle = 0.f;
    }
    else
    {
        if (frontBack == "back")
        {
            fAngle *= 1.5f;
        }
        if (frontBack == "front") {
            fAngle /= 2.f;
        }
    }
    fAngle += offset;

    return clockwise ? fAngle : -fAngle;
}
