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
    SetScale(0.1f);

    Bones["Body"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale /2.f), this, L"ZombieBody_Mtrl");
    Bones["Body"]->GetComponent<TransformComponent>()->SetScale(8.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["Body"]->GetComponent<TransformComponent>()->SetPosition(0.f, 0.f, 0.f);
    
    Bones["Head"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale), Bones["Body"], L"ZombieHead_Mtrl");
    Bones["Head"]->GetComponent<TransformComponent>()->SetScale(8.f * Scale, 8.f * Scale, 8.f * Scale);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPosition(0.f, 20.f * Scale, 0.f);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -8.f * Scale, 0.0f));
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    
    Bones["LHand"] = Bone::Create(owner, objType, _vec3(Scale /2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieArm_Mtrl");
    Bones["LHand"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPosition(-11.f * Scale, 0.f, 0.f);
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -6.f * Scale, 0.0f));
    Bones["LHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["RHand"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieArm_Mtrl");
    Bones["RHand"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPosition(11.f * Scale, 0.f, 0.f);
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -6.f * Scale, 0.0f));
    Bones["RHand"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["LLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieLeg_Mtrl");
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPosition(-4.f * Scale, -24.f * Scale, 0.f);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.f * Scale, 0.0f));
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["RLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], L"ZombieLeg_Mtrl");
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPosition(4.f * Scale, -24.f * Scale, 0.f);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.f * Scale, 0.0f));
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    
    for (auto& bone : Bones)
        owner->AddObject(objType, bone.second);

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

void BaseCharacter::MoveTo(_vec3* dir,_float dt)
{
    GetComponent<TransformComponent>()->Translate((*dir)* 0.1f);
}

void BaseCharacter::Attack(Object* target)
{
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
