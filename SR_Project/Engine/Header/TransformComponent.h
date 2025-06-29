#pragma once
#include "ObjectComponent.h"

BEGIN(Engine)

class ENGINE_DLL TransformComponent :
    public ObjectComponent
{
private:
    explicit TransformComponent(Object* owner);
    virtual ~TransformComponent();

public:
    static TransformComponent* Create(Object* owner);

    void Update(float dt)override;

    void SetPosition(float x, float y, float z);
    void SetPosition(_vec3 position);

    void SetPosition(float x, float y) { SetPosition(x, y, 0.f); }

    void SetScale(float cx, float cy, float cz);
    void SetScale(_vec3 scale);

    void SetRotate(float pitch, float yaw, float roll);
    void SetRotate(_vec3 rotate);

    void SetForward(_vec3 forward);

    void SetParent(Object* parent);
    void SetParent(TransformComponent* parent);

    void Translate(_vec3 velocity);
    void Translate(float x, float y, float z);

    _vec3 GetPosition()const;
    _vec3 GetScale()const;
    _vec3 GetRotate()const;
    _vec3 GetFoward()const;
    _vec3 GetRight()const;
    _vec3 GetUp()const;

    _matrix GetWorldMatrix()const;
    _matrix GetLocalMatrix()const;

private:
    void Free()override;

    _vec3 Position{ 0.f,0.f,0.f };
    _vec3 Scale{ 1.f,1.f,1.f };
    _vec3 Rotation{ 0.f,0.f,0.f };

    _vec3 Right{ 1.f,0.f,0.f };
    _vec3 Up{ 0.f,1.f,0.f };
    _vec3 Forward{ 0.f,0.f,1.f };

    TransformComponent* Parent = nullptr;
};

END