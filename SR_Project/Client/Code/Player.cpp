#include "pch.h"
#include "Player.h"
#include "Player.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "EngineCore.h"
#include "InputSystem.h"

Player::Player(ObjectManager* owner, ObjectType objType) :Object(owner, objType)
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
    Object::Ready_Object();

    auto transform = AddComponent<TransformComponent>();

    Bones["Body"] = Bone::Create(owner, objType, _vec3(8.f, 12.f, 4.f), this, L"playerBody_Mtrl");
    SetPosition("Body", _vec3(0.f, 0.f, 0.f));

    Bones["Head"] = Bone::Create(owner, objType, _vec3(8.f, 8.f, 8.f), Bones["Body"], L"playerHead_Mtrl");
    SetPosition("Head", _vec3(0.f, 20.f, 0.f));

    Bones["LHand"] = Bone::Create(owner, objType, _vec3(3.f, 12.f, 4.f), Bones["Body"], L"playerLeftArm_Mtrl");
    SetPosition("LHand", _vec3(-11.f, 0.f, 0.f));
    
    Bones["RHand"] = Bone::Create(owner, objType, _vec3(3.f, 12.f, 4.f), Bones["Body"], L"playerRightArm_Mtrl");
    SetPosition("RHand", _vec3(11.f, 0.f, 0.f));
    
    Bones["LLeg"] = Bone::Create(owner, objType, _vec3(4.f, 12.f, 4.f), Bones["Body"], L"playerLeftLeg_Mtrl");
    SetPosition("LLeg", _vec3(-4.f, -24.f, 0.f));
    
    Bones["RLeg"] = Bone::Create(owner, objType, _vec3(4.f, 12.f, 4.f), Bones["Body"], L"playerRightLeg_Mtrl");
    SetPosition("RLeg", _vec3(4.f, -24.f, 0.f));

    owner->AddObject(ObjectType::Player, Bones["Body"]);
    owner->AddObject(ObjectType::Player, Bones["Head"]);
    owner->AddObject(ObjectType::Player, Bones["LHand"]);
    owner->AddObject(ObjectType::Player, Bones["RHand"]);
    owner->AddObject(ObjectType::Player, Bones["LLeg"]);
    owner->AddObject(ObjectType::Player, Bones["RLeg"]);

    return S_OK;
}

void Player::Update(_float dt)
{
    Object::Update(dt);

    KeyInput(dt);
    switch (m_eState) {
    case ePlayerState::IDLE:
        UpdateIdle(dt);
        break;
    case ePlayerState::WALK:
        UpdateWalk(dt);
        break;
    }
}

void Player::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void Player::Free()
{
    Object::Free();
}

void Player::SetMaterial(string str, const std::wstring& mtrl)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
    }
}

void Player::SetScale(string str, _vec3 scale)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetScale(scale);
    }
}

void Player::SetPosition(string str, _vec3 position)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetPosition(position);
    }
}

void Player::MovePosition(_vec3 moveVec)
{
    auto transform = GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition()+moveVec);
}

void Player::SetRotation(string str, _vec3 rotation)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
    }
}

void Player::UpdateIdle(_float dt)
{
    if (m_fWalkTime != 0.f) m_fWalkTime = 0.f;
}

void Player::UpdateWalk(_float dt)
{
    m_fWalkTime += dt;
    _vec3 moveVec = { 0.f * dt, 0.f * dt, 100.f * dt };
    MovePosition(moveVec);

    float fAngle = sinf(m_fWalkTime) * 30.f;
    SetRotation("LLeg", { fAngle, 0.f, 0.f });
    SetRotation("RLeg", { -fAngle, 0.f, 0.f });

    SetRotation("LHand", { -fAngle, 0.f, 0.f });
    SetRotation("RHand", { fAngle, 0.f, 0.f });
}

void Player::KeyInput(_float dt)
{
    auto input = EngineCore::GetInstance()->GetInputSystem();
    if (input->IsKeyPressed(TAB)) {
        switch (m_eState) {
        case ePlayerState::IDLE:
            m_eState = ePlayerState::WALK;
            break;
        case ePlayerState::WALK:
            m_eState = ePlayerState::IDLE;
            break;
        }
    }
}
