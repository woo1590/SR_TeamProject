#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL Prefab : public Base
{
public:
    explicit Prefab(const std::string& name);
    virtual ~Prefab();

public:
    const std::string& GetName() const { return Name; }
    const std::vector<PREFAB>& GetBlocks() const { return Blocks; }

    void AddBlock(const PREFAB& block) { Blocks.push_back(block); }
    void ClearBlocks() { Blocks.clear(); }

    bool SaveToFile(const std::string& filepath) const;
    bool LoadFromFile(const std::string& filepath);

private:
    void Free() override;

private:
    std::string Name;
    std::vector<PREFAB> Blocks;
};
END