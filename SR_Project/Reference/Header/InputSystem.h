#pragma once

BEGIN(Engine)

enum KEY
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	SPACE,
	Q, W, E, R, T, Y, U, I, O, P,
	A, S, D, F, G, H, J, K, L,
	Z, X, C, V, B, N, M,
	NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
	LSHIFT, RSHIFT,
	LCTRL, RCTRL,
	TAB,
	ESC,
	LBUTTON,
	RBUTTON,
	KEY_END
};

enum KEY_STATE
{
	KS_NONE,
	KEY_PRESS,
	KEY_DOWN,
	KEY_RELEASE
};

typedef struct tagKeyInfo
{
	KEY_STATE	eKeyState = KS_NONE;
	bool		bPrePress = false;
}KEYINFO;

class ENGINE_DLL InputSystem :public Base
{
private:
	InputSystem();
	virtual ~InputSystem();

public:
	static InputSystem* Create();
	HRESULT Ready_InputSystem();
	void BeginFrame();
	void EndFrame();

	KEY_STATE getKeyState(KEY _eKey);
	bool IsKeyPressed(KEY _ekey);		//처음 누른 시점에만 true 반환
	bool IsKeyDown(KEY _ekey);			//누르고 있는 동안 계속 true 반환
	bool IsKeyRelease(KEY _ekey);		//누르고 키를 떼면 true 반환

	void SetMousePos()const;			//커서 위치 고정용
	_vec3 GetMousePos()const;

	_vec2 GetMouseDelta()const;
	void SetMouseDelta(_vec2 delta);

	void SetKeyWork(bool work) { Is_KeyWork = work; } 

private:
	void Free()override;

	std::vector<KEYINFO> m_vecKey;

	_vec3 MousePos{0.f,0.f,0.f};
	_vec3 LastMousePos{ 0.f,0.f,0.f };
	_vec3 CenterMousePos{ 0.f,0.f,0.f };
	_vec2 MouseDelta{ 0.f,0.f };

	bool Is_KeyWork = true;
	bool Is_MouseLock = true;
	bool First_Mouse = true;
	bool EventLock = false;
};

END