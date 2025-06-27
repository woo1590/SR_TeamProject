#include "pch.h"
#include "BaseCharacter.h"

//system
#include "ObjectManager.h"

//object
#include "Bone.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"


BaseCharacter::BaseCharacter(ObjectManager* owner, ObjectType objType)
	:Object(owner,objType)
{
}

BaseCharacter::~BaseCharacter()
{
}

BaseCharacter* BaseCharacter::Create(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter* Instance = new BaseCharacter(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT BaseCharacter::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Object::Ready_Object();

    Bones["Body"] = Bone::Create(owner, objType, _vec3(1.f, 1.f, 0.5f), nullptr, L"ZombieBody_Mtrl");

    Bones["Head"] = Bone::Create(owner, objType, _vec3(1.f, 1.f, 1.f), Bones["Body"], L"ZombieHead_Mtrl");
    SetPosition("Head", _vec3(0.f, 2.f, 0.f));

    Bones["LHand"] = Bone::Create(owner, objType, _vec3(0.5f, 1.f, 0.5f), Bones["Body"], L"ZombieArm_Mtrl");
    SetPosition("LHand", _vec3(-1.5f, 0.f, 0.f));

    Bones["RHand"] = Bone::Create(owner, objType, _vec3(0.5f, 1.f, 0.5f), Bones["Body"], L"ZombieArm_Mtrl");
    SetPosition("RHand", _vec3(1.5f, 0.f, 0.f));

    Bones["LLeg"] = Bone::Create(owner, objType, _vec3(0.5f, 1.f, 0.5f), Bones["Body"], L"ZombieLeg_Mtrl");
    SetPosition("LLeg", _vec3(-0.5f, -2.f, 0.f));

    Bones["RLeg"] = Bone::Create(owner, objType, _vec3(0.5f, 1.f, 0.5f), Bones["Body"], L"ZombieLeg_Mtrl");
    SetPosition("RLeg", _vec3(0.5f, -2.f, 0.f));

    owner->AddObject(ObjectType::Monster, Bones["Body"]);
    owner->AddObject(ObjectType::Monster, Bones["Head"]);
    owner->AddObject(ObjectType::Monster, Bones["LHand"]);
    owner->AddObject(ObjectType::Monster, Bones["RHand"]);
    owner->AddObject(ObjectType::Monster, Bones["LLeg"]);
    owner->AddObject(ObjectType::Monster, Bones["RLeg"]);

    auto transform = AddComponent<TransformComponent>();

    return S_OK;
}

void BaseCharacter::Update(_float dt)
{
    Object::Update(dt);
}

void BaseCharacter::Late_Update(_float dt)
{
    Object::Late_Update(dt);
}

void BaseCharacter::Free()
{
    Object::Free();
}

void BaseCharacter::SetMaterial(string str, const std::wstring& mtrl)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
    }
}

void BaseCharacter::SetScale(string str, _vec3 scale)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetScale(scale);
    }
}

void BaseCharacter::SetPosition(string str, _vec3 position)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetPosition(position);
    }
}

void BaseCharacter::SetRotation(string str, _vec3 rotation)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
    }
}
