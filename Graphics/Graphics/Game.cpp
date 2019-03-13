#include "PCH.h"
#include "Game.h"
#include <thread>
#include "Oktaeder.h"
#include "Sphere.h"
#include "TexturedCube.h"
#include "Image2D.h"
#include "Cursor2D.h"
#include "Button2D.h"
#include "Text2D.h"
#include "Helper.h"
#include "OSNMCube.h"
#include "Terrain.h"
#include "Target.h"

LRESULT CALLBACK WndProc(HWND _hwnd, UINT _message, WPARAM _wparam, LPARAM _lparam);


using namespace DirectX;

CGame::CGame()
{
}

CGame::~CGame()
{
}

int CGame::Initialize(HINSTANCE _hInstance, int _argC)
{
	if (!XMVerifyCPUSupport())
	{
		MessageBox(nullptr, "Failed to load DirectXMath library support", "Error", MB_OK);
		return -1;
	}

	// Fenster erzeugen
	int value = InitApplication(_hInstance, _argC);
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to create window", "Error", MB_OK);
		return value;
	}

	// DirectX initialisieren
	value = InitDirectX();
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to initialize DirectX", "Error", MB_OK);
		return value;
	}

	value = InitConstantBuffers();
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to initialize Constant Buffers", "Error", MB_OK);
		return value;
	}

	CTM.m_spriteBatch = new SpriteBatch(DXS.m_deviceContext);
	CTM.m_spriteFont = new SpriteFont(DXS.m_device, L"arial.spritefont");
	CTM.m_cursor = new CCursor2D(L"Crosshair.png");

	value = CreateSimpleShader();
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to create Simple Shader", "Error", MB_OK);
		return value;
	}

	value = CreateTexturedShader();
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to create Textured Shader", "Error", MB_OK);
		return value;
	}

	value = CreateOSNMShader();
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to create OSNM Shader", "Error", MB_OK);
		return value;
	}

	value = CreateTerrainShader();
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to create Terrain Shader", "Error", MB_OK);
		return value;
	}

	value = m_inputManager.InitDirectInput(_hInstance);
	if (FAILED(value))
	{
		MessageBox(nullptr, "Failed to initialize Direct Input", "Error", MB_OK);
		return value;
	}

	LoadLevel();

	m_isRunning = true;
	return 0;
}

int CGame::Run()
{
	MSG msg = { 0 };
	static const float targetFrameRate = 30.0f;
	static const float maxTimeStep = 1.0f / targetFrameRate;

	static DWORD previousTime = timeGetTime();
	static DWORD currentTime;
	float deltaTime;
	//float sleepTime;

	while (msg.message != WM_QUIT && m_isRunning)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			currentTime = timeGetTime();
			deltaTime = (currentTime - previousTime) * 0.001f;	// Tausendstel Sekunde -> Sekunde
			deltaTime = std::min<float>(deltaTime, maxTimeStep);

			//sleepTime = (maxTimeStep - deltaTime);
			//Sleep(sleepTime * 1000);

			previousTime = timeGetTime();

			Update(deltaTime);
			Render();
		}
	}

	return 0;

}

void CGame::Finalize()
{
}

int CGame::InitApplication(HINSTANCE _hInstance, int _argC)
{
	// Einfache Möglichkeit ein Struct mit 0 zu füllen, geht nicht immer, also diese beiden Alternativen drunter:
	WNDCLASSEX wndClass = { 0 };
	//ZeroMemory(&wndClass, sizeof(WNDCLASSEX));
	//memset(&wndClass,0, sizeof(WNDCLASSEX));

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;		// Was soll passieren wenn das Fenster etwas macht (zb resize)
	wndClass.hInstance = _hInstance;
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH) (COLOR_WINDOW + 9);
	wndClass.lpszClassName = m_windowSettings.m_windowClassName;

	if (!RegisterClassEx(&wndClass))
	{
		return -1;
	}

	RECT windowRect = {0, 0, m_windowSettings.m_windowWidth, m_windowSettings.m_windowHeight };
	AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	m_windowSettings.m_windowHandle = CreateWindowA(m_windowSettings.m_windowClassName,
													m_windowSettings.m_windowName,
													WS_OVERLAPPEDWINDOW,
													CW_USEDEFAULT,
													CW_USEDEFAULT,
													windowRect.right - windowRect.left,
													windowRect.bottom - windowRect.top,
													nullptr, 
													nullptr, 
													_hInstance,
													nullptr);

	if (!m_windowSettings.m_windowHandle)
	{
		return -1;
	}

	ShowWindow(m_windowSettings.m_windowHandle, _argC);
	UpdateWindow(m_windowSettings.m_windowHandle);


	return 0;
}

int CGame::InitDirectX()
{
	RECT clientRect;
	GetClientRect(m_windowSettings.m_windowHandle, &clientRect);

	unsigned int clientWidth = clientRect.right - clientRect.left;
	unsigned int clientHeight = clientRect.bottom - clientRect.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = clientWidth;
	swapChainDesc.BufferDesc.Height = clientHeight;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;		// Zähler
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;		// Nenner	// TODO: rumprobieren
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = m_windowSettings.m_windowHandle;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Windowed = true;

	

	unsigned int createDeviceFlags = 0;
#if _DEBUG
	createDeviceFlags = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	D3D_FEATURE_LEVEL featureLevel;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
					nullptr,	// Default Grafikkarte
					D3D_DRIVER_TYPE_HARDWARE,
					nullptr,	// Wir nutzen keinen Software Type
					createDeviceFlags,
					featureLevels,
					_countof(featureLevels),		// Funktioniert nur bei echten Arrays
					D3D11_SDK_VERSION,
					&swapChainDesc,
					&m_directXSettings.m_swapChain,
					&m_directXSettings.m_device,
					&featureLevel,
					&m_directXSettings.m_deviceContext);

	if (FAILED(hr))
	{
		return -30;
	}

	ID3D11Texture2D* backbuffer;
	hr = m_directXSettings.m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), 
									reinterpret_cast<void**>(&backbuffer));	// Verändere die Bits nichts, lese es nur als diesen anderen Datentyp
	if (FAILED(hr))
	{
		return -31;
	}
	hr = m_directXSettings.m_device->CreateRenderTargetView(backbuffer, nullptr, 
									&m_directXSettings.m_renderTargetView);
	if (FAILED(hr))
	{
		return -32;
	}

	SafeRelease(backbuffer);

	// Depth Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilBufferDesc = {0};
	depthStencilBufferDesc.ArraySize = 1;
	depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilBufferDesc.CPUAccessFlags = 0;
	depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilBufferDesc.Width = clientWidth;
	depthStencilBufferDesc.Height = clientHeight;
	depthStencilBufferDesc.MipLevels = 1;
	depthStencilBufferDesc.SampleDesc.Count = 1;
	depthStencilBufferDesc.SampleDesc.Quality = 0;
	depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	hr = m_directXSettings.m_device->CreateTexture2D(&depthStencilBufferDesc,
								nullptr, &m_directXSettings.m_depthStencilBuffer);

	if (FAILED(hr))
	{
		return -33;
	}

	hr = m_directXSettings.m_device->CreateDepthStencilView(m_directXSettings.m_depthStencilBuffer,
								nullptr, &m_directXSettings.m_depthStencilView);

	if (FAILED(hr))
	{
		return -34;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = { 0 };
	depthStencilDesc.DepthEnable = true;		// Führe Tiefentests durch
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;	// Näheres Objekt überzeichnet altes // TODO: rumspielen
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.StencilEnable = false;

	hr = m_directXSettings.m_device->CreateDepthStencilState(&depthStencilDesc, &m_directXSettings.m_depthStencilState);
	if (FAILED(hr))
	{
		return -35;
	}

	// Rasterizer
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.AntialiasedLineEnable = false;		// TODO: rumspielen
	rasterDesc.CullMode = D3D11_CULL_BACK;			// backfaces loswerden
	rasterDesc.FillMode = D3D11_FILL_SOLID;			// Objekte komplett anzeigen, TODO: rumspielen
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0;
	rasterDesc.DepthClipEnable = true;				// Zu weit entfernte Objekte werden nicht gerendert
	rasterDesc.FrontCounterClockwise = false;		// Dreiecke sollen IM Uhrzeigersinn sein
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = false;

	hr = m_directXSettings.m_device->CreateRasterizerState(&rasterDesc, &m_directXSettings.m_rasterizerStateSolidFront);
	if (FAILED(hr))
	{
		return -36;
	}

	rasterDesc.CullMode = D3D11_CULL_NONE;
	hr = m_directXSettings.m_device->CreateRasterizerState(&rasterDesc, &m_directXSettings.m_rasterizerStateSolidFull);
	if (FAILED(hr))
	{
		return -36;
	}
	
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = m_directXSettings.m_device->CreateRasterizerState(&rasterDesc, &m_directXSettings.m_rasterizerStateWireframeFull);
	if (FAILED(hr))
	{
		return -36;
	}

	rasterDesc.CullMode = D3D11_CULL_BACK;
	hr = m_directXSettings.m_device->CreateRasterizerState(&rasterDesc, &m_directXSettings.m_rasterizerStateWireframeFront);
	if (FAILED(hr))
	{
		return -36;
	}

	m_directXSettings.m_viewport.Width = static_cast<float>(clientWidth);
	m_directXSettings.m_viewport.Height = static_cast<float>(clientHeight);
	m_directXSettings.m_viewport.TopLeftX = 0;		// TODO: rumspielen
	m_directXSettings.m_viewport.TopLeftY = 0;
	m_directXSettings.m_viewport.MinDepth = 0;
	m_directXSettings.m_viewport.MaxDepth = 1;

	return 0;
}

int CGame::InitConstantBuffers()
{
	D3D11_BUFFER_DESC constantBuffer = { 0 };
	constantBuffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBuffer.ByteWidth = sizeof(SStandardConstantBuffer);
	constantBuffer.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = m_directXSettings.m_device->CreateBuffer(&constantBuffer, nullptr, &m_directXSettings.m_constantBuffers[CB_APPLICATION]);
	FAILHR(-40);

	hr = m_directXSettings.m_device->CreateBuffer(&constantBuffer, nullptr, &m_directXSettings.m_constantBuffers[CB_FRAME]);
	FAILHR(-41);

	hr = m_directXSettings.m_device->CreateBuffer(&constantBuffer, nullptr, &m_directXSettings.m_constantBuffers[CB_OBJECT]);
	FAILHR(-42);

	constantBuffer.ByteWidth = sizeof(SLightConstantBuffer);
	hr = m_directXSettings.m_device->CreateBuffer(&constantBuffer, nullptr, &m_directXSettings.m_constantBuffers[CB_LIGHT]);
	FAILHR(-43);

	constantBuffer.ByteWidth = sizeof(STerrainConstantBuffer);
	hr = m_directXSettings.m_device->CreateBuffer(&constantBuffer, nullptr, &m_directXSettings.m_constantBuffers[CB_TERRAIN]);
	FAILHR(-44);

	hr = m_directXSettings.m_device->CreateBuffer(&constantBuffer, nullptr, &m_directXSettings.m_constantBuffers[CB_VALUE]);
	FAILHR(-43);

	RECT clientRect;
	GetClientRect(m_windowSettings.m_windowHandle, &clientRect);
	float clientHeight = clientRect.bottom - clientRect.top;
	float clientWidth = clientRect.right - clientRect.left;

	// Matrix für ApplicationBuffer berechnen
	m_applicationConstantBuffer.m_matrix = XMMatrixPerspectiveFovLH(
											XMConvertToRadians(60),		// FOV
											clientWidth / clientHeight,	// Aspect Ratio
											0.1f,						// near clipping plane
											100.0f						// far clipping plane
											);

	// In die Grafikkarte schieben
	m_directXSettings.m_deviceContext->UpdateSubresource(m_directXSettings.m_constantBuffers[CB_APPLICATION],
													0, nullptr, &m_applicationConstantBuffer, 0, 0);

	m_camPos = XMFLOAT3(0, 2, -5);
	m_camRot = XMFLOAT3(30, 0, 0);

	return 0;
}

int CGame::CreateSimpleShader()
{
	ID3DBlob* shaderBlob;

#if _DEBUG
	LPCWSTR compiledShaderName = L"SimpleVertexShaderd.cso";
#else
	LPCWSTR compiledShaderName = L"SimpleVertexShader.cso";
#endif

	HRESULT hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -50;
	}

	hr = m_directXSettings.m_device->CreateVertexShader(shaderBlob->GetBufferPointer(),
														shaderBlob->GetBufferSize(),
														nullptr,		// kein Parentshader
														&m_directXSettings.m_simpleVertexShader);
	if (FAILED(hr))
	{
		return -51;
	}

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] = 
	{
		{
			"POSITION",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32_FLOAT,		// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Position),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"COLOR",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32A32_FLOAT,		// Datentyp in shader => float4
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Color),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"NORMAL",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32_FLOAT,		// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Normal),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
	};

	hr = m_directXSettings.m_device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc),
														shaderBlob->GetBufferPointer(),
														shaderBlob->GetBufferSize(),
														&m_directXSettings.m_simpleInputLayout);
	if (FAILED(hr))
	{
		return -52;
	}

	SafeRelease(shaderBlob);

#if _DEBUG
	compiledShaderName = L"SimplePixelShaderd.cso";
#else
	compiledShaderName = L"SimplePixelShader.cso";
#endif

	hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -53;
	}

	hr = m_directXSettings.m_device->CreatePixelShader(shaderBlob->GetBufferPointer(), 
														shaderBlob->GetBufferSize(),
														nullptr, 
														&m_directXSettings.m_simplePixelShader);
	if (FAILED(hr))
	{
		return -54;
	}
	SafeRelease(shaderBlob);


	return 0;
}

int CGame::CreateTexturedShader()
{
	ID3DBlob* shaderBlob;

#if _DEBUG
	LPCWSTR compiledShaderName = L"TexturedVertexShaderd.cso";
#else
	LPCWSTR compiledShaderName = L"TexturedVertexShader.cso";
#endif

	HRESULT hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -55;
	}

	hr = m_directXSettings.m_device->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,		// kein Parentshader
		&m_directXSettings.m_texturedVertexShader);
	if (FAILED(hr))
	{
		return -56;
	}

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{
			"POSITION",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32_FLOAT,		// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Position),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"COLOR",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32A32_FLOAT,		// Datentyp in shader => float4
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Color),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"NORMAL",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32_FLOAT,		// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Normal),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"TEXCOORD",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32_FLOAT,			// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, UV),		// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
	};

	hr = m_directXSettings.m_device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&m_directXSettings.m_texturedInputLayout);
	if (FAILED(hr))
	{
		return -57;
	}

	SafeRelease(shaderBlob);

#if _DEBUG
	compiledShaderName = L"TexturedPixelShaderd.cso";
#else
	compiledShaderName = L"TexturedPixelShader.cso";
#endif

	hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -58;
	}

	hr = m_directXSettings.m_device->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&m_directXSettings.m_texturedPixelShader);
	if (FAILED(hr))
	{
		return -59;
	}
	SafeRelease(shaderBlob);

	return 0;
}

int CGame::CreateOSNMShader()
{
	ID3DBlob* shaderBlob;

#if _DEBUG
	LPCWSTR compiledShaderName = L"OSNMVertexShaderd.cso";
#else
	LPCWSTR compiledShaderName = L"OSNMVertexShader.cso";
#endif

	HRESULT hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -60;
	}

	hr = m_directXSettings.m_device->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,		// kein Parentshader
		&m_directXSettings.m_osnmVertexShader);
	if (FAILED(hr))
	{
		return -61;
	}

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{
			"POSITION",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32_FLOAT,		// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Position),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"COLOR",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32B32A32_FLOAT,		// Datentyp in shader => float4
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, Color),	// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
		{
			"TEXCOORD",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
			DXGI_FORMAT_R32G32_FLOAT,			// Datentyp in shader => float3
			0,									// Falls mehrere VertexShader benutzt werden
			offsetof(VertexPosColor, UV),		// Position des Members im struct
			D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
			0
		},
	};

	hr = m_directXSettings.m_device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&m_directXSettings.m_osnmInputLayout);
	if (FAILED(hr))
	{
		return -62;
	}

	SafeRelease(shaderBlob);

#if _DEBUG
	compiledShaderName = L"OSNMPixelShaderd.cso";
#else
	compiledShaderName = L"OSNMPixelShader.cso";
#endif

	hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -63;
	}

	hr = m_directXSettings.m_device->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&m_directXSettings.m_osnmPixelShader);
	if (FAILED(hr))
	{
		return -64;
	}
	SafeRelease(shaderBlob);

	return 0;
}

int CGame::CreateTerrainShader()
{
	ID3DBlob* shaderBlob;

#if _DEBUG
	LPCWSTR compiledShaderName = L"TerrainVertexShaderd.cso";
#else
	LPCWSTR compiledShaderName = L"TerrainVertexShader.cso";
#endif

	HRESULT hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -65;
	}

	hr = m_directXSettings.m_device->CreateVertexShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,		// kein Parentshader
		&m_directXSettings.m_terrainVertexShader);
	if (FAILED(hr))
	{
		return -66;
	}

	D3D11_INPUT_ELEMENT_DESC vertexLayoutDesc[] =
	{
		{
			"POSITION",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
		DXGI_FORMAT_R32G32B32_FLOAT,		// Datentyp in shader => float3
		0,									// Falls mehrere VertexShader benutzt werden
		offsetof(VertexPosColor, Position),	// Position des Members im struct
		D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
		0
		},
		{
			"COLOR",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
		DXGI_FORMAT_R32G32B32A32_FLOAT,		// Datentyp in shader => float4
		0,									// Falls mehrere VertexShader benutzt werden
		offsetof(VertexPosColor, Color),	// Position des Members im struct
		D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
		0
		},
		{
			"TEXCOORD",							// Semantic Name - Identifikation im Shader
			0,									// Semantic Index, falls mehr als ein gleichheißendes Semantic
		DXGI_FORMAT_R32G32_FLOAT,			// Datentyp in shader => float3
		0,									// Falls mehrere VertexShader benutzt werden
		offsetof(VertexPosColor, UV),		// Position des Members im struct
		D3D11_INPUT_PER_VERTEX_DATA,		// Shader arbeitet Vertex für Vertex
		0
		},
	};

	hr = m_directXSettings.m_device->CreateInputLayout(vertexLayoutDesc, _countof(vertexLayoutDesc),
		shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		&m_directXSettings.m_terrainInputLayout);
	if (FAILED(hr))
	{
		return -67;
	}

	SafeRelease(shaderBlob);

#if _DEBUG
	compiledShaderName = L"TerrainPixelShaderd.cso";
#else
	compiledShaderName = L"TerrainPixelShader.cso";
#endif

	hr = D3DReadFileToBlob(compiledShaderName, &shaderBlob);
	if (FAILED(hr))
	{
		return -68;
	}

	hr = m_directXSettings.m_device->CreatePixelShader(shaderBlob->GetBufferPointer(),
		shaderBlob->GetBufferSize(),
		nullptr,
		&m_directXSettings.m_terrainPixelShader);
	if (FAILED(hr))
	{
		return -69;
	}
	SafeRelease(shaderBlob);

	return 0;
}

void TestButton(CButton2D* _button)
{
	CTM.AddEntity(new COktaeder(XMFLOAT3(0, 5, 0)));
	CTM.RemoveEntity(_button);
}

void CGame::LoadLevel()
{
	m_contentManager.AddEntity(new CTarget(800));
}

void CGame::Update(float _deltaTime)
{
	m_inputManager.DetectInput();

	if (m_inputManager.GetKeyDown(DIK_F1))
	{
		m_directXSettings.m_useWireframe = !m_directXSettings.m_useWireframe;
	}
	if (m_inputManager.GetKeyDown(DIK_F2))
	{
		m_directXSettings.m_useBackfaces = !m_directXSettings.m_useBackfaces;
	}

	CameraMovement(_deltaTime);

	m_contentManager.Update(_deltaTime);

	if (m_inputManager.GetKey(DIK_ESCAPE))
	{
		m_isRunning = false;
	}
}

void CGame::Render()
{
	// Ist das Device noch ok, oder hat hier jemand hotswap betrieben
	assert(m_directXSettings.m_device);
	assert(m_directXSettings.m_deviceContext);

	// Backbuffer clear
	m_directXSettings.m_deviceContext->ClearRenderTargetView(m_directXSettings.m_renderTargetView, Colors::Navy);
	m_directXSettings.m_deviceContext->ClearDepthStencilView(m_directXSettings.m_depthStencilView, 
												D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	XMMATRIX camRotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_camRot.x), 
													XMConvertToRadians(m_camRot.y), 
													XMConvertToRadians(m_camRot.z));
	XMMATRIX camTranslation = XMMatrixTranslation(m_camPos.x, m_camPos.y, m_camPos.z);

	// Cameraspace verhält sich wie ein Objektspace
	m_frameConstantBuffer.m_matrix = XMMatrixInverse(nullptr, camRotation * camTranslation);

	m_directXSettings.m_deviceContext->UpdateSubresource(m_directXSettings.m_constantBuffers[CB_FRAME],
													0, nullptr, &m_frameConstantBuffer, 0, 0);

	m_lightConstantBuffer.m_ambientColor = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	m_lightConstantBuffer.m_diffuseColor = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	m_lightConstantBuffer.m_lightDir = XMFLOAT3(1.0f, -1.0f, 1.0f) * -1;
	m_lightConstantBuffer.m_cameraPos = m_camPos;
	m_lightConstantBuffer.m_specularColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	m_directXSettings.m_deviceContext->UpdateSubresource(m_directXSettings.m_constantBuffers[CB_LIGHT],
													0, nullptr, &m_lightConstantBuffer, 0, 0);

	m_contentManager.Render();

	// Backbuffer Swap 'Umdrehen der Tafel'
	m_directXSettings.m_swapChain->Present(1, 0);
}

void CGame::CameraMovement(float _deltaTime)
{
	XMVECTOR f = { 0, 0, 1, 0 };
	XMVECTOR r = { 1, 0, 0, 0 };
	XMVECTOR u = { 0, 1, 0, 0 };

	XMMATRIX m = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_camRot.x), XMConvertToRadians(m_camRot.y), XMConvertToRadians(m_camRot.z));
	f = XMVector3Transform(f, m);
	r = XMVector3Transform(r, m);
	u = XMVector3Transform(u, m);

	XMFLOAT3 forward = XMFLOAT3(f.m128_f32[0], f.m128_f32[1], f.m128_f32[2]);
	XMFLOAT3 right = XMFLOAT3(r.m128_f32[0], r.m128_f32[1], r.m128_f32[2]);
	XMFLOAT3 up = XMFLOAT3(u.m128_f32[0], u.m128_f32[1], u.m128_f32[2]);

	if (m_inputManager.GetKey(DIK_W))
	{
		m_camPos = m_camPos + (forward * _deltaTime * 5.0f);
	}

	if (m_inputManager.GetKey(DIK_A))
	{
		m_camPos = m_camPos - (right* _deltaTime * 5.0f);
	}

	if (m_inputManager.GetKey(DIK_S))
	{
		m_camPos = m_camPos - (forward * _deltaTime * 5.0f);
	}

	if (m_inputManager.GetKey(DIK_D))
	{
		m_camPos = m_camPos + (right * _deltaTime * 5.0f);
	}

	if (m_inputManager.GetKey(DIK_Q))
	{
		m_camPos = m_camPos + (up * _deltaTime * 5.0f);
	}

	if (m_inputManager.GetKey(DIK_E))
	{
		m_camPos = m_camPos - (up * _deltaTime * 5.0f);
	}

	// Rechte maustaste gedrückt
	if (m_inputManager.GetMouseKey(1))
	{
		XMFLOAT2 mouseDelta = m_inputManager.GetMouseMovement();
		m_camRot.x += mouseDelta.y * _deltaTime * 10;
		m_camRot.y += mouseDelta.x * _deltaTime * 10;
	}
}



LRESULT CALLBACK WndProc(HWND _hwnd, UINT _message, WPARAM _wparam, LPARAM _lparam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (_message)
	{
	case WM_PAINT:
		hDC = BeginPaint(_hwnd, &ps);
		EndPaint(_hwnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hwnd, _message, _wparam, _lparam);
	}

	return 0;
}
