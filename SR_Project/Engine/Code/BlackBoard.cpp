#include "EnginePCH.h"
#include "BlackBoard.h"

BlackBoard::BlackBoard()
{
}

BlackBoard::~BlackBoard()
{
    Free();
}

BlackBoard* BlackBoard::Create()
{
    BlackBoard* Instance = new BlackBoard();
    return Instance;
}

void BlackBoard::SetValue(const std::string& key, const void* value)
{
    Data[key] = const_cast<void*>(value);
}

void* BlackBoard::GetValue(const std::string& key) const
{
    auto it = Data.find(key);

    if (it != Data.end())
        return it->second;

    return nullptr;
}

bool BlackBoard::HasKey(const std::string& key) const
{
    return Data.find(key) != Data.end();
}

void BlackBoard::Free()
{
   //
}
