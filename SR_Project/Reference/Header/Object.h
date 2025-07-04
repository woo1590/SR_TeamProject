#pragma once

BEGIN(Engine)

class ObjectComponent;
class ObjectManager;
class Scene;
class ENGINE_DLL Object :  public Base
{
protected:
    explicit Object(ObjectManager* owner, ObjectType objType);
    virtual ~Object();

public:
    virtual HRESULT Ready_Object();
    virtual void Update(float dt);
    virtual void Late_Update(float dt);

    ObjectType GetObjectType()const;
    virtual void SetDead();
    _bool IsDead()const;
    ObjectManager* GetOwner()const;
    Scene* GetScene()const;

    template<typename T,typename... Args>
    T* AddComponent(Args&&... args)
    {
        T* comp = T::Create(this, std::forward<Args>(args)...);

        Components.push_back(comp);

        ComponentMap[typeid(T)] = comp;
        comp->AddRef();

        return comp;
    }

    template<typename T>
    T* GetComponent()
    {
        auto it = ComponentMap.find(typeid(T));
        if (it != ComponentMap.end())
            return static_cast<T*>(it->second);
        else
            return nullptr;
    }

    template<typename T>
    vector<T*> GetComponents()
    {
        vector<T*> result;
        for (auto* component : Components)
        {
            if (auto* casted = dynamic_cast<T*>(component))
                result.push_back(casted);
        }
        return result;
    }

protected:
    void Free()override;

    _bool Is_Dead = false;
    ObjectManager* owner;
    ObjectType ObjType;

    std::vector<ObjectComponent*> Components;
    std::unordered_map<std::type_index, ObjectComponent*> ComponentMap;
};

END

