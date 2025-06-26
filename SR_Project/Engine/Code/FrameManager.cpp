#include "EnginePCH.h"
#include "FrameManager.h"
#include "Frame.h"

FrameManager::FrameManager()
{
}

FrameManager::~FrameManager()
{

}

HRESULT FrameManager::AddFrame(const std::wstring frameTag, const _float callLimit)
{
	Frame* frame = FindFrame(frameTag);

	if (frame)
		return E_FAIL;

	frame = Frame::Create(callLimit);
	if (!frame)
		return E_FAIL;

	FrameMap.insert({ frameTag,frame });

	return S_OK;
}

_bool FrameManager::IsPermitCall(const std::wstring frameTag, const _float dt)
{
	Frame* frame = FindFrame(frameTag);

	if (!frame)
		return false;

	return frame->IsPermitCall(dt);
}

FrameManager* FrameManager::Create()
{
	FrameManager* Instance = new FrameManager;

	return Instance;
}

Frame* FrameManager::FindFrame(const std::wstring frameTag)
{
	auto iter = FrameMap.find(frameTag);

	if (iter != FrameMap.end())
		return iter->second;
	else
		return nullptr;
}

void FrameManager::Free()
{
	std::for_each(FrameMap.begin(), FrameMap.end(), DeleteMap());
	FrameMap.clear();
}
