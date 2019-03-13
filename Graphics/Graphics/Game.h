#pragma once
#include "WindowSettings.h"
#include "DirectXSettings.h"
#include "ContentManager.h"
#include "InputManager.h"
#include "AssetManager.h"

// TODO:
// Texturen
// Licht
// Shaderveränderungen zur echtzeit

#define WDS (*(CGame::Get()->GetWindowSettings()))
#define DXS (*(CGame::Get()->GetDirectXSettings()))
#define CTM (*(CGame::Get()->GetContentManager()))
#define IPM (*(CGame::Get()->GetInputManager()))
#define ASM (*(CGame::Get()->GetAssetManager()))

#define FAILHR(errorcode) if (FAILED(hr)) { return errorcode; }

// Singletons:
// Es darf nur einmal instanziert werden können
// Zugriff von Überall, meist mit static gelöst
// Sie sind faul, lazy loading
class CGame
{
private:
	CGame();
public:
	~CGame();

	// Hinweis für den Compiler statt einem Funktionsaufruf den Code am Zielort reinzuschreiben
	// Das ist nur ein Hinweis und muss nicht befolgt werden
	inline static CGame* Get()
	{
		static CGame* instance = new CGame();
		return instance;
	}

public:
	int Initialize(HINSTANCE _hInstance, int _argC);
	int Run();
	void Finalize();

	// Der Datentyp XMFLOAT3 muss alligned (speicherausrichtung) 16 sein aus performancegründen
	static void* operator new(size_t _size)
	{
		return _aligned_malloc(_size, 16);
	}

	static void operator delete(void* _memory)
	{
		_aligned_free(_memory);
	}

	inline SWindowSettings* GetWindowSettings() { return &m_windowSettings; }
	inline SDirectXSettings* GetDirectXSettings() { return &m_directXSettings; }
	inline CContentManager* GetContentManager() { return &m_contentManager; }
	inline CInputManager* GetInputManager() { return &m_inputManager; }
	inline CAssetManager* GetAssetManager() { return &m_assetManager; }

private:
	int InitApplication(HINSTANCE _hInstance, int _argC);
	int InitDirectX();
	int InitConstantBuffers();

	int CreateSimpleShader();
	int CreateTexturedShader();
	int CreateOSNMShader();
	int CreateTerrainShader();

	void LoadLevel();

	void Update(float _deltaTime);
	void Render();

	void CameraMovement(float _deltaTime);

public:


	XMFLOAT3 m_camPos;
	XMFLOAT3 m_camRot;
private:
	SWindowSettings m_windowSettings;
	SDirectXSettings m_directXSettings;
	CContentManager m_contentManager;
	CInputManager m_inputManager;
	CAssetManager m_assetManager;

	SStandardConstantBuffer m_applicationConstantBuffer;
	SStandardConstantBuffer m_frameConstantBuffer;
	SLightConstantBuffer m_lightConstantBuffer;

	bool m_isRunning;
};