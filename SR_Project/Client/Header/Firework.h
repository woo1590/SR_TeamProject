#pragma once
#include "Item.h"
class Firework :
    public Item
{
    Firework(ObjectManager* owner, ObjectType objType);
    virtual ~Firework();
    void Free() override;
public:
    static Firework* Create(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    void SetCollisionEnter(Object* other);

    void ArrowRotateSet();

    void PlayerFireworkInfo();

    float fireworkSpeed = 0.f;
    _vec3 fireworkDirection = { 0.f,0.f,0.f };

    float shootTime = 0.f;
    float fireworkPersistTime = 5.f;

    const float fireworkRange = 10.f;
};

