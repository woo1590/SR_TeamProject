#include "EnginePCH.h"
#include "InputSystem.h"
#include "EngineCore.h"

int g_iKey[256] =
{
	VK_UP,
	VK_DOWN,
	VK_LEFT,
	VK_RIGHT,
	VK_SPACE,
	'Q','W','E','R','T','Y','U','I','O','P',
	'A','S','D','F','G','H','J','K','L',
	'Z','X','C','V','B','N','M',
	'0','1','2','3','4','5','6','7','8','9',
	VK_LSHIFT,VK_RSHIFT,
	VK_LCONTROL,VK_RCONTROL,
	VK_TAB,
	VK_ESCAPE,
	VK_LBUTTON,
	VK_RBUTTON,
};

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

InputSystem* InputSystem::Create()
{
	InputSystem* Instance = new InputSystem;

	if (FAILED(Instance->Ready_InputSystem()))
	{
		Safe_Release(Instance);

		Instance = nullptr;	
	}

	return Instance;
}

HRESULT InputSystem::Ready_InputSystem()
{
	m_vecKey.reserve(static_cast<size_t>(KEY_END));

	for (int i = 0; i < static_cast<int>(KEY_END); ++i)
	{
		m_vecKey.push_back({ KS_NONE,false });
	}

	MousePos = { 0,0 };
	LastMousePos = { 0.f,0.f };

	RECT clientRect;
	GetClientRect(EngineCore::GetInstance()->GetWindowHandle(), &clientRect);

	_float x = clientRect.right * 0.5f;
	_float y = clientRect.bottom * 0.5f;

	CenterMousePos = { x,y };

	return S_OK;
}

void InputSystem::BeginFrame()
{
	POINT pos{};
	
	GetCursorPos(&pos);
	ScreenToClient(EngineCore::GetInstance()->GetWindowHandle(), &pos);
	MousePos = { (float)pos.x,(float)pos.y };

	for (int i = 0; i < m_vecKey.size(); ++i)
	{
		if (GetAsyncKeyState(g_iKey[i]) & 0x8000)	//키 누름 감지
		{
			if (m_vecKey[i].bPrePress)	//이전에 누르고 지금도 누름
			{
				m_vecKey[i].eKeyState = KEY_DOWN;
			}
			else						//이전에 누르지 않았지만 지금 누름	
			{
				m_vecKey[i].bPrePress = true;
				m_vecKey[i].eKeyState = KEY_PRESS;
			}
		}
		else
		{
			if (m_vecKey[i].bPrePress)//이전에 눌렀지만 지금은 누르지 않음
			{
				m_vecKey[i].bPrePress = false;
				m_vecKey[i].eKeyState = KEY_RELEASE;
			}
		}
	}
}

void InputSystem::EndFrame()
{
	MouseDelta = { 0.f,0.f };
}

KEY_STATE InputSystem::getKeyState(KEY _eKey)
{
	return m_vecKey[static_cast<int>(_eKey)].eKeyState;
}

bool InputSystem::IsKeyPressed(KEY _ekey)
{
	if (m_vecKey[_ekey].eKeyState == KEY_PRESS && Is_KeyWork) return true;

	return false;
}

bool InputSystem::IsKeyDown(KEY _ekey)
{
	if (m_vecKey[_ekey].eKeyState == KEY_DOWN && Is_KeyWork) return true;

	return false;
}

bool InputSystem::IsKeyRelease(KEY _ekey)
{
	if (m_vecKey[_ekey].eKeyState == KEY_RELEASE && Is_KeyWork) return true;

	return false;
}

_vec2 InputSystem::GetMousePos()const
{
	return MousePos;
}

_vec2 InputSystem::GetMouseDelta() const
{
	_vec2 res = MouseDelta;
	return res;
}

void InputSystem::SetMouseDelta(_vec2 delta)
{
	if (!EventLock)
	{
		if (First_Mouse)
		{
			MouseDelta = { 0.f,0.f };
			First_Mouse = false;
		}
		else
		{
			MouseDelta = delta;
		}

	}
}

void InputSystem::Free()
{
}
