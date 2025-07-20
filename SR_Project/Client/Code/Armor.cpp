#include "pch.h"
#include "Armor.h"

#include "Bone.h"
#include "TransformComponent.h"
#include "ObjectManager.h"
#include "MeshRendererComponent.h"
#include "Player.h"

Armor::Armor(ObjectManager* owner, ObjectType objType):Item(owner,objType){}

Armor::~Armor(){}

void Armor::Free()
{
	Item::Free();
}

Armor* Armor::Create(ObjectManager* owner, ObjectType objType)
{
    Armor* Instance = new Armor(owner, objType);
    if (FAILED(Instance->Ready_Object(owner, objType))) {
        Safe_Release(Instance);
        Instance = nullptr;
    }
    return Instance;
}

HRESULT Armor::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Item::Ready_Object(owner,objType);

    auto player = static_cast<Player*>(owner->GetFrontObject(ObjectType::Player));

    Scale = 0.1f;

    Bones["Body"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale / 2.f), player->GetBone("Body"), "armorBody_Mtrl");
    Bones["Body"]->GetComponent<TransformComponent>()->SetScale(10.f * Scale, 9.f * Scale, 7.f * Scale);
    Bones["Body"]->GetComponent<TransformComponent>()->SetPosition(0.f, 3.f * Scale, 0.f);
    Bones["Body"]->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);

    Bones["Head"] = Bone::Create(owner, objType, _vec3(Scale, Scale, Scale), player->GetBone("Head"), "armorHead_Mtrl");
    Bones["Head"]->GetComponent<TransformComponent>()->SetScale(10.f * Scale, 10.f * Scale, 9.f * Scale);
    Bones["Head"]->GetComponent<TransformComponent>()->SetPosition(0.f, -1.9f * Scale, 0.f);

    Bones["LEar"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("Head"), "armorEar_Mtrl");
    Bones["LEar"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 5.f * Scale, 0.1f * Scale);
    Bones["LEar"]->GetComponent<TransformComponent>()->SetPosition(-6.f * Scale, 10.f * Scale, 0.f);

    Bones["REar"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("Head"), "armorEar_Mtrl");
    Bones["REar"]->GetComponent<TransformComponent>()->SetScale(3.f * Scale, 5.f * Scale, 0.1f * Scale);
    Bones["REar"]->GetComponent<TransformComponent>()->SetPosition(6.f * Scale, 10.f * Scale, 0.f);

    Bones["Nose"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("Head"), "armorNose_Mtrl");
    Bones["Nose"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 3.f * Scale, 3.f * Scale);
    Bones["Nose"]->GetComponent<TransformComponent>()->SetPosition(0.f, -1.f * Scale, 10.f * Scale);

    Bones["LArm"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("LArm"), "armorLeftArm_Mtrl");
    Bones["LArm"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 5.f * Scale, 5.f * Scale);
    Bones["LArm"]->GetComponent<TransformComponent>()->SetPosition(0.f, 7.1f * Scale, 0.f);

    Bones["RArm"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("RArm"), "armorRightArm_Mtrl");
    Bones["RArm"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 5.f * Scale, 5.f * Scale);
    Bones["RArm"]->GetComponent<TransformComponent>()->SetPosition(0.f, 7.1f * Scale, 0.f);

    Scale *= 1.4;
    Bones["LLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("LLeg"), "armorLeftLeg_Mtrl");
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 8.f * Scale, 3.f * Scale);
    Bones["LLeg"]->GetComponent<TransformComponent>()->SetPosition(0.f, 0.f, 0.f);

    Bones["RLeg"] = Bone::Create(owner, objType, _vec3(Scale / 2.f, Scale / 2.f * 3.f, Scale / 2.f), player->GetBone("RLeg"), "armorRightLeg_Mtrl");
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetScale(4.f * Scale, 8.f * Scale, 3.f * Scale);
    Bones["RLeg"]->GetComponent<TransformComponent>()->SetPosition(0.f, 0.f, 0.f);

    for (auto& bone : Bones)
    {
        bone.second->GetComponent<MeshRenderer>()->SetRenderID(RENDER_ID::Render_Alpha);
        owner->AddObject(ObjectType::Bone, bone.second);
    }
    return S_OK;
}

void Armor::Update(_float dt)
{
    Item::Update(dt);
}

void Armor::Late_Update(_float dt)
{
    Item::Late_Update(dt);
}

std::unordered_map<string,Object*> Armor::GetBones()
{
    return Bones;
}
