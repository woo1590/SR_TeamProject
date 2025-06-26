#pragma once

BEGIN(Engine)

class ENGINE_DLL Shader : public Base
{
private:
    Shader() {}
    virtual ~Shader() {}

public:
    static Shader* Create() { return {}; }

private:
    void Free()override {}

};

END