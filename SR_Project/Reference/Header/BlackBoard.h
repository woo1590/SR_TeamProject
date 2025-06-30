#pragma once
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL BlackBoard :
    public Base
{
protected:
    explicit BlackBoard();
    virtual ~BlackBoard();

public:
    static BlackBoard* Create();

    void SetValue(const std::string& key, const void* value);
    void* GetValue(const std::string& key) const;
    bool HasKey(const std::string& key) const;

protected:
    void Free() override;

protected:
    std::unordered_map<std::string, void*> Data;
};

END