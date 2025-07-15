#pragma once

BEGIN(Engine)

class ENGINE_DLL Mesh : public Base
{
protected:
    Mesh();
    virtual ~Mesh();

public:
    virtual void Draw()PURE;

    /// <이지훈>
    void SetRender(bool render) { isRender = render; }
    bool GetRender() { return isRender; }
    /// <07.15 추가 - 청크 로딩>

protected:
    void Free();

    /// <이지훈>
    bool isRender = true;
    /// <07.15 추가 - 청크 로딩>

    LPDIRECT3DDEVICE9 Device = nullptr;
};

END