#include "pch.h"
#include "Player.h"
#include "Player.h"
#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"

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

    Bones["Body"] = Bone::Create(owner, objType, _vec3(8.f, 12.f, 4.f), nullptr, L"playerBody_Mtrl");

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

    auto transform = AddComponent<TransformComponent>();

    return S_OK;
}

void Player::Update(_float dt)
{
    Object::Update(dt);
    _vec3 moveVec = { 0.f * dt, 0.f * dt, 100.f * dt }; // ���� status �ӵ� �����ͼ� x,y,z �̵��� ����
    MovePosition(moveVec);
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
    auto transform = Bones["Body"]->GetComponent<TransformComponent>();
    transform->SetPosition(transform->GetPosition()+moveVec);
}

void Player::SetRotation(string str, _vec3 rotation)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
    }
}
