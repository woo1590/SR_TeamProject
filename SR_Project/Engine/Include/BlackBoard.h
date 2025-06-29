#pragma once
#include "Base.h"

class BlackBoard :
    public Base
{
protected:
    explicit BlackBoard();
    virtual ~BlackBoard();

public:
    void SetValue(const std::string& key, const void* value);
    void* GetValue(const std::string& key) const;
    bool HasKey(const std::string& key) const;

protected:
    void Free() override;

private:
    std::unordered_map<std::string, void*> Data;
};

