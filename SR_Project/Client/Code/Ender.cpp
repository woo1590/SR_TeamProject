#include "pch.h"
#include "Ender.h"
#include "CollisionComponent.h"
#include "TransformComponent.h"

Ender::Ender(ObjectManager* owner, ObjectType objType)
	:Monster(owner, objType)
{
}

Ender::~Ender()
{
}

Ender* Ender::Create(ObjectManager* owner, ObjectType objType)
{
    Ender* Instance = new Ender(owner, objType);

    if (FAILED(Instance->Ready_Object(owner, objType)))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

HRESULT Ender::Ready_Object(ObjectManager* owner, ObjectType objType)
{
    Monster::Ready_Object(owner, objType);

    InitTransform(objType);
    InitTree();
    InitAnimation();

    auto collision = GetComponent<CollisionComponent>();
    collision->SetSize(_vec3(10.f, 20.f, 20.f));

	return S_OK;
}

void Ender::Update(_float dt)
{
    Monster::Update(dt);
    PlayAnimation(dt);
}

void Ender::Late_Update(_float dt)
{
    Monster::Late_Update(dt);
}

void Ender::InitTransform(ObjectType objType)
{
    auto transform = AddComponent<TransformComponent>();
    Bones["Body"]->GetComponent<TransformComponent>()->SetParent(transform);

    Add_Bone("TopHead", objType, Bones["Head"], "EnderTopHead_Mtrl");
    SetMaterial("EnderBottomHead_Mtrl", "Head", RENDER_ID::Render_NonAlpha);
    SetMaterial("EnderBody_Mtrl", "Body");

    transform->SetRotate(_vec3(0.f, D3DXToRadian(90.f), 0.f/*D3DXToRadian(-90.f)*/));
    transform->SetPosition(10 , 100, 10);
    
    auto toptransform = Bones["TopHead"]->GetComponent<TransformComponent>();
    toptransform->SetScale(24.f * Scale, 18.f * Scale, 24.f * Scale);
    toptransform->SetPosition(0.0f, 24.f * Scale, 0.f);
    toptransform->SetRotate(D3DXToRadian(-45.f), 0.f, 0.f);
    toptransform->SetPivot(0.f, -18.f* Scale, 24.f * Scale);
    toptransform->SetPivotEnable(true);

    auto bottomtransform = Bones["Head"]->GetComponent<TransformComponent>();
    bottomtransform->SetScale(24.f * Scale, 6.f * Scale, 24.f * Scale);
    bottomtransform->SetPosition(0.f * Scale, 66.f * Scale, 0.f * Scale);
    bottomtransform->SetPivot(0.f, -84.f * Scale,0.f);
    bottomtransform->SetPivotEnable(true);
    bottomtransform->SetRotate(_vec3(/*D3DXToRadian(-90.f)*/0.f, 0.f, 0.f));

    auto bodytransform = Bones["Body"]->GetComponent<TransformComponent>();
    bodytransform->SetScale(16.f * Scale, 60.f * Scale, 16.f * Scale);

    Add_Bone("LLeg1", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto leg1transrorm = Bones["LLeg1"]->GetComponent<TransformComponent>();
    leg1transrorm->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg1transrorm->SetPosition(40.f* Scale, 45.f* Scale, 0 * Scale);
    leg1transrorm->SetPivot(-24.f * Scale, 0.f, 0.f);
    leg1transrorm->SetPivotEnable(true);
    leg1transrorm->SetRotate(0.f, D3DXToRadian(45.f), 0.f);

    Add_Bone("LLeg2", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto leg2transform = Bones["LLeg2"]->GetComponent<TransformComponent>();
    leg2transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg2transform->SetPosition(40.f * Scale, 0 * Scale, 0 * Scale);
    leg2transform->SetPivot(-24.f * Scale, 0.f, 0.f);
    leg2transform->SetPivotEnable(true);
    leg2transform->SetRotate(0.f, D3DXToRadian(45.f), 0.f);

    Add_Bone("LLeg3", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto leg3transform = Bones["LLeg3"]->GetComponent<TransformComponent>();
    leg3transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    leg3transform->SetPosition(40.f * Scale, -45.f * Scale, 0 * Scale);
    leg3transform->SetPivot(-24.f * Scale, 0.f, 0.f);
    leg3transform->SetPivotEnable(true);
    leg3transform->SetRotate(0.f, D3DXToRadian(45.f), 0.f);

    Add_Bone("LFoot1", ObjectType::Bone, Bones["LLeg1"], "EnderBody_Mtrl");
    auto foot1transform = Bones["LFoot1"]->GetComponent<TransformComponent>();
    foot1transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot1transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot1transform->SetPivot(-30.f * Scale, 0.f, 0.f);
    foot1transform->SetPivotEnable(true);
    foot1transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);

    Add_Bone("LFoot2", ObjectType::Bone, Bones["LLeg2"], "EnderBody_Mtrl");
    auto foot2transform = Bones["LFoot2"]->GetComponent<TransformComponent>();
    foot2transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot2transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot2transform->SetPivot(-30.f * Scale, 0.f, 0.f);
    foot2transform->SetPivotEnable(true);
    foot2transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);

    Add_Bone("LFoot3", ObjectType::Bone, Bones["LLeg3"], "EnderBody_Mtrl");
    auto foot3transform = Bones["LFoot3"]->GetComponent<TransformComponent>();
    foot3transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    foot3transform->SetPosition(54.f * Scale, 0.f, 0 * Scale);
    foot3transform->SetPivot(-30.f * Scale, 0.f, 0.f);
    foot3transform->SetPivotEnable(true);
    foot3transform->SetRotate(0.f, D3DXToRadian(-90.f), 0.f);
 
    Add_Bone("RLeg1", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto rleg1transrorm = Bones["RLeg1"]->GetComponent<TransformComponent>();
    rleg1transrorm->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg1transrorm->SetPosition(-40.f * Scale, 45.f * Scale, 0 * Scale);
    rleg1transrorm->SetPivot(24.f * Scale, 0.f, 0.f);
    rleg1transrorm->SetPivotEnable(true);
    rleg1transrorm->SetRotate(0.f, D3DXToRadian(-45.f), 0.f);

    Add_Bone("RLeg2", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto rleg2transform = Bones["RLeg2"]->GetComponent<TransformComponent>();
    rleg2transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg2transform->SetPosition(-40.f * Scale, 0 * Scale, 0 * Scale);
    rleg2transform->SetPivot(24.f * Scale, 0.f, 0.f);
    rleg2transform->SetPivotEnable(true);
    rleg2transform->SetRotate(0.f, D3DXToRadian(-45.f), 0.f);

    Add_Bone("RLeg3", ObjectType::Bone, Bones["Body"], "EnderBody_Mtrl");
    auto rleg3transform = Bones["RLeg3"]->GetComponent<TransformComponent>();
    rleg3transform->SetScale(24.f * Scale, 5.f * Scale, 5.f * Scale);
    rleg3transform->SetPosition(-40.f * Scale, -45.f * Scale, 0 * Scale);
    rleg3transform->SetPivot(24.f * Scale, 0.f, 0.f);
    rleg3transform->SetPivotEnable(true);
    rleg3transform->SetRotate(0.f, D3DXToRadian(-45.f), 0.f);

    Add_Bone("RFoot1", ObjectType::Bone, Bones["RLeg1"], "EnderBody_Mtrl");
    auto rfoot1transform = Bones["RFoot1"]->GetComponent<TransformComponent>();
    rfoot1transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot1transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot1transform->SetPivot(30.f * Scale, 0.f, 0.f);
    rfoot1transform->SetPivotEnable(true);
    rfoot1transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    
    Add_Bone("RFoot2", ObjectType::Bone, Bones["RLeg2"], "EnderBody_Mtrl");
    auto rfoot2transform = Bones["RFoot2"]->GetComponent<TransformComponent>();
    rfoot2transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot2transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot2transform->SetPivot(30.f * Scale, 0.f, 0.f);
    rfoot2transform->SetPivotEnable(true);
    rfoot2transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);
    
    Add_Bone("RFoot3", ObjectType::Bone, Bones["RLeg3"], "EnderBody_Mtrl");
    auto rfoot3transform = Bones["RFoot3"]->GetComponent<TransformComponent>();
    rfoot3transform->SetScale(30.f * Scale, 5.f * Scale, 5.f * Scale);
    rfoot3transform->SetPosition(-54.f * Scale, 0.f, 0 * Scale);
    rfoot3transform->SetPivot(30.f * Scale, 0.f, 0.f);
    rfoot3transform->SetPivotEnable(true);
    rfoot3transform->SetRotate(0.f, D3DXToRadian(90.f), 0.f);

    Bones["LArm"]->SetDead();
    Bones["LArm"] = nullptr;

    Bones["RArm"]->SetDead();
    Bones["RArm"] = nullptr;

    Bones["LLeg"]->SetDead();
    Bones["LLeg"] = nullptr;

    Bones["RLeg"]->SetDead();
    Bones["RLeg"] = nullptr;
}

void Ender::InitTree()
{
}

void Ender::InitAnimation()
{
}

void Ender::PlayAnimation(_float dt)
{
}

void Ender::Free()
{
    Monster::Free();
}
