#pragma once

BEGIN(Engine)

class Scene;
class Object;
class ENGINE_DLL ObjectManager : public Base
{
private:
    explicit ObjectManager(Scene* owner);
    virtual ~ObjectManager();

public:
    static ObjectManager* Create(Scene* owner);

    HRESULT Ready_ObjectMgr();
    void Update(float dt);
    void Late_Update(float dt);

    void AddObject(ObjectType objType, Object* object);
    void RemoveObject(ObjectType objType, const _vec3& worldPosition);
    void ClearList(ObjectType objType);
    void AddUIObject(Object* obj);

    Object* GetFrontObject(ObjectType objType);
    std::list<Object*> GetObjectList(ObjectType objType);

private:
    void CleanDeadObject();
    void Free()override;

    std::vector<std::list<Object*>> Objects;
    std::list<std::list<Object*>::iterator> DeadObjects;
    Scene* owner;
};

END

