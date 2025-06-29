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

    Bones["Body"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale /2.f), this, L"ZombieBody_Mtrl");

    Bones["Head"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale), Bones["Body"], L"ZombieHead_Mtrl");
    SetPosition(_vec3(0.f, Scale *2.f, 0.f), "Head");

    Bones["LHand"] = Bone::Create(owner, objType, _vec3(Scale /2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieArm_Mtrl");
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -Scale / 2.f, 0.0f));
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetPosition(_vec3(-Scale / 2.f * 3.f, -Scale / 2.f, 0.f),"LHand");

    Bones["RHand"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieArm_Mtrl");
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -Scale / 2.f, 0.0f));
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetPosition(_vec3(Scale / 2.f * 3.f, -Scale / 2.f, 0.f), "RHand");

    Bones["LLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieLeg_Mtrl");
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, Scale / 2.f, 0.0f));
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetPosition(_vec3(-Scale / 2.f, -Scale / 2.f * 5.f, 0.f), "LLeg");

    Bones["RLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieLeg_Mtrl");
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, Scale / 2.f, 0.0f));
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    SetPosition(_vec3(Scale / 2.f, -Scale / 2.f * 5.f, 0.f), "RLeg");

    for (auto& bone : Bones)
        owner->AddObject(ObjectType::Monster, bone.second);

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

void BaseCharacter::SetMaterial(const std::wstring& mtrl, string str)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
    }
}

void BaseCharacter::SetScale(float scale)
{
    Scale = scale;
}

void BaseCharacter::SetPosition(_vec3 position, string str)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetPosition(position);
    }
}

void BaseCharacter::SetRotation(_vec3 rotation, string str)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetRotate(rotation);
    }
}
