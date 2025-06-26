#pragma once

BEGIN(Engine)

class Frame;
class ENGINE_DLL FrameManager :  public Base
{
private:
    FrameManager();
    virtual ~FrameManager();

public:
    HRESULT AddFrame(const std::wstring frameTag, const _float callLimit);
    _bool IsPermitCall(const std::wstring frameTag, const _float dt);

    static FrameManager* Create();
private:
    std::unordered_map<std::wstring, Frame*> FrameMap;

    Frame* FindFrame(const std::wstring frameTag);
    void Free()override;
};

END