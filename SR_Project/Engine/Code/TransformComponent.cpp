#include "EnginePCH.h"
#include "TransformComponent.h"
#include "MyMath.h"

//object
#include "Object.h"

TransformComponent::TransformComponent(Object* owner)
    :ObjectComponent(owner), Parent(nullptr)
{
}

TransformComponent::~TransformComponent()
{
}

TransformComponent* TransformComponent::Create(Object* owner)
{
    TransformComponent* Instance = new TransformComponent(owner);

    if (FAILED(Instance->Ready_Component()))
    {
        Safe_Release(Instance);

        Instance = nullptr;
    }

    return Instance;
}

void TransformComponent::Update(float dt)
{

}

void TransformComponent::SetPosition(float x, float y, float z)
{
    SetPosition(_vec3(x, y, z));
}

void TransformComponent::SetPosition(_vec3 position)
{
    Position = position;
}


void TransformComponent::SetWorldPosition(const _vec3 worldPos)
{
    if (!Parent)
    {
        Position = worldPos;
        return;
    }

    _matrix parentWorld = Parent->GetWorldMatrix();
    _matrix invParentWorld;
    D3DXMatrixInverse(&invParentWorld, nullptr, &parentWorld);

    _vec3 localPos;
    D3DXVec3TransformCoord(&localPos, &worldPos, &invParentWorld);

    Position = localPos;
}

void TransformComponent::SetScale(float cx, float cy, float cz)
{
    SetScale(_vec3(cx, cy, cz));
}

void TransformComponent::SetScale(_vec3 scale)
{
    Scale = scale;
}

void TransformComponent::SetScale(float cx, float cy)
{
    SetScale(_vec3(cx, cy, 0.f));
}

void TransformComponent::SetRotate(float pitch, float yaw, float roll)
{
    SetRotate(_vec3(pitch, yaw, roll));
}

void TransformComponent::SetRotate(_vec3 rotate)
{
    rotate.x = math::NormalizeAngle(rotate.x);
    rotate.y = math::NormalizeAngle(rotate.y);
    rotate.z = math::NormalizeAngle(rotate.z);

    Rotation = rotate;
}

void TransformComponent::SetForward(_vec3 forward)
{
    Forward = forward;
    _vec3 worldUp = { 0.f,1.f,0.f };
    if (D3DXVec3Dot(&forward, &worldUp) > 0.999f)
        worldUp = { 0.f,1.01f,0.f };

    D3DXVec3Cross(&Right, &worldUp, &Forward);
    D3DXVec3Normalize(&Right, &Right);
    D3DXVec3Cross(&Up, &Forward, &Right);
    D3DXVec3Normalize(&Up, &Up);

    _float pitch = asin(std::clamp(-Forward.y, -1.f, 1.f));
    _float yaw = atan2(Forward.x, Forward.z);
    _float roll;

    if (fabsf(cosf(pitch)) > 0.0001f) {
        roll = atan2f(Right.y, Up.y);
    }
    else {
        roll = 0.0f;
    }

    Rotation = { pitch,yaw,roll };
}

void TransformComponent::SetPivot(float px, float py, float pz)
{
    SetPivot(_vec3(px, py, pz));
}

void TransformComponent::SetPivot(_vec3 pivot)
{
    Pivot = pivot;
}

void TransformComponent::SetPivotEnable(_bool enabled)
{
    IsPivotEnabled = enabled;
}

void TransformComponent::SetParent(Object* parent)
{
    if (parent == nullptr) Parent = nullptr;
    else Parent = parent->GetComponent<TransformComponent>();
}

void TransformComponent::SetParent(TransformComponent* parent)
{
    Parent = parent;
}

void TransformComponent::Translate(_vec3 velocity)
{
    Position += velocity;
}

void TransformComponent::Translate(float x, float y, float z)
{
    Translate(_vec3(x, y, z));
}

void TransformComponent::LookAt(const _vec3& targetPos)
{
    _vec3 dir = targetPos - Position;
    D3DXVec3Normalize(&dir, &dir);

    _float pitch = asinf(dir.y);
    _float yaw = atan2f(dir.x, dir.z);

    SetRotate({pitch, yaw, 0.f});
}

_vec3 TransformComponent::GetPosition() const
{
    return Position;
}

_vec3 TransformComponent::GetWorldPosition() const
{
    _matrix worldMat = GetWorldMatrix();
    return _vec3(worldMat._41, worldMat._42, worldMat._43);
}

_vec3 TransformComponent::GetScale() const
{
    return Scale;
}

_vec2 TransformComponent::GetScale2D() const
{
    return _vec2(Scale.x, Scale.y);
}

_vec3 TransformComponent::GetRotate() const
{
    return Rotation;
}

_vec3 TransformComponent::GetFoward() const
{
    return Forward;
}

_vec3 TransformComponent::GetRight() const
{
    return Right;
}

_vec3 TransformComponent::GetUp() const
{
    return Up;
}

TransformComponent* TransformComponent::GetParent() const
{
    return Parent;
}

_matrix TransformComponent::GetWorldMatrix() const
{
    _matrix worldMat = GetLocalMatrix();

    if (Parent) {
        _matrix matParentWorld = Parent->GetWorldMatrix();

        // �θ��� ������ ����� ����
        _vec3 vParentScale = Parent->GetScale();

        _matrix matScaleInverse;
        D3DXMatrixScaling(&matScaleInverse,
            vParentScale.x != 0.f ? 1.f / vParentScale.x : 1.f,
            vParentScale.y != 0.f ? 1.f / vParentScale.y : 1.f,
            vParentScale.z != 0.f ? 1.f / vParentScale.z : 1.f);

        // ������ ���ŵ� �θ� ��� = �θ���� * �θ������� �����
        matParentWorld = matScaleInverse * matParentWorld;

        // �ڽ��� ���� ��Ŀ� ����
        worldMat *= matParentWorld;
    }

    return worldMat;
}

_matrix TransformComponent::GetLocalMatrix() const
{
    _matrix pivotMat;
    _matrix reverseMat;

    _matrix transMat;
    _matrix rotX;
    _matrix rotY;
    _matrix rotZ;
    _matrix scaleMat;

    _matrix localMat;

    D3DXMatrixTranslation(&pivotMat, Pivot.x, Pivot.y, Pivot.z);
    D3DXMatrixTranslation(&reverseMat, -Pivot.x, -Pivot.y, -Pivot.z);

    D3DXMatrixTranslation(&transMat, Position.x, Position.y, Position.z);
    D3DXMatrixRotationX(&rotX, Rotation.x);
    D3DXMatrixRotationY(&rotY, Rotation.y);
    D3DXMatrixRotationZ(&rotZ, Rotation.z);
    D3DXMatrixScaling(&scaleMat, Scale.x, Scale.y, Scale.z);

    if (IsPivotEnabled)
    {
        if (IsBlock) localMat = scaleMat * reverseMat * rotY * rotX * rotZ * pivotMat * transMat;
        else localMat = scaleMat * transMat * pivotMat * rotY * rotX * rotZ * reverseMat;
    }
    else
    {
        localMat = scaleMat * rotY * rotX * rotZ * transMat;
    }

    return localMat;
}

_matrix TransformComponent::GetTranslateMatrix() const
{
    _matrix transMat;
    D3DXMatrixTranslation(&transMat, Position.x, Position.y, Position.z);

    return transMat;
}

_matrix TransformComponent::GetRotationMatrix() const
{
    _matrix rotX;
    _matrix rotY;
    _matrix rotZ;
    D3DXMatrixRotationX(&rotX, Rotation.x);
    D3DXMatrixRotationY(&rotY, Rotation.y);
    D3DXMatrixRotationZ(&rotZ, Rotation.z);

    return rotY * rotX * rotZ;
}

_matrix TransformComponent::GetScaleMatrix() const
{
    _matrix scaleMat;
    D3DXMatrixScaling(&scaleMat, Scale.x, Scale.y, Scale.z);

    return scaleMat;
}

void TransformComponent::Free()
{

}