#include "EnginePCH.h"
#include "Frame.h"

Frame::Frame() :CallLimit(0.f), AccDeltaTime(0.f)
{
}

Frame::~Frame()
{
}

HRESULT Frame::Ready_Frame(const _float callLimit)
{
	CallLimit = 1.f / callLimit;

	return S_OK;
}

_bool Frame::IsPermitCall(_float dt)
{
	AccDeltaTime += dt;
	if (AccDeltaTime >= CallLimit)
	{
		AccDeltaTime = 0.f;

		return true;
	}

	return false;
}

Frame* Frame::Create(const _float callLimit)
{
	Frame* frame = new Frame;

	if (FAILED(frame->Ready_Frame(callLimit)))
	{
		Safe_Release<Frame*>(frame);

		return nullptr;
	}

	return frame;
}

void Frame::Free()
{
}
