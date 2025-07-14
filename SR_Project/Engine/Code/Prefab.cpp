#include "EnginePCH.h"
#include "Prefab.h"
#include "json.hpp"

using json = nlohmann::json;

Prefab::Prefab(const std::string& name) : Name(name)
{
}

Prefab::~Prefab()
{
}

bool Prefab::SaveToFile(const std::string& filepath) const
{
    json jArray = json::array();

    for (const auto& block : Blocks)
    {
        json jBlock;
        jBlock["localPos"] = { block.localPos.x, block.localPos.y, block.localPos.z };
        jBlock["Type"] = static_cast<int>(block.Type);
        jBlock["Axis"] = static_cast<int>(block.Axis);
        jBlock["Rot"] = static_cast<int>(block.Rot);
        jBlock["Usage"] = static_cast<int>(block.Usage);
        jArray.push_back(jBlock);
    }

    std::ofstream file(filepath);
    if (!file.is_open()) return false;

    file << jArray.dump(4);
    return true;
}

bool Prefab::LoadFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    json jArray;
    file >> jArray;
    Blocks.clear();

    for (const auto& jBlock : jArray)
    {
        PREFAB block;
        block.localPos.x = jBlock.at("localPos")[0].get<float>();
        block.localPos.y = jBlock.at("localPos")[1].get<float>();
        block.localPos.z = jBlock.at("localPos")[2].get<float>();
        block.Type = static_cast<StaticBlockType>(jBlock.at("Type").get<int>());
        block.Axis = static_cast<StaticBlockAxis>(jBlock.at("Axis").get<int>());
        block.Rot = static_cast<StaticBlockRot>(jBlock.at("Rot").get<int>());
        block.Usage = static_cast<StaticBlockUsage>(jBlock.at("Usage").get<int>());

        Blocks.push_back(block);
    }

    return true;
}

void Prefab::Free()
{
}