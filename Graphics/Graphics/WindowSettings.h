#pragma once

struct SWindowSettings
{
	const LONG m_windowWidth = 800;
	const LONG m_windowHeight = 600;

	LPCSTR m_windowClassName = "Graphics Class";
	LPCSTR m_windowName = "irgendwas cooles";
	HWND m_windowHandle = nullptr;

	const BOOL m_enableVSync = false;
};