#include "pch.h"
#include "Npc.h"
#include "TransformComponent.h"
#include "CollisionComponent.h"
#include "PhysicsComponent.h"
#include "Scene.h"
#include "PhysicsSystem.h"

Npc::Npc(ObjectManager* owner, ObjectType objType)
	:BaseCharacter(owner, objType)
{
}

Npc::~Npc()
{
}

Npc* Npc::Create(ObjectManager* owner, ObjectType objType)
{
    Npc* Instance = new Npc(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Npc::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    BaseCharacter::Ready_Object(owner, objType);
    InitTransform();
    InitCollision();
    InitPhysics();
	return S_OK;
}

void Npc::Update(_float dt)
{
    BaseCharacter::Update(dt);
}

void Npc::Late_Update(_float dt)
{
    BaseCharacter::Late_Update(dt);
}

void Npc::InitTransform()
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);
    transform->SetPosition(5.f, 100.f, 100.f);
    transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f));
    SetMaterial("VillageBody_Mtrl", "Body", RENDER_ID::Render_Alpha);
    SetScale(_vec3(8.f * Scale, 16.f * Scale, 4.f * Scale), "Body");
    SetMaterial("VillageHead_Mtrl", "Head");
    SetMaterial("VillageLeg_Mtrl", "LLeg");
    SetMaterial("VillageLeg_Mtrl", "RLeg");
    SetMaterial("VillageArm_Mtrl", "LArm");
    SetScale(_vec3(3.f* Scale, 6.f * Scale, 3.f * Scale), "LArm");
    SetRotation(_vec3(D3DXToRadian(-45.f), 0.f, 0.f), "LArm");
    SetMaterial("VillageArm_Mtrl", "RArm");
    SetScale(_vec3(3.f * Scale, 6.f * Scale, 3.f * Scale), "RArm");
    SetRotation(_vec3(D3DXToRadian(-45.f), 0.f, 0.f), "RArm");

    Add_Bone("RHand", ObjectType::Bone, Bones["RArm"], "VillageLeftHand_Mtrl");
    SetScale(_vec3(5.f * Scale, 3.f * Scale, 3.f * Scale), "RHand");
    Bones["RHand"]->GetComponent<TransformComponent>()->Translate(_vec3(-5.f * Scale, -4.f* Scale, -2.f * Scale));
    
    Add_Bone("LHand", ObjectType::Bone, Bones["LArm"], "VillageRightHand_Mtrl");
    SetScale(_vec3(5.f * Scale, 3.f * Scale, 3.f * Scale), "LHand");
    Bones["LHand"]->GetComponent<TransformComponent>()->Translate(_vec3(5.f * Scale, -4.f * Scale, -2.f * Scale));

    Add_Bone("Nose", ObjectType::Bone, Bones["Head"], "VillageNose_Mtrl");
    SetScale(_vec3(2.f * Scale, 4.f * Scale, 2.f * Scale), "Nose");
    SetPosition(_vec3(0.f, -4.f * Scale, Scale * 8.f), "Nose");
}

void Npc::InitCollision()
{
    auto collision = AddComponent<CollisionComponent>();
    GetScene()->GetCollisionSystem()->RegisterCollision(collision);//test
    collision->SetSize(_vec3(2.f, 7.f, 2.f));
    collision->SetLayer(LAYER_TRRIGER);
    collision->SetMask(LAYER_PLAYER | LAYER_DEFAULT);
}

void Npc::InitPhysics()
{
    auto physics = AddComponent<PhysicsComponent>();
    GetScene()->GetPhysicsStstem()->RegisterBody(physics);//test
    physics->SetMass(1.f);
}

void Npc::Free()
{
    BaseCharacter::Free();
}
