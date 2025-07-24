#include "pch.h"
#include "Player.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "InputSystem.h"
#include "InfoComponent.h"
#include "ItemComponent.h"
#include "UIRenderer.h"

#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "EngineCore.h"
#include "RenderSystem.h"
#include "SceneManager.h"
#include "Scene.h"
#include "CameraComponent.h"
#include "CameraManager.h"
#include "UIManager.h"
#include "DialogManager.h"
#include "UILoader.h"

#include "MyMath.h"

#include "Sword.h"
#include "Bow.h"
#include "Arrow.h"
#include "Firework.h"
#include "Spear.h"
#include "Crossbow.h"
#include "Armor.h"
#include "EmeraldObj.h"
#include "DashUI.h"
#include "CollisionBlock.h"

#include "StaticGrid.h"
#include "SpriteRenderer.h"
#include "SpriteEffect.h"
#include "StatikkEffect.h"
#include "ChargeDownEffect.h"
#include "ChargeOnEffect.h"
#include "ChargeFrontEffect.h"
#include "RollEffect.h"

#include "SoundManager.h"
#include "Npc.h"
#include "DialogComponent.h"
#include "FirstCam.h"
#include "ThirdCam.h"
#include "FreecamComponent.h"
#include "ThirdcamComponent.h"

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

    for (auto& bone : Bones)
    {
        if (bone.second)
            bone.second->AddRef();
    }

    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    auto collision = AddComponent<CollisionComponent>();
    collision->AddCollider<OBBCollider>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);

    collision->SetLayer(LAYER_PLAYER);
    collision->SetMask(LAYER_ENEMY | LAYER_DEFAULT | LAYER_PROJECTILE |LAYER_TRRIGER);
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetCollisionStay([this](Object* other) {this->OnCollisionStay(other);});

    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
    physics->SetMass(1.f);

    auto playerInfo = AddComponent<InfoComponent<PlayerInfo>>();

    SetScale(1.f);

    SetMaterial("playerBody_Mtrl",     "Body");
    SetMaterial("playerHead_Mtrl",     "Head");
    SetMaterial("playerLeftArm_Mtrl",  "LArm");
    SetMaterial("playerRightArm_Mtrl", "RArm");
    SetMaterial("playerLeftLeg_Mtrl",  "LLeg");
    SetMaterial("playerRightLeg_Mtrl", "RLeg");

    Bones["LHand"] = nullptr;
    Bones["RHand"] = nullptr;
    Bones["Armor"] = nullptr;

    SetUpIdleRotations();
    UpdateNewIdleRotations();

    invMgr = GetScene()->GetUIManager()->GetInventory();
    invComp = AddComponent<InventoryComponent>();

    return S_OK;
}
void Player::Update(_float dt)
{
    auto curRenderType = EngineCore::GetInstance()->GetRenderSystem()->GetCurRenderState();
    if (curRenderType == UIRenderType::Inventory || curRenderType == UIRenderType::WorldMap ||
        curRenderType == UIRenderType::QuestUI || curRenderType == UIRenderType::Shop) return;

    BaseCharacter::Update(dt);

    KeyInput(dt);
    UpdateNewIdleRotations();

    switch (State) 
    {
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
    case ePlayerState::REVIVE:
        UpdateRevive(dt);
        break;
    }
    CheckTargetDead();
    UpdateFloat(dt);
    SaveBeforeCollisionVelocity(dt);
}

void Player::Late_Update(_float dt)
{
    const auto& input = EngineCore::GetInstance()->GetInputSystem();
    if (input->IsKeyPressed(KEY::I))
    {
        bool enableInventory = (EngineCore::GetInstance()->GetRenderSystem()->GetCurRenderState() == UIRenderType::MainGame);
        SetInventoryMode(enableInventory);
    }
    if (input->IsKeyPressed(KEY::S))
    {
        bool enableShop = (EngineCore::GetInstance()->GetRenderSystem()->GetCurRenderState() == UIRenderType::MainGame);
        SetShopMode(enableShop);
    }
    if (isClickedI)
    {
        const auto& allItemTypes = InventoryComponent::GetSupportedItemTypes();
        for (const auto& itemType : allItemTypes)
            invComp->Add(itemType);

        isClickedI = false;
    }

    // ---------------------------------------- 
    BaseCharacter::Late_Update(dt);
    if (moveToInteract && moveToObject)
    {
        auto npcTf = moveToObject->GetComponent<TransformComponent>();
        auto myTf = GetComponent<TransformComponent>();

        _vec3 toNpc = npcTf->GetWorldPosition() - myTf->GetWorldPosition();
        toNpc.y = 0.f;

        float distance = D3DXVec3Length(&toNpc);
        if (distance <= interactRange)
        {
            auto dialogMgr = GetScene()->GetUIManager()->GetDialog();
            if (!dialogMgr->IsTalking())
            {
                InteractWithNPC(moveToObject);

                moveToInteract = false;
                moveToObject = nullptr;
                State = ePlayerState::IDLE;
            }
        }
    }
    SaveAfterCollisionVelocity(dt);
    CheckFallHit(dt);
}

void Player::Free()
{
    BaseCharacter::Free();
}

void Player::PickingTerrain()
{
    if (State == ePlayerState::DEAD   ||
        State == ePlayerState::ROLL   ||
        State == ePlayerState::ATTACK ||
        State == ePlayerState::SHOOT  ||
        State == ePlayerState::REVIVE)
        return;
   
    if (EngineCore::GetInstance()->GetRenderSystem()->GetCurRenderState() == UIRenderType::Inventory) return;
        

    auto input = EngineCore::GetInstance()->GetInputSystem();
    auto curScene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
    auto mainCam = curScene->GetCameraManager()->GetMainCamera();
    auto collision = curScene->GetCollisionSystem();
    auto grid = curScene->GetStaticGrid();

    if (input->IsKeyPressed(Z) || input->IsKeyDown(LBUTTON))
    {
        Ray ray = mainCam->ScreenPointRay();

        //////////////////////////////////////////////Picking Change
        HitInfo hit;
        HitInfo objectHit = collision->Raycast(ray);
        HitInfo terrainHit = grid->RayCast(ray);
        hit = (objectHit.Distance <= terrainHit.Distance) ? objectHit : terrainHit;
        //////////////////////////////////////////////
        if (hit.IsHit)
        {
            if ( (input->IsKeyPressed(Z) || hit.Component->GetLayer() == LAYER_ENEMY)    &&
                Bones["RHand"] != nullptr                   )
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
                auto distance = sqrtf(PlayerDirection.x * PlayerDirection.x + PlayerDirection.y * PlayerDirection.y + PlayerDirection.z * PlayerDirection.z);
                
                if (distance <= MeleeRange)
                {
                    EngineCore::GetInstance()->GetSoundManager()->PlaySFX("SwingSword");
                    State = ePlayerState::ATTACK;
                    AttackTime = 0.f;

                    SaveStartRotation();
                    curPos = transform->GetWorldPosition();
                    auto attackPos = hit.Position;
                    AttackDirection = attackPos - curPos;
                }
            }
            else
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

        if ((State == ePlayerState::IDLE || State == ePlayerState::WALK) &&
            hit.IsHit && hit.Component->GetLayer() == CollisionLayer::LAYER_NPC)
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

            moveToObject = hit.Component->GetOwner();
            moveToInteract = true;

            auto transform = GetComponent<TransformComponent>();
            auto curPos = transform->GetWorldPosition();
            DestinationPos = hit.Position;

            DestinationPos.y = 0.f;
            curPos.y = 0.f;
            PlayerDirection = DestinationPos - curPos;
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

    if (input->IsKeyDown(RBUTTON) && Bones["LHand"] != nullptr)
    {
        Ray ray = mainCam->ScreenPointRay();
        //////////////////////////////////////////////Picking Change
        HitInfo hit;
        HitInfo objectHit = collision->Raycast(ray);
        HitInfo terrainHit = grid->RayCast(ray);
        hit = (objectHit.Distance <= terrainHit.Distance) ? objectHit : terrainHit;
        //////////////////////////////////////////////

        if (State == ePlayerState::IDLE || State == ePlayerState::WALK)
        {
            auto transform = GetComponent<TransformComponent>();
            auto collision = GetComponent<CollisionComponent>();
            auto curPos = transform->GetWorldPosition();
            auto attackPos = hit.Position;
            
            AttackDirection = attackPos - curPos;
            if (State == ePlayerState::IDLE)
                PlayerDirection = attackPos - curPos;;

            State = ePlayerState::SHOOT;
            AttackTime = 0.f;
            
            SaveStartRotation();
            
            if(Bones["LHand"])
                Bones["LHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_Alpha);

            if (Bones["RHand"])
                Bones["RHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_None);
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

void Player::SetUpSwordFirstAttackPhaseRotations()
{
    vector<float> phase = { 0.1f, 0.45f, 1.f };

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        {0.f, 0.f, 0.f },
        {0.f, GetStringAngleY("right","front"), 0.f},
        {0.f, 0.f, 0.f},
        {0.f, GetStringAngleY("left","front"),  0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        { 0.f, 0.f, 0.f },
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
        { GetStringAngleX("","down"),        0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("","down"),        0.f, GetStringAngleZ("left","down")}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
        StartRotations["RArm"],
        { GetStringAngleX("","up"),         0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("front","up"),    0.f, GetStringAngleZ("left","down") },
        { GetStringAngleX("front","down"),  0.f, GetStringAngleZ("left","down") }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        StartRotations["LLeg"],
        { GetStringAngleX("front","down") * 0.5f,    0.f, 0.f },
        { 0.f,                                       0.f,-5.f },
        { GetStringAngleX("back","down") * 0.5f,     0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations =
    {
        StartRotations["RLeg"],
        { GetStringAngleX("back","down")  * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

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

void Player::SetUpSwordSecondAttackPhaseRotations() {
    vector<float> phase = { 0.1f, 0.45f, 1.f };

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        {0.f, -GetStringAngleY("left","front"),  0.f},
        {0.f, -GetStringAngleY("left","front"),  0.f },
        {0.f, -GetStringAngleY("","front"),      0.f },
        {0.f, -GetStringAngleY("right","front"), 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations = 
    {
        { GetStringAngleX("back","down"),     0.f, GetStringAngleZ("left","down") * 0.2f},
        { GetStringAngleX("","down"),         0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down"),    0.f, GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down"),    0.f, GetStringAngleZ("left","")}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations = 
    {
        { GetStringAngleX("front","down"),  0.f,                           GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down"),     GetStringAngleY("","back"), GetStringAngleZ("left","down") },
        { GetStringAngleX("front","up") *0.75f,GetStringAngleY("","back"), GetStringAngleZ("left","down")},
        { GetStringAngleX("front","down") +    GetStringAngleX("","up"),   GetStringAngleY("","back"),          GetStringAngleZ("left","down") }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f },
        { GetStringAngleX("back","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, -5.f },
        { GetStringAngleX("front","down")* 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations = 
    {
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { GetStringAngleX("back","down")  * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        {0.f, GetStringAngleY("left","front"),   0.f},
        {0.f, GetStringAngleY("left","front"),   0.f },
        {0.f,  GetStringAngleY("","front"),      0.f },
        {0.f,  GetStringAngleY("right","front"), 0.f }
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

void Player::SetUpSwordLastAttackPhaseRotations()
{
    vector<float> phase = { 0.1f, 0.45f, 1.f };

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        { 0.f, -GetStringAngleY("right","front"), 0.f },
        { 0.f, -GetStringAngleY("right","front"), 0.f},
        { 0.f, -GetStringAngleY("", "front"),     0.f},
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
        { GetStringAngleX("","down"),     0.f, GetStringAngleZ("right","down") },
        { GetStringAngleX("front","down"),     GetStringAngleY("right","up"), GetStringAngleZ("right","")}
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
        { GetStringAngleX("back","down")  * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations = 
    {
        { GetStringAngleX("back","down")  * 0.5f, 0.f, 0.f },
        { GetStringAngleX("back","down")  * 0.5f, 0.f, 0.f },
        { 0.f, 0.f, 5.f },
        { GetStringAngleX("front","down") * 0.5f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
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
        { GetStringAngleX("front",""), 0.f, GetStringAngleZ("","down")},
        { GetStringAngleX("front",""), 0.f, GetStringAngleZ("","down")},
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
        StartRotations["RArm"],
        { GetStringAngleX("front","up") * 0.9f, GetStringAngleY("right",""), GetStringAngleZ("right","")},
        { GetStringAngleX("front","") * 0.9f, 0.f, GetStringAngleZ("right","down") },
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
        D3DXToDegree(StartRotations["Body"]),
        { 0.f, 10.f, 0.f },
        { 0.f, 0.f, 0.f },
        { 0.f, -10.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::BODY, bodyRot);

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        D3DXToDegree(StartRotations["Head"]),
        { 0.f, 0.f, 0.f},
        { 0.f,  0.f, 0.f},
        { 0.f,  0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::HEAD, headRot);
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

void Player::SetUpRevivePhaseRotations()
{
    vector<float> phase = { 0.6f, 1.f };

    PhaseRotation LeftArmPhaseRot; 
    LeftArmPhaseRot.name = "LArm";
    LeftArmPhaseRot.phaseVec = phase;
    LeftArmPhaseRot.destinations =
    {
        { 0.f, -90.f, -90.f },
        { 0.f, -45.f, -45.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::LARM, LeftArmPhaseRot);

    PhaseRotation RightArmPhaseRot;
    RightArmPhaseRot.name = "RArm";
    RightArmPhaseRot.phaseVec = phase;
    RightArmPhaseRot.destinations =
    {
        { 0.f, 90.f, 90.f },
        { 0.f, 45.f, 45.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::RARM, RightArmPhaseRot);

    PhaseRotation LeftLegPhaseRot;
    LeftLegPhaseRot.name = "LLeg";
    LeftLegPhaseRot.phaseVec = phase;
    LeftLegPhaseRot.destinations =
    {
        { 0.f, 0.f, -10.f },
        { 0.f, 0.f, -10.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::LLEG, LeftLegPhaseRot);

    PhaseRotation RightLegPhaseRot;
    RightLegPhaseRot.name = "RLeg";
    RightLegPhaseRot.phaseVec = phase;
    RightLegPhaseRot.destinations =
    {
        { 0.f, 0.f, 10.f },
        { 0.f, 0.f, 10.f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::RLEG, RightLegPhaseRot);
}

void Player::SetUpSpearFirstAttackPhaseRotations()
{
    vector<float> phase = { 0.2f, 0.3f, 1.f };

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        {0.f, 60.f, 0.f},
        {0.f, 60.f, 0.f},
        {0.f, GetStringAngleY("","front"), 0.f},
        {0.f, 60.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        {0.f, -60.f, 0.f},
        { 0.f, -60.f, 0.f},
        { 0.f, -GetStringAngleY("","front"), 0.f},
        {0.f, -60.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations =
    {
        D3DXToDegree(StartRotations["LArm"]),
        { GetStringAngleX("front","down"),   GetStringAngleY("left",""), GetStringAngleZ("left","down")},
        { GetStringAngleX("front","") * 0.8f,        GetStringAngleY("left","") * 0.75f, GetStringAngleZ("right","") * 0.75f},
        D3DXToDegree(StartRotations["LArm"])
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
       D3DXToDegree(StartRotations["RArm"]),
        { GetStringAngleX("front","down"),   GetStringAngleY("left",""), GetStringAngleZ("","down")},
        { GetStringAngleX("front","") * 0.8f,    GetStringAngleY("left","") * 0.75f, GetStringAngleZ("left","") * 0.75f},
        D3DXToDegree(StartRotations["RArm"])
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        D3DXToDegree(StartRotations["LLeg"]),
        { GetStringAngleX("front","down") * 0.2f, 0.f, GetStringAngleZ("left","down") * 0.2f },
        { GetStringAngleX("back","down") * 0.2f, 0.f, GetStringAngleZ("left","down") * 0.2f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations =
    {
        D3DXToDegree(StartRotations["RLeg"]),
        { GetStringAngleX("back","down") * 0.2f, 0.f, GetStringAngleZ("right","down") * 0.2f },
        { GetStringAngleX("front","down") * 0.2f, 0.f, GetStringAngleZ("right","down") * 0.2f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    _vec3 baseOffset = D3DXToDegree(itemBaseRotOffset.at("spear"));
    PhaseRotation RightHandRot;
    RightHandRot.name = "RHand";
    RightHandRot.phaseVec = phase;
    RightHandRot.destinations =
    {
        {baseOffset.x, 0.f, 0.f },
        {baseOffset.x, 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x , 0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND, RightHandRot);
}

void Player::SetUpSpearSecondAttackPhaseRotations()
{
    vector<float> phase = { 0.2f, 0.3f, 1.f };

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        {0.f, 60.f, 0.f},
        {0.f, 60.f, 0.f},
        {0.f, GetStringAngleY("","front"), 0.f},
        {0.f, 60.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        {0.f, -60.f, 0.f},
        { 0.f, -60.f, 0.f},
        { 0.f, -GetStringAngleY("","front"), 0.f},
        {0.f, -60.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations =
    {
        D3DXToDegree(StartRotations["LArm"]),
        { GetStringAngleX("front","down"),   GetStringAngleY("left",""), GetStringAngleZ("left","down")},
        { GetStringAngleX("","down"),       GetStringAngleY("left",""), GetStringAngleZ("left","down")},
        D3DXToDegree(StartRotations["LArm"])
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
       D3DXToDegree(StartRotations["RArm"]),
        { GetStringAngleX("front","down") * 0.5f,   GetStringAngleY("left",""), GetStringAngleZ("right","down")},
        { GetStringAngleX("front",""),    GetStringAngleY("left",""), GetStringAngleZ("","down") },
        D3DXToDegree(StartRotations["RArm"])
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        D3DXToDegree(StartRotations["LLeg"]),
        { GetStringAngleX("front","down") * 0.3f,    0.f, GetStringAngleZ("left","down") * 0.3f },
        { GetStringAngleX("back","down") * 0.3f,    0.f, GetStringAngleZ("left","down") * 0.3f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations =
    {
        D3DXToDegree(StartRotations["RLeg"]),
        { GetStringAngleX("back","down") * 0.3f, 0.f, GetStringAngleZ("right","down") * 0.3f },
        { GetStringAngleX("front","down") * 0.3f, 0.f, GetStringAngleZ("right","down") * 0.3f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    _vec3 baseOffset = D3DXToDegree(itemBaseRotOffset.at("spear"));
    PhaseRotation RightHandRot;
    RightHandRot.name = "RHand";
    RightHandRot.phaseVec = phase;
    RightHandRot.destinations =
    {
        {baseOffset.x, 0.f, 0.f },
        {baseOffset.x + GetStringAngleX("front","") * 0.2f, 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x , 0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND, RightHandRot);
}

void Player::SetUpSpearLastAttackPhaseRotations()
{
    vector<float> phase = { 0.2f, 0.3f, 1.f };

    PhaseRotation bodyRot;
    bodyRot.name = "Body";
    bodyRot.phaseVec = phase;
    bodyRot.destinations =
    {
        {0.f, 60.f, 0.f},
        {0.f, 60.f, 0.f},
        {0.f, GetStringAngleY("left","front"), 0.f},
        {0.f, 60.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY, bodyRot);

    PhaseRotation headRot;
    headRot.name = "Head";
    headRot.phaseVec = phase;
    headRot.destinations =
    {
        {0.f, -60.f, 0.f},
        { 0.f, -60.f, 0.f},
        { 0.f, -GetStringAngleY("left","front"), 0.f},
        {0.f, -60.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::HEAD, headRot);

    PhaseRotation LeftArmRot;
    LeftArmRot.name = "LArm";
    LeftArmRot.phaseVec = phase;
    LeftArmRot.destinations =
    {
        D3DXToDegree(StartRotations["LArm"]),
       { GetStringAngleX("front","down"),   GetStringAngleY("left",""), GetStringAngleZ("left","down")},
        { GetStringAngleX("back","down"),    GetStringAngleY("left",""), GetStringAngleZ("left","down")},
        D3DXToDegree(StartRotations["LArm"])
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LARM, LeftArmRot);

    PhaseRotation RightArmRot;
    RightArmRot.name = "RArm";
    RightArmRot.phaseVec = phase;
    RightArmRot.destinations =
    {
       D3DXToDegree(StartRotations["RArm"]),
        { GetStringAngleX("front","down") * 0.5f,   GetStringAngleY("left","front"), GetStringAngleZ("right","down")},
        { GetStringAngleX("front","") * 0.8f,    GetStringAngleY("left","front"), GetStringAngleZ("right","down") },
        D3DXToDegree(StartRotations["RArm"])
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RARM, RightArmRot);

    PhaseRotation LeftLegRot;
    LeftLegRot.name = "LLeg";
    LeftLegRot.phaseVec = phase;
    LeftLegRot.destinations =
    {
        D3DXToDegree(StartRotations["LLeg"]),
        { GetStringAngleX("front","down") * 0.3f,    0.f, GetStringAngleZ("left","down") * 0.3f },
        { GetStringAngleX("back","down") * 0.3f,    0.f, GetStringAngleZ("right","down") * 0.3f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::LLEG, LeftLegRot);

    PhaseRotation RightLegRot;
    RightLegRot.name = "RLeg";
    RightLegRot.phaseVec = phase;
    RightLegRot.destinations =
    {
        D3DXToDegree(StartRotations["RLeg"]),
        { GetStringAngleX("back","down") * 0.3f, 0.f, GetStringAngleZ("right","down") * 0.3f },
        { GetStringAngleX("front","down") * 0.3f, 0.f, GetStringAngleZ("left","down") * 0.3f },
        { 0.f, 0.f, 0.f }
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RLEG, RightLegRot);

    phase = { 0.2f, 0.3f, 0.8f, 1.f };

    _vec3 baseOffset = D3DXToDegree(itemBaseRotOffset.at("spear"));
    PhaseRotation RightHandRot;
    RightHandRot.name = "RHand";
    RightHandRot.phaseVec = phase;
    RightHandRot.destinations =
    {
        {baseOffset.x, 0.f, 0.f },
        {baseOffset.x + GetStringAngleX("front","") * 0.2f, 0.f, 0.f},
        {baseOffset.x + GetStringAngleX("back",""), 0.f, 0.f},
        {baseOffset.x , 0.f, 0.f},
        {baseOffset.x , 0.f, 0.f}
    };
    SetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::RHAND, RightHandRot);
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

void Player::SetUpIdleRotations()
{
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::BODY)}] = { 0.f, 0.f, 0.f };
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::HEAD)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::LARM)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::RARM)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::LLEG)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::RLEG)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::LHAND)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::RHAND)}] = _vec3(0.f, 0.f, 0.f);

    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::BODY)}] = { 0.f, 0.f, 0.f };
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::HEAD)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::LARM)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::RARM)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::LLEG)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::RLEG)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::LHAND)}] = _vec3(0.f, 0.f, 0.f);

    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::BODY)}] = { 0.f, D3DXToRadian(60.f), 0.f};
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::HEAD)}] = _vec3(0.f, -D3DXToRadian(60.f), 0.f);
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::LARM)}] = _vec3(D3DXToRadian(GetStringAngleX("front", "down") * 0.5f), D3DXToRadian(GetStringAngleY("left", "")), D3DXToRadian(GetStringAngleZ("right", "down") * 0.2f));
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::RARM)}] = _vec3(D3DXToRadian(GetStringAngleX("front","down") * 0.5f), D3DXToRadian(GetStringAngleY("left","")), D3DXToRadian(GetStringAngleZ("right", "down") * 0.2f));
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::LLEG)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::RLEG)}] = _vec3(0.f, 0.f, 0.f);
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::LHAND)}] = _vec3(0.f, 0.f, 0.f);
}

void Player::UpdateNewIdleRotations()
{
    idleRot[{static_cast<int>(ItemType::None), static_cast<int>(ePlayerBone::BODY)}] = { 0.f, atan2f(PlayerDirection.x, PlayerDirection.z), 0.f };
    idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::BODY)}] = { 0.f, atan2f(PlayerDirection.x, PlayerDirection.z), 0.f };
    idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::BODY)}] = { 0.f, atan2f(PlayerDirection.x, PlayerDirection.z) + D3DXToRadian(60.f), 0.f };
    
    if (itemBaseRotOffset.find("sword") != itemBaseRotOffset.end())
        idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::RHAND)}] = itemBaseRotOffset.at("sword");
    else
        idleRot[{static_cast<int>(ItemType::Sword), static_cast<int>(ePlayerBone::RHAND)}] = _vec3(0.f, 0.f, 0.f);

    if (itemBaseRotOffset.find("spear") != itemBaseRotOffset.end())
        idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::RHAND)}] = itemBaseRotOffset.at("spear");
    else
        idleRot[{static_cast<int>(ItemType::Spear), static_cast<int>(ePlayerBone::RHAND)}] = _vec3(0.f, 0.f, 0.f);
}

void Player::CamRotTest(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static const KEY keyLeft = LEFT;
    static const KEY keyRight = RIGHT;

    if (input->IsKeyDown(keyLeft) || input->IsKeyDown(keyRight))
    {
        auto mainCam = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetCameraManager()->GetMainCamera();

        auto camTransform = mainCam->GetTarget();
        auto playerTransform = GetComponent<TransformComponent>();

        auto camObject = mainCam->GetOwner();
        
        if (dynamic_cast<ThirdCam*>(camObject))
        {
            auto thirdCam = camObject->GetComponent<ThirdcamComponent>();

            _vec3 playerPos = playerTransform->GetWorldPosition();
            _vec3 camPos = camTransform->GetWorldPosition();
            _vec3 distance = camPos - playerPos;

            _vec3 playerUp = playerTransform->GetUp();
            _vec3 camRight = camTransform->GetRight();

            _float fAngle;
            if (input->IsKeyDown(keyLeft))
            {
                fAngle = D3DXToRadian(180.f * dt);
            }
            else if (input->IsKeyDown(keyRight))
            {
                fAngle = D3DXToRadian(-180.f * dt);
            }
            else
            {
                fAngle = 0.f;
            }

            _matrix matRot;
            D3DXMatrixRotationAxis(&matRot, &playerUp, fAngle);

            _vec3 newPos;
            D3DXVec3TransformCoord(&newPos, &distance, &matRot);
            thirdCam->SetOffset(newPos);
        }
        //else if (dynamic_cast<FirstCam*>(camObject))
        //{
        //    camObject->GetComponent<FreecamComponent>();
        //}
        else
            return;
    }
}

void Player::CheckJump()
{
    if (State != ePlayerState::IDLE && State != ePlayerState::WALK)
        return;

    auto input = EngineCore::GetInstance()->GetInputSystem();
    static const KEY keyJump = X;

    auto physics = GetComponent<PhysicsComponent>();
    if (input->IsKeyPressed(keyJump) && physics->IsGrounded() == true)
    {
        _vec3 velocity = physics->GetVelocity();
        velocity.y = jumpPower;
        physics->SetVelocity(velocity);
        physics->SetGround(false);
    }
}

void Player::CheckTargetDead()
{
    if (moveToObject && moveToObject->IsDead())
    {
        moveToObject = nullptr;
        moveToAttack = false;
    }
}

void Player::CheckSkill()
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static const KEY keyStatikk = C;
    static const KEY keyFirework = V;
    static const KEY keyFloat = B;

    auto physics = GetComponent<PhysicsComponent>();
    if (input->IsKeyPressed(keyStatikk))
    {
        SetStatikkMode(true);
    }
    if (input->IsKeyPressed(keyFirework))
    {
        ChangeShootType();
    }
    if (input->IsKeyPressed(keyFloat))
    {
        SetFloatMode(true);
    }
}

void Player::ResetWalkTimer()
{
    if (State != ePlayerState::WALK)
        walkEffectTimer = 0.f;
}

void Player::UpdateFloat(_float dt)
{
    if (!FloatMode) return;
    FloatTimer += dt;

    auto physics = GetComponent<PhysicsComponent>();
    physics->SetMass(0.f);

    _vec3 velocity = physics->GetVelocity();
    velocity.y = 0.f;
    physics->SetVelocity(velocity);

    auto offset =  _vec3(0.f, 1.f, 0.f) * dt * FloatingSpeed;
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition() + offset);

    if (FloatTimer > FloatDuration)
    {
        SetFloatMode(false);
        physics->SetMass(1.f);
    }
}

void Player::PullingEmerald(_float dt)
{
    _int emeraldCount = 0;
    _vec3 playerPos = GetComponent<TransformComponent>()->GetWorldPosition();
    auto itemList = owner->GetObjectList(ObjectType::Item);
    for (auto& item : itemList)
    {
        auto emerald = dynamic_cast<EmeraldObj*>(item);
        if (!emerald) continue;

        auto itemTransform = item->GetComponent<TransformComponent>();
        _vec3 itemPos = itemTransform->GetWorldPosition();
        _vec3 dir = playerPos - itemPos;
        _float distance = sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        
        if (distance < 5.f)
        {
            item->SetDead();
            ++emeraldCount;
        }
        else if(distance < itemPullingRange)
        {
            D3DXVec3Normalize(&dir, &dir);
            itemTransform->SetPosition(itemPos + dir * dt * itemPullingSpeed);
        }
    }
    if (emeraldCount != 0)
    {
        auto playerInfo = GetComponent<InfoComponent<PlayerInfo>>();
        auto info = playerInfo->GetInfo();
        info.gold += emeraldCount;
        playerInfo->SetInfo(info);
    }
}

void Player::CheckFallOut(_float dt)
{
    auto transform = GetComponent<TransformComponent>();
    if (transform->GetWorldPosition().y < falloutValue)
        transform->SetPosition(spawnPoint);
}

void Player::SaveBeforeCollisionVelocity(_float dt)
{
    beforeCollisionVelocity = GetComponent<PhysicsComponent>()->GetVelocity();
}

void Player::SaveAfterCollisionVelocity(_float dt)
{
    afterCollisionVelocity = GetComponent<PhysicsComponent>()->GetVelocity();
}

void Player::CheckFallHit(_float dt)
{
    auto velocityGap = afterCollisionVelocity.y - beforeCollisionVelocity.y;
    if (velocityGap >= minFallHitVelocityY)
    {
        velocityGap = std::clamp(velocityGap, minFallHitVelocityY,maxFallHitVelocityY);

        _float maxGap = maxFallHitVelocityY - minFallHitVelocityY;
        _float fallDamagePercent = (velocityGap - minFallHitVelocityY) / maxGap;
        fallDamagePercent = std::clamp(fallDamagePercent, 0.f, 1.f);
        
        auto playerInfo = GetComponent<InfoComponent<PlayerInfo>>();
        _float maxHp = playerInfo->GetInfo().maxHp;
        _float maxFallDamage = maxHp * 0.2f;
        _float minFallDamage = maxHp * 0.05f;

        _float fallDamage = maxFallDamage * fallDamagePercent;
        if (fallDamage < minFallDamage)
            fallDamage = minFallDamage;

        playerInfo->AddHp(-fallDamage);
    }
}

void Player::EquipItem(ItemType itemType)
{
    switch (itemType) {
    case ItemType::Bow:
        Bones["LHand"] = Bow::Create(owner, ObjectType::Item);
        Bones["LHand"]->GetComponent<TransformComponent>()->SetParent(Bones["LArm"]->GetComponent<TransformComponent>());
        itemBaseRotOffset.insert({ "bow", Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate() });
        Bones["LHand"]->AddRef();
        break;
    case ItemType::Sword:
        Bones["RHand"] = Sword::Create(owner, ObjectType::Item);
        Bones["RHand"]->GetComponent<TransformComponent>()->SetParent(Bones["RArm"]->GetComponent<TransformComponent>());
        itemBaseRotOffset.insert({ "sword", Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate() });
        Bones["RHand"]->AddRef();
        break;
    case ItemType::Spear:
        Bones["RHand"] = Spear::Create(owner, ObjectType::Item);
        Bones["RHand"]->GetComponent<TransformComponent>()->SetParent(Bones["RArm"]->GetComponent<TransformComponent>());
        itemBaseRotOffset.insert({ "spear", Bones["RHand"]->GetComponent<TransformComponent>()->GetRotate() });
        Bones["RHand"]->AddRef();
        break;
    case ItemType::CrossBow:
        Bones["LHand"] = Crossbow::Create(owner, ObjectType::Item);
        Bones["LHand"]->GetComponent<TransformComponent>()->SetParent(Bones["LArm"]->GetComponent<TransformComponent>());
        itemBaseRotOffset.insert({ "crossbow", Bones["LHand"]->GetComponent<TransformComponent>()->GetRotate() });
        Bones["LHand"]->AddRef();
        break;
    case ItemType::Armor:
        Bones["Armor"] = Armor::Create(owner, ObjectType::Item);
        Bones["Armor"]->AddRef();
        break;
    }
}

void Player::UnEquipItem(ItemType itemType)
{
    switch (itemType) {
    case ItemType::Bow:
        itemBaseRotOffset.erase("bow");
        Bones["LHand"]->SetDead();
        Safe_Release(Bones["LHand"]);
        Bones["LHand"] = nullptr;
        break;
    case ItemType::CrossBow:
        itemBaseRotOffset.erase("crossbow");
        Bones["LHand"]->SetDead();
        Safe_Release(Bones["LHand"]);
        Bones["LHand"] = nullptr;
        break;
    case ItemType::Sword:
        itemBaseRotOffset.erase("sword");
        Bones["RHand"]->SetDead();
        Safe_Release(Bones["RHand"]);
        Bones["RHand"] = nullptr;
        break;
    case ItemType::Spear:
        itemBaseRotOffset.erase("spear");
        Bones["RHand"]->SetDead();
        Safe_Release(Bones["RHand"]);
        Bones["RHand"] = nullptr;
        break;
    case ItemType::Armor:
        auto armorBones = static_cast<Armor*>(Bones["Armor"])->GetBones();
        for (auto& bone : armorBones)
        {
            bone.second->SetDead();
            Safe_Release(bone.second);
        }
        Bones["Armor"]->SetDead();
        Safe_Release(Bones["Armor"]);
        Bones["Armor"] = nullptr;
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

void Player::RevivePlayer()
{
    if (DeadTime < DeadDuration) return;
    EngineCore::GetInstance()->GetSoundManager()->PlaySFX("RevivePlayer");
    State = ePlayerState::REVIVE;
    DeadTime = 0.f;
    ReviveTime = 0.f;
    auto info = GetComponent<InfoComponent<PlayerInfo>>();
    info->AddHp(info->GetInfo().maxHp);
}

_vec3 Player::GetAttackDirection()
{
    return AttackDirection;
}

void Player::ChangeShootType()
{
    switch (shootType)
    {
    case ePlayerShootType::ARROW:
        shootType = ePlayerShootType::FIREWORK;
        arrows->GetComponent<UIRenderer>()->SetTexture(L"inventory_rocket");
        break;
    case ePlayerShootType::FIREWORK:
        shootType = ePlayerShootType::ARROW;
        arrows->GetComponent<UIRenderer>()->SetTexture(L"arrows");
        break;
    }
}

bool Player::IsMovingToAttack()
{
    return moveToAttack;
}

void Player::SetStatikkMode(_bool _statickk)
{
    StatikkMode = _statickk;
}

_bool Player::IsStatikkMode()
{
    return StatikkMode;
}

_float Player::GetAttackDelay()
{
    return AttackDuration;
}

void Player::SetOwner(ObjectManager* owner)
{
    this->owner = owner;

    for (auto& bone : Bones)
    {
        if (bone.second)
        {
            if (bone.second->GetObjectType() == ObjectType::Item)
            {
                bone.second->SetDead();
                Safe_Release(bone.second);
            }
            else
            {
                bone.second->SetOwner(owner);
                bone.second->AddRef();
                owner->AddObject(bone.second->GetObjectType(), bone.second);
            }

        }
    }

    auto collision = GetComponent<CollisionComponent>();

    auto physics = GetComponent<PhysicsComponent>();
    physics->SetGround(false);
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);
}

_vec3 Player::GetAttackDirection() const
{
    return AttackDirection;
}

_vec3 Player::GetDir() const

{
    _vec3 dir = PlayerDirection;
    D3DXVec3Normalize(&dir, &dir);
    return dir;
}

_bool Player::IsCharge()
{
    return onCharge;
}

std::unordered_map<string, Object*> Player::GetBones()
{
    return Bones;
}

void Player::SetFloatMode(_bool _floatMode)
{
    FloatMode = _floatMode;
    FloatTimer = 0.f;
}

void Player::SetSpawnPointFromTrigger(LOADID _scene, _int _num)
{
    switch (_scene)
    {
    case LOADID::Village:
        SetSpawnPoint(_vec3(278.f, 35.f, 50.f));
        break;
    case LOADID::Stage1:
        switch (_num)
        {
        case 1:
            SetSpawnPoint(_vec3(40.f, 105.f, 120.f));
            break;
        case 2:
            SetSpawnPoint(_vec3(110.f, 130.f, 40.f));
            break;
        case 3:
            SetSpawnPoint(_vec3(210.f, 130.f, 120.f));
            break;
        case 4:
            SetSpawnPoint(_vec3(305.f, 123.f, 110.f));
            break;
        case 5:
            SetSpawnPoint(_vec3(262.f, 107.f, 355.f));
            break;
        case 6:
            SetSpawnPoint(_vec3(112.f, 106.f, 320.f));
            break;
        default:
            SetSpawnPoint(_vec3(110.f, 140.f, 170.f));
            break;
        }
        break;
    case LOADID::Stage2:
        switch (_num)
        {
        case 1:
            SetSpawnPoint(_vec3(148.f, 72.f, 116.f));
            break;
        case 2:
            SetSpawnPoint(_vec3(480.f, 72.f, 200.f));
            break;
        case 3:
            SetSpawnPoint(_vec3(475.f, 72.f, 365.f));
            break;
        case 4:
            SetSpawnPoint(_vec3(135.f, 72.f, 410.f));
            break;
        default:
            SetSpawnPoint(_vec3(210.f, 80.f, 90.f));
            break;
        }
        break;
    }
}

void Player::SetSpawnPoint(_vec3 _spawnPoint)
{
    spawnPoint = _spawnPoint;
}

void Player::SetInventoryMode(bool enable)
{
    auto renderSystem = EngineCore::GetInstance()->GetRenderSystem();
    auto camMgr = GetScene()->GetCameraManager();
    auto tf = GetComponent<TransformComponent>();
    auto physics = GetComponent<PhysicsComponent>();

    if (enable) // 인벤토리 여는경우
    {
        if (renderSystem->GetCurRenderState() == UIRenderType::Inventory) return;

        lastWorldPos = tf->GetPosition();
        lastWorldDir = GetDir();
        tf->SetPosition(0.f, lastWorldPos.y, 0.f);
        renderSystem->SetUIRenderState(UIRenderType::Inventory);
        camMgr->SetMainCamera(L"Inventory_Camera");
        physics->SetMass(0.f); 
        PlayerDirection = {0.f,0.f,1.f};
        tf->SetForward(PlayerDirection);
    }
    else
    {
        if (renderSystem->GetCurRenderState() == UIRenderType::MainGame) return;

        tf->SetPosition(lastWorldPos);
        renderSystem->SetUIRenderState(UIRenderType::MainGame);
        camMgr->SetMainCamera(L"Third_Camera");
        physics->SetMass(1.f);
        PlayerDirection = {lastWorldDir.x,lastWorldDir.y,lastWorldDir.z};
        tf->SetForward(PlayerDirection);
    }
}

void Player::SetShopMode(bool enable)
{
    auto renderSys = EngineCore::GetInstance()->GetRenderSystem();
    if (enable)
        renderSys->SetUIRenderState(UIRenderType::Shop);
    else
        renderSys->SetUIRenderState(UIRenderType::MainGame);
}

void Player::UpdateIdle(_float dt)
{
    if (comboTime < comboLimit)
    {
        comboTime += dt;
        if (comboTime >= comboLimit) attackType = ePlayerAttackType::FIRST;
        return;
    }
    
    IdleSmoothing(dt, ePlayerBone::HEAD);
    IdleSmoothing(dt, ePlayerBone::BODY);

    IdleSmoothing(dt, ePlayerBone::LARM);
    IdleSmoothing(dt, ePlayerBone::RARM);

    IdleSmoothing(dt, ePlayerBone::LLEG);
    IdleSmoothing(dt, ePlayerBone::RLEG);

    IdleSmoothing(dt, ePlayerBone::LHAND);
    IdleSmoothing(dt, ePlayerBone::RHAND);
}

void Player::UpdateWalk(_float dt) {
    WalkTime += dt;

    if(GetComponent<PhysicsComponent>()->IsGrounded())
        walkEffectTimer += dt;

    if (comboTime < comboLimit)
    {
        comboTime += dt;
        if (comboTime >= comboLimit) attackType = ePlayerAttackType::FIRST;
    }

    IdleSmoothing(dt, ePlayerBone::HEAD);
    IdleSmoothing(dt, ePlayerBone::BODY);
    IdleSmoothing(dt, ePlayerBone::LHAND);
    IdleSmoothing(dt, ePlayerBone::RHAND);

    //Rotate Bones
    float fAngle = sinf(WalkTime * WalkSwingSpeed);
    if (Bones["RHand"] && static_cast<Item*>(Bones["RHand"])->GetItemType() == ItemType::Spear)
    {
        IdleSmoothing(dt, ePlayerBone::LARM);
        IdleSmoothing(dt, ePlayerBone::RARM);
        fAngle = std::fabsf(fAngle) * 0.6;
        SetRotation({ D3DXToRadian(-10.f), 0.f, -fAngle }, "LLeg");
        SetRotation({ D3DXToRadian(10.f), 0.f, fAngle }, "RLeg");
    }
    else
    {
        SetRotation({ -fAngle, 0.f, 0.f }, "LArm");
        SetRotation({ fAngle, 0.f, 0.f }, "RArm");
        SetRotation({ fAngle, 0.f, 0.f }, "LLeg");
        SetRotation({ -fAngle, 0.f, 0.f }, "RLeg");
    }

    //Move Player
    auto transform = GetComponent<TransformComponent>();
    
    //잠시 사용안함 - 0716
    //if (moveToAttack)
    //{
    //    //이동하는 순간 오브젝트가 죽으면 버그 생김
    //    PlayerDirection = moveToObject->GetComponent<TransformComponent>()->GetWorldPosition() - transform->GetWorldPosition();
    //    PlayerDirection.y = 0.f;
    //}

    _vec3 vDir;
    D3DXVec3Normalize(&vDir, &PlayerDirection);

    _float Speed = GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().speed;
    _vec3 moveVec =
    {
        vDir.x * Speed * Scale * dt,
        0,
        vDir.z * Speed * Scale * dt
    };

    _vec3 playerHalfSize = GetComponent<CollisionComponent>()->GetSize() * 0.5f;
    auto blockPos = transform->GetWorldPosition() + moveVec + vDir * 1.8f;

    auto grid = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene()->GetStaticGrid();

    auto blockUp = grid->QueryCell(
        grid->WorldToCell(blockPos.x), 
        grid->WorldToCell(blockPos.y + playerHalfSize.y + -0.1f * Scale), 
        grid->WorldToCell(blockPos.z));
    auto blockFront = grid->QueryCell(
        grid->WorldToCell(blockPos.x), 
        grid->WorldToCell(blockPos.y), 
        grid->WorldToCell(blockPos.z));
    auto blockDown = grid->QueryCell(
        grid->WorldToCell(blockPos.x), 
        grid->WorldToCell(blockPos.y - playerHalfSize.y + 0.1f * Scale), 
        grid->WorldToCell(blockPos.z));

    auto pos = transform->GetWorldPosition();
    auto blockUnder = grid->QueryCell(
        grid->WorldToCell(pos.x),
        grid->WorldToCell(pos.y - playerHalfSize.y - 0.1f * Scale),
        grid->WorldToCell(pos.z));

    auto litY = (pos.y - floorf(pos.y));
    if (litY == 0.5f)
        onHalf = false;
    else
        onHalf = true;

    if (onHalf && blockDown && blockUnder)
    {
        auto ubPos = blockUnder->GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
        auto bdPos = blockDown->GetOwner()->GetComponent<TransformComponent>()->GetWorldPosition();
        auto bdSize = blockDown->GetOwner()->GetComponent<TransformComponent>()->GetScale();
        if (bdSize.y < 2.f && bdPos.y == ubPos.y)
            blockDown = nullptr;
    }

    if (blockUp == nullptr && blockFront == nullptr && blockDown == nullptr)
    {
        transform->Translate(moveVec);
    }
    else if (blockUp == nullptr && blockFront == nullptr && blockDown != nullptr)
    {
        auto blockDownType = blockDown->GetOwner()->GetObjectType();
        if (blockDownType == ObjectType::StaticBlock || blockDownType == ObjectType::CollisionBlock)
        {
            CollisionComponent* block;
            if(onHalf) 
               block = grid->QueryCell(
                   grid->WorldToCell(blockPos.x), 
                   grid->WorldToCell(blockPos.y - playerHalfSize.y + 0.6f * Scale), 
                   grid->WorldToCell(blockPos.z));
            else
                block = grid->QueryCell(
                    grid->WorldToCell(blockPos.x), 
                    grid->WorldToCell(blockPos.y - playerHalfSize.y + 1.6f * Scale), 
                    grid->WorldToCell(blockPos.z));

            if (block == nullptr)
            {
                if (onHalf)
                    transform->Translate(moveVec + _vec3(0.f, 0.5f, 0.f));
                else
                    transform->Translate(moveVec + _vec3(0.f, 1.5f, 0.f));
            }
            else
            {
                auto blockType = block->GetOwner()->GetObjectType();
                if (blockType == ObjectType::StaticBlock || blockType == ObjectType::CollisionBlock)
                {
                    if (onHalf)
                        transform->Translate(moveVec + _vec3(0.f, 0.5f, 0.f));
                    else
                        transform->Translate(moveVec + _vec3(0.f, 2.f, 0.f));
                }
            }
        }
    }

    //////////////////////////////////////////Walk Effect
    if (walkEffectTimer >= walkEffectTerm)
    {
        CollisionBlock* block;
        StaticBlockType blockType = StaticBlockType::sBlockEnd;;
        if (blockUnder)
        {
            block = dynamic_cast<CollisionBlock*>(blockUnder->GetOwner());

            if (block)
                blockType = block->GetType();
        }

        if (soundBefore == 2)
        {
            switch (blockType)
            {
             case StaticBlockType::DarkDirt: case StaticBlockType::DirtPath:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnDefault1");
                break;
            case StaticBlockType::DarkGrass: case StaticBlockType::GrassDirt: case StaticBlockType::Dirt:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnGrass1");
                break;
            case StaticBlockType::Wood: case StaticBlockType::WoodPlank: case StaticBlockType::DarkWoodPlank: case StaticBlockType::Oak: case StaticBlockType::DarkOak:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnWood1");
                break;
            case StaticBlockType::Stone: case StaticBlockType::CobbleStone: case StaticBlockType::SmoothStone: case StaticBlockType::StoneBrick: 
            case StaticBlockType::MossyStoneBrick: case StaticBlockType::EndStone: case StaticBlockType::EndStoneBrick: case StaticBlockType::DarkStone:
            case StaticBlockType::DarkCobbleStone: case StaticBlockType::DarkSmoothStone: case StaticBlockType::DarkStoneBrick: case StaticBlockType::DarkMossyStoneBrick:
            case StaticBlockType::PurBrick:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnStone1");
                break;
            case StaticBlockType::WhiteWool: case StaticBlockType::YellowWool:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnCloth1");
                break;
            case StaticBlockType::Air: case StaticBlockType::Glass: case StaticBlockType::Leaf: case StaticBlockType::Furnace: case StaticBlockType::Haybale:
            case StaticBlockType::Terracota: case StaticBlockType::BookShelf: case StaticBlockType::DarkLeaf: case StaticBlockType::PurPillar:
            case StaticBlockType::PurGlass: case StaticBlockType::EndRod: case StaticBlockType::ChorusBranch: case StaticBlockType::ChorusFlower:
            case StaticBlockType::ChorusFruit:
            default:
                //EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnSand1");
                break;
            }
            soundBefore = 1;
        }
        else if (soundBefore == 1)
        {
            switch (blockType)
            {
             case StaticBlockType::DarkDirt: case StaticBlockType::DirtPath:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnDefault2");
                break;
            case StaticBlockType::DarkGrass: case StaticBlockType::GrassDirt: case StaticBlockType::Dirt:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnGrass2");
                break;
            case StaticBlockType::Wood: case StaticBlockType::WoodPlank: case StaticBlockType::DarkWoodPlank: case StaticBlockType::Oak: case StaticBlockType::DarkOak:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnWood2");
                break;
            case StaticBlockType::Stone: case StaticBlockType::CobbleStone: case StaticBlockType::SmoothStone: case StaticBlockType::StoneBrick:
            case StaticBlockType::MossyStoneBrick: case StaticBlockType::EndStone: case StaticBlockType::EndStoneBrick: case StaticBlockType::DarkStone:
            case StaticBlockType::DarkCobbleStone: case StaticBlockType::DarkSmoothStone: case StaticBlockType::DarkStoneBrick: case StaticBlockType::DarkMossyStoneBrick:
            case StaticBlockType::PurBrick:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnStone2");
                break;
            case StaticBlockType::WhiteWool: case StaticBlockType::YellowWool:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnCloth2");
                break;
            case StaticBlockType::Air: case StaticBlockType::Glass: case StaticBlockType::Leaf: case StaticBlockType::Furnace: case StaticBlockType::Haybale:
            case StaticBlockType::Terracota: case StaticBlockType::BookShelf: case StaticBlockType::DarkLeaf: case StaticBlockType::PurPillar:
            case StaticBlockType::PurGlass: case StaticBlockType::EndRod: case StaticBlockType::ChorusBranch: case StaticBlockType::ChorusFlower:
            case StaticBlockType::ChorusFruit:
            default:
                //EngineCore::GetInstance()->GetSoundManager()->PlaySFX("WalkOnSand2");
                break;
            }
            soundBefore = 2;
        }
        _vec3 vDir = { 0.f,0.f,1.f };
        _vec3 vMove;
        D3DXVec3Normalize(&vMove, &PlayerDirection);

        _float dot = D3DXVec3Dot(&vDir, &vMove);
        _float fAngle = D3DXToDegree(acosf(dot));

        auto effect = RollEffect::Create(owner, ObjectType::ParticleEffect,this, fAngle - 180.f);
        owner->AddObject(ObjectType::ParticleEffect, effect);
        walkEffectTimer = 0.f;
    }
    //////////////////////////////////////////

    auto blockOn1 = grid->QueryCell(grid->WorldToCell(blockPos.x - 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z - 1));
    auto blockOn2 = grid->QueryCell(grid->WorldToCell(blockPos.x - 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z + 1));
    auto blockOn3 = grid->QueryCell(grid->WorldToCell(blockPos.x + 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z - 1));
    auto blockOn4 = grid->QueryCell(grid->WorldToCell(blockPos.x + 1), grid->WorldToCell(blockPos.y - 2), grid->WorldToCell(blockPos.z + 1));
    
    if (blockOn1 == nullptr && blockOn2 == nullptr && blockOn3 == nullptr && blockOn4 == nullptr) GetComponent<PhysicsComponent>()->SetGround(false);

    //Rotate Player
    const float fRotateDuration = 0.05f;
    
    float TargetAngle;
    if (Bones["RHand"] && static_cast<Item*>(Bones["RHand"])->GetItemType() == ItemType::Spear)
    {
        TargetAngle = atan2f(vDir.x, vDir.z) + D3DXToRadian(60.f);
    }
    else
    {
        TargetAngle = atan2f(vDir.x, vDir.z);
    }
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
        if (distance <= MeleeRange)
        {
            EngineCore::GetInstance()->GetSoundManager()->PlaySFX("SwingSword");
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

    if (RollTime == dt)
    {
        _int iMax = 10;
        for (int i = 0;i < iMax;++i)
        {
            auto rollEffect = RollEffect::Create(owner, ObjectType::ParticleEffect, this, (360.f / iMax) * i);
            owner->AddObject(ObjectType::ParticleEffect, rollEffect);
        }
    }
    
    rollEffectTimer += dt;
    if (RollTime > 0.25f && rollEffectTimer > rollEffectTerm)
    {
        rollEffectTimer = 0.f;
        _vec3 vDir = { 0.f,0.f,1.f };
        _vec3 vMove;
        D3DXVec3Normalize(&vMove, &PlayerDirection);

        _float dot = D3DXVec3Dot(&vDir, &vMove);
        _float fAngle = D3DXToDegree(acosf(dot));

        auto rollEffect = RollEffect::Create(owner, ObjectType::ParticleEffect, this, fAngle - 90);
        owner->AddObject(ObjectType::ParticleEffect, rollEffect);
        auto rollEffect2 = RollEffect::Create(owner, ObjectType::ParticleEffect, this, fAngle + 90);
        owner->AddObject(ObjectType::ParticleEffect, rollEffect2);
    }

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

    if (walkEffectTimer >= walkEffectTerm)
    {
        auto effect = SpriteEffect::Create(owner, ObjectType::SpriteEffect);
        effect->GetComponent<TransformComponent>()->SetPosition(GetComponent<TransformComponent>()->GetPosition() - _vec3(0.f, 2.f, 0.f));
        effect->AddComponent<SpriteRenderer>("Walk", 7, 10.f, 2.f);
        owner->AddObject(ObjectType::SpriteEffect, effect);
        walkEffectTimer = 0.f;
    }

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

    if (!Bones["RHand"])
    {
        State = ePlayerState::IDLE;
        return;
    }
    //Components
    auto transform = GetComponent<TransformComponent>();

    //Rotate Bones
    float fProgress = AttackTime / AttackDuration;
    fProgress = std::clamp(fProgress, 0.f, 1.f);
    
    if (AttackTime == dt)
    {
        SaveStartRotation();
        ItemType type = static_cast<Item*>(Bones["RHand"])->GetItemType();
        switch (type)
        {
        case ItemType::Sword:
            switch (attackType)
            {
            case ePlayerAttackType::FIRST:
                SetUpSwordFirstAttackPhaseRotations();
                break;
            case ePlayerAttackType::SECOND:
                SetUpSwordSecondAttackPhaseRotations();
                break;
            case ePlayerAttackType::LAST:
                SetUpSwordLastAttackPhaseRotations();
                break;
            }
            break;
        case ItemType::Spear:
            switch (attackType)
            {
            case ePlayerAttackType::FIRST:
                SetUpSpearFirstAttackPhaseRotations();
                break;
            case ePlayerAttackType::SECOND:
                SetUpSpearSecondAttackPhaseRotations();
                break;
            case ePlayerAttackType::LAST:
                SetUpSpearLastAttackPhaseRotations();
                break;
            }
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
    _vec3 vStartRot = _vec3{ 0.f, atan2f(AttackDirection.x, AttackDirection.z), 0.f };
    _vec3 vCurrentRot = GetPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::ATTACK, ePlayerBone::BODY));
    _vec3 vDeltaRot = vStartRot + vCurrentRot;
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

    if (!Bones["LHand"])
    {
        State = ePlayerState::IDLE;
        return;
    }
    //Components
    auto transform = GetComponent<TransformComponent>();

    //shoot arrow
    float fProgress = std::clamp(AttackTime / ShootDuration, 0.f, 1.f);


    vector<float> phaseVec = { 0.15f, 0.4f, 0.9f, 1.f };
    static float prePhase = 0.f;
    if (prePhase <= phaseVec.at(0) && fProgress > phaseVec.at(0)) {
        auto input = EngineCore::GetInstance()->GetInputSystem();
        ItemType leftHandType = static_cast<Item*>(Bones["LHand"])->GetItemType();
        if (input->IsKeyDown(RBUTTON) && leftHandType == ItemType::Bow && shootType == ePlayerShootType::ARROW)
        {
            if (onCharge == false)
            {
                onCharge = true;
                auto chargeDownEffect = ChargeDownEffect::Create(owner, ObjectType::ParticleEffect, Bones["LHand"]);
                owner->AddObject(ObjectType::ParticleEffect, chargeDownEffect);
                auto chargeFrontEffect = ChargeFrontEffect::Create(owner, ObjectType::ParticleEffect, Bones["LHand"]);
                owner->AddObject(ObjectType::ParticleEffect, chargeFrontEffect);
            }
            if (onChargeEnd == false && chargedTime >= 1.f)
            {
                onChargeEnd = true;
                auto chargeOnEffect = ChargeOnEffect::Create(owner, ObjectType::ParticleEffect, Bones["LHand"]);
                owner->AddObject(ObjectType::ParticleEffect, chargeOnEffect);
            }
            auto input = EngineCore::GetInstance()->GetInputSystem();
            auto curScene = EngineCore::GetInstance()->GetSceneManager()->GetActiveScene();
            auto mainCam = curScene->GetCameraManager()->GetMainCamera();
            auto collision = curScene->GetCollisionSystem();
            auto grid = curScene->GetStaticGrid();

            fProgress = phaseVec.at(0);
            chargedTime += dt;
            AttackTime -= dt;
            Ray ray = mainCam->ScreenPointRay();
            //////////////////////////////////////////////Picking Change
            HitInfo hit;
            HitInfo objectHit = collision->Raycast(ray);
            HitInfo terrainHit = grid->RayCast(ray);
            hit = (objectHit.Distance <= terrainHit.Distance) ? objectHit : terrainHit;
            //////////////////////////////////////////////

            if (hit.IsHit)
            {
                auto transform = GetComponent<TransformComponent>();
                auto collision = GetComponent<CollisionComponent>();
                auto curPos = transform->GetWorldPosition();
                auto attackPos = hit.Position;

                AttackDirection = attackPos - curPos;
                _vec3 distanceVec = DestinationPos - curPos;
                _float distance = sqrtf(distanceVec.x * distanceVec.x + distanceVec.z * distanceVec.z);

                if(distance < 1.f)
                    PlayerDirection = attackPos - curPos;
            }
        }
        else
        {
            onCharge = false;
            onChargeEnd = false;

            auto shootDir = AttackDirection;
            D3DXVec3Normalize(&shootDir, &shootDir);
            switch (shootType)
            {
            case ePlayerShootType::ARROW:
                switch (leftHandType)
                {
                case ItemType::Bow:
                    EngineCore::GetInstance()->GetSoundManager()->PlaySFX("ShootArrow");
                    chargedTime = std::clamp(chargedTime, 0.f, MaxChargeTime);
                    Arrow::Create(owner, ObjectType::Projectile, this, shootDir, (1.f + chargedTime));
                    chargedTime = 0.f;
                    break;
                case ItemType::CrossBow:
                    EngineCore::GetInstance()->GetSoundManager()->PlaySFX("ShootArrow");
                    Arrow::Create(owner, ObjectType::Projectile, this, shootDir);

                    _matrix matRotY;
                    D3DXMatrixRotationY(&matRotY, D3DXToRadian(10.f));
                    D3DXVECTOR3 vResult;
                    D3DXVec3TransformNormal(&vResult, &shootDir, &matRotY);
                    D3DXVec3Normalize(&vResult, &vResult);
                    Arrow::Create(owner, ObjectType::Projectile, this, vResult);

                    D3DXMatrixRotationY(&matRotY, D3DXToRadian(-10.f));
                    D3DXVec3TransformNormal(&vResult, &shootDir, &matRotY);
                    D3DXVec3Normalize(&vResult, &vResult);
                    Arrow::Create(owner, ObjectType::Projectile, this, vResult);
                    break;
                }
                break;
            case ePlayerShootType::FIREWORK:
                EngineCore::GetInstance()->GetSoundManager()->PlaySFX("ShootFirework");
                Firework::Create(owner, ObjectType::Projectile, this, shootDir);
                break;
            }
        }
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
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::SHOOT, ePlayerBone::HEAD));

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
        if(Bones["LHand"])
            Bones["LHand"]->GetComponent<MeshRenderer>()->SetRenderID(Engine::RENDER_ID::Render_None);
        if(Bones["RHand"])
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
    if (DeadTime >= DeadDuration) 
    {
        State = ePlayerState::DEAD;
        physics->SetGround(true);

        //auto input = EngineCore::GetInstance()->GetInputSystem();
        //if (input->IsKeyPressed(R)) RevivePlayer();
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

void Player::UpdateRevive(_float dt)
{
    ReviveTime += dt;

    //Components
    auto physics = GetComponent<PhysicsComponent>();
    auto transform = GetComponent<TransformComponent>();

    //Fix Dead State
    if (ReviveTime >= reviveDuration) 
    {
        State = ePlayerState::IDLE;
        physics->SetGround(true);
        return;
    }

    //Rotate Bones
    float fProgress = std::clamp(ReviveTime / reviveDuration, 0.f, 1.f);

    if (ReviveTime == dt)
    {
        SetUpRevivePhaseRotations();
    }

    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::LARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::RARM));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::LLEG));
    ApplyPhasedRotation(fProgress, GetPhaseRotations(ePlayerState::REVIVE, ePlayerBone::RLEG));

    //Rotate Player
    float MaxReviveAngle = D3DXToRadian(90.f);
    float fCurrentAngle = MaxReviveAngle * (1 - fProgress);

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
    float fSinT = sinf(D3DX_PI / 2.f * fProgress);
    float fLerpY = 1.f + (7.f - 1.f) * fSinT;
    collision->SetSize(_vec3(2.f, fLerpY, 2.f));
    physics->SetGround(false);
}

void Player::KeyInput(_float dt)
{
    PickingTerrain();
    CheckStateRoll(dt);
    CheckDead();
    CheckJump();
    CheckSkill();
    CamRotTest(dt);
    ResetWalkTimer();
    PullingEmerald(dt);
    CheckFallOut(dt);
}

void Player::CheckStateRoll(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    static const KEY keyRoll = SPACE;

    if (!input->IsKeyPressed(keyRoll)) return;

    DashUI* dashUI = nullptr;
    for (auto obj : owner->GetObjectList(ObjectType::UI))
    {
        dashUI = dynamic_cast<DashUI*>(obj);
        if (dashUI)
            break;
    }
    if (!dashUI) return;

    auto dashItem = dashUI->GetComponent<ItemComponent>();
    if (!dashItem || dashItem->IsCoolDown())
        return;

    dashItem->Use(this);
    // ------------------------------------------------ 제현 --------
    
    if (State == ePlayerState::IDLE || State == ePlayerState::WALK)
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("Roll");
    State = ePlayerState::ROLL;
    SaveStartRotation();
}

void Player::CheckDead()
{
    if (GetComponent<InfoComponent<PlayerInfo>>()->GetInfo().curHp <= 0.f && DeadTime == 0)
    {
        EngineCore::GetInstance()->GetSoundManager()->PlaySFX("DeathPlayer");
        State = ePlayerState::DEAD;
        SaveStartRotation();

        EngineCore::GetInstance()->GetRenderSystem()->SetUIRenderState(UIRenderType::DeathUI);
    }
}

void Player::InteractWithNPC(Object* obj)
{
    if (!obj) return;

    auto* npc = dynamic_cast<Npc*>(obj);
    if (!npc) return;

    auto* dialogMgr = GetScene()->GetUIManager()->GetDialog();
    if (!dialogMgr)return;

    npc->Talk(dialogMgr);
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

void Player::IdleSmoothing(_float dt, ePlayerBone bone) {
    std::string strBone;
    switch (bone)
    {
    case ePlayerBone::BODY:
        strBone = "Body";
        break;
    case ePlayerBone::HEAD:
        strBone = "Head";
        break;
    case ePlayerBone::LARM:
        strBone = "LArm";
        break;
    case ePlayerBone::RARM:
        strBone = "RArm";
        break;
    case ePlayerBone::LLEG:
        strBone = "LLeg";
        break;
    case ePlayerBone::RLEG:
        strBone = "RLeg";
        break;
    case ePlayerBone::LHAND:
        strBone = "LHand";
        break;
    case ePlayerBone::RHAND:
        strBone = "RHand";
        break;
    }
    if (Bones[strBone] == nullptr) return;

    ItemType rHandType;
    if (Bones["RHand"] && itemBaseRotOffset.find("sword") != itemBaseRotOffset.end())
    {
        rHandType = ItemType::Sword;
    }
    else if (Bones["RHand"] && itemBaseRotOffset.find("spear") != itemBaseRotOffset.end())
    {
        rHandType = ItemType::Spear;
    }
    else
    {
        rHandType = ItemType::None;
    }

    _vec3 vTargetRot;
    if (bone == ePlayerBone::LHAND && itemBaseRotOffset.find("bow") != itemBaseRotOffset.end())
    {
        vTargetRot = itemBaseRotOffset.at("bow");
    }
    else if (bone == ePlayerBone::LHAND && itemBaseRotOffset.find("crossbow") != itemBaseRotOffset.end())
    {
        vTargetRot = itemBaseRotOffset.at("crossbow");
    }
    else {
        vTargetRot = idleRot[{static_cast<int>(rHandType),static_cast<int>(bone)}];
    }

    if (bone == ePlayerBone::BODY) {
        _vec3 vCurrentRot = GetComponent<TransformComponent>()->GetRotate();

        _vec3 vDeltaRot = {
            WrapAngle(vTargetRot.x - vCurrentRot.x),
            WrapAngle(vTargetRot.y - vCurrentRot.y),
            WrapAngle(vTargetRot.z - vCurrentRot.z)
        };

        _vec3 vLerpedRot = vCurrentRot + vDeltaRot * dt * IdleSmoothingSpeed;
        GetComponent<TransformComponent>()->SetRotate(vLerpedRot);
        return;
    }

    _vec3 vCurrentRot = Bones[strBone]->GetComponent<TransformComponent>()->GetRotate();

    _vec3 vDeltaRot = {
        WrapAngle(vTargetRot.x - vCurrentRot.x),
        WrapAngle(vTargetRot.y - vCurrentRot.y),
        WrapAngle(vTargetRot.z - vCurrentRot.z)
    };

    _vec3 vLerpedRot = vCurrentRot + vDeltaRot * dt * IdleSmoothingSpeed;
    Bones[strBone]->GetComponent<TransformComponent>()->SetRotate(vLerpedRot);
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
            fAngle *= 0.5f;
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

    if (leftRight == "") 
    {
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

_float Player::WrapAngle(_float fAngle)
{
    while (fAngle > D3DX_PI) fAngle -= D3DX_PI * 2;
    while (fAngle < -D3DX_PI) fAngle += D3DX_PI * 2;
    return fAngle;
}
