#pragma once
#include "Object.h"
class Item :
    public Object
{
public:
    enum class ItemType {
        ITEM_SWORD,
        ITEM_BOW
    };
protected:
    Item(ObjectManager* owner, ObjectType objType);
    virtual ~Item();
    void Free() override;
public:
    static Item* Create(ObjectManager* owner, ObjectType objType);
    HRESULT Ready_Object(ObjectManager* owner, ObjectType objType);
    void Update(_float dt)override;
    void Late_Update(_float dt)override;

    void SetScale(_float _scale);
    void SetScaleRatio(_vec3 _scaleRatio);
    void SetPosition(_vec3 _position);
    void SetPivot(bool _pivotEnable, _vec3 _pivotPosition = {0.f,0.f,0.f});
    void SetRotation(_vec3 _rotation);
    void SetRenderId(Engine::RENDER_ID _renderId);
    void SetOwnerObject(Object* _ownerObject);
    void SetMesh(std::string _mesh);
    void SetMaterial(std::string _material);

    void ApplyComponents();

protected:
    float itemScale = 1.f;
    _vec3 itemScaleRatio = { 1.f,1.f,1.f };
    _vec3 itemPosition = { 0.f, 0.f, 0.f };
    bool itemPivotEnable = false;
    _vec3 pivotPosition = { 0.f,0.f,0.f };
    _vec3 itemRotation = { 0.f,0.f,0.f };
    
    std::string meshType;
    std::string material;
    Engine::RENDER_ID renderId = Engine::RENDER_ID::Render_Alpha;

    Object* ownerObject = nullptr;
};

