#pragma once

BEGIN(Engine)

class Object;
class ENGINE_DLL ObjectComponent : public Base
{
protected:
    explicit ObjectComponent(Object* owner);
    virtual ~ObjectComponent();

public:
    virtual HRESULT Ready_Component();
    virtual void Update(float dt) {}
    virtual void Late_Update(float dt) {}
    Object* GetOwner()const;

protected:
    void Free()override;

    Object* owner;
};

END