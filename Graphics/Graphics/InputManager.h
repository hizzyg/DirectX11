#pragma once

#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include <dinput.h>

class CInputManager
{
public:
	CInputManager();
	~CInputManager();

	int InitDirectInput(HINSTANCE _hInstance);
	void DetectInput();
	void Cleanup();

	bool GetKey(int _keyID);
	bool GetKeyUp(int _keyID);
	bool GetKeyDown(int _keyID);

	inline XMFLOAT2 GetMousePos() { return m_mousePos; }
	XMFLOAT2 GetMouseMovement();
	float GetMouseWheel();

	bool GetMouseKey(int _keyID);
	bool GetMouseKeyUp(int _keyID);
	bool GetMouseKeyDown(int _keyID);

private:
	IDirectInputDevice8* m_mouse;
	IDirectInputDevice8* m_keyboard;

	LPDIRECTINPUT8 m_directInput;

	DIMOUSESTATE m_currentMouseState;
	DIMOUSESTATE m_lastMouseState;
	BYTE m_currentKeyboardState[256];
	BYTE m_lastKeyboardState[256];

	XMFLOAT2 m_mousePos;
};

