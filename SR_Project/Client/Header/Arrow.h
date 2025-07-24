#pragma once
#include "Item.h"
class Arrow : public Item
{
    Arrow(ObjectManager* owner, ObjectType objType);
    Arrow(ObjectManager* owner, ObjectType objType, float _damagePercent);
    virtual ~Arrow();
    void Free() override;
public:
    static Arrow* Create(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection);
    static Arrow* Create(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection, float damagePercent);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType, Object* shooter, _vec3 normalDirection);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

protected:
    void SetCollisionEnter(Object* other);

    void ArrowRotateSet();

    void PlayerArrowInfo();
    void MonsterArrowInfo();

    Object* hitObject = nullptr;
    _vec3 hitObjectPos = { 0.f,0.f,0.f };

    float arrowSpeed = 0.f;
    _vec3 arrowDirection = { 0.f,0.f,0.f };

    float shootTime = 0.f;
    float hitTime = 0.f;
    float arrowPersistTime = 0.5f;

    float damagePercent = 1.f;
};

