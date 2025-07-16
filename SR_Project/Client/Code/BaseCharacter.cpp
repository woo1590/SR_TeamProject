#include "pch.h"
#include "BaseCharacter.h"

//system
#include "ObjectManager.h"

//object
#include "Bone.h"

//component
#include "TransformComponent.h"
#include "MeshRendererComponent.h"
#include "InfoComponent.h"

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

    Bones["Body"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale /2.f), this, "ZombieBody_Mtrl");
    Bones["Body"]->GetComponent<TransformComponent>()->SetScale(8.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["Body"]->GetComponent<TransformComponent>()->SetPosition(0.f, 0.f, 0.f);
    
    Bones["Head"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale), Bones["Body"], "ZombieHead_Mtrl");
    Bones["Head"]->GetComponent<TransformComponent>()->SetScale(8.f * Scale, 8.f * Scale, 8.f * Scale);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPosition(0.f, 20.f * Scale, 0.f);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, -8.f * Scale, 0.0f));
    Bones["Head"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    
    Bones["LArm"] = Bone::Create(owner, objType, _vec3(Scale /2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], "ZombieArm_Mtrl");
    Bones["LArm"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPosition(-11.f * Scale, 0.f, 0.f);
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(11.0f * Scale, -8.f * Scale, 0.0f));
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["RArm"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], "ZombieArm_Mtrl");
    Bones["RArm"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPosition(11.f * Scale, 0.f, 0.f);
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPivot(_vec3(-11.0f * Scale, -8.f * Scale, 0.0f));
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["LLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], "ZombieLeg_Mtrl");
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPosition(-4.f * Scale, -24.f * Scale, 0.f);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.f * Scale, 0.0f));
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);

    Bones["RLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), Bones["Body"], "ZombieLeg_Mtrl");
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 12.f * Scale, 4.f * Scale);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPosition(4.f * Scale, -24.f * Scale, 0.f);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivot(_vec3(0.0f, 12.f * Scale, 0.0f));
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPivotEnable(true);
    
    for (auto& bone : Bones)
        owner->AddObject(ObjectType::Bone, bone.second);

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

void BaseCharacter::HitMoveTo(_vec3* dir, _float dt)
{
    auto Transform = GetComponent<TransformComponent>();
    auto stat = GetComponent<InfoComponent<EnemyInfo>>();
    Transform->Translate(*dir * dt);
}

void BaseCharacter::Attack(Object* target)
{
}

void BaseCharacter::Add_Bone(string str, ObjectType objType, Object* parent, const string& mtrl)
{
    Bones[str] = Bone::Create(owner, objType, _vec3(1.f, 1.f, 1.f), parent, mtrl);

    owner->AddObject(ObjectType::Bone, Bones[str]);
    //scale position 따로 설정하셈
}

void BaseCharacter::PlayKnockBack(_vec3 dir, _float attack, _float dt)
{
    _float length = sqrt(dir.x * dir.x + dir.z * dir.z);
    if (length == 0) length = 1;

    _vec3 knockback;
    knockback.x = (dir.x / length) * attack;
    knockback.y = 0.2 * attack;
    knockback.z = (dir.z / length) * attack;

    HitMoveTo(&knockback, dt);
}

void BaseCharacter::Free()
{
    Object::Free();
}

void BaseCharacter::SetMaterial(const std::string& mtrl, string str, RENDER_ID id)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<MeshRenderer>()->SetMaterial(mtrl);
        Bones[str]->GetComponent<MeshRenderer>()->SetRenderID(id);
    }
}

Material* BaseCharacter::GetMaterial(string str)
{
    if (Bones[str] != nullptr)
    {
        return Bones[str]->GetComponent<MeshRenderer>()->GetMaterial();
    }
    return nullptr;
}

void BaseCharacter::SetScale(_vec3 scale, string str)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->SetScale(scale);
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

void BaseCharacter::SetTranslate(_vec3 translate, string str)
{
    if (Bones[str] != nullptr)
    {
        Bones[str]->GetComponent<TransformComponent>()->Translate(translate);
    }
}

void BaseCharacter::SetWeapon(Object* parent, ObjectType objType, const string& mtrl)
{
    if (parent == nullptr) return;
    Bones["Weapon"] = Bone::Create(owner, objType, _vec3(0.1f, 1.5f, 1.5f), parent, mtrl);
    Bones["Weapon"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
    owner->AddObject(ObjectType::Bone, Bones["Weapon"]);
}

void BaseCharacter::DetachParent(string str)
{
    auto it = Bones.find(str);
    if (it != Bones.end() && it->second != nullptr)
    {
        auto transform = it->second->GetComponent<TransformComponent>();
        if (transform && transform->GetParent() != nullptr)
        {
            transform->SetParent((Object*)nullptr);
        }
    }
}

void BaseCharacter::SetDead()
{
    Object::SetDead();
    std::for_each(Bones.begin(), Bones.end(), [](auto& pair) {
        if (pair.second != nullptr)
        {
            pair.second->SetDead();
        }});
}
