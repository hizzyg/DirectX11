#pragma once


// System Includes
#include <Windows.h>			// Nötig zur Fenstererzeugung

// DirectX
#include <d3d11.h>				// hauptheaderfile von DirectX 11
#include <d3dcompiler.h>		// Läd und kompiliert Shader
#include <DirectXMath.h>		// Vektoren und Matrizen
#include <DirectXColors.h>		// Schöne Farben!

// STL includes
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")

#include "DirectXTK-master\Inc\WICTextureLoader.h"
#include "DirectXTK-master\Inc\SpriteFont.h"
#include "DirectXTK-master\Inc\SpriteBatch.h"

// Mathezeug
#include <algorithm>

using namespace DirectX;


// Safetly release COM-Object
// Ein Component Object Model ist eine Schnittstelle zur Interprozesskommunikation
// C++ Templates sind NICHT typsicher -> Reine Textersetzung, Compilererrors bei falscher Benutzung
template<typename T>
inline void SafeRelease(T& _ptr)
{
	if (_ptr != nullptr)
	{
		_ptr->Release();
		_ptr = nullptr;
	}
}
