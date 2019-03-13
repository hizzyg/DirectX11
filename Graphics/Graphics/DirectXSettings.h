#pragma once
#include "ConstantBuffer.h"
enum CB_Type
{
	CB_APPLICATION,		// Für Daten die sich nie ändern, zb CameraToScreen-Matrix
	CB_FRAME,			// Für Daten die sich einmal pro Frame ändern, zb WorldToCamera-Matrix
						// Für Daten die sich in jedem Objekt in jedem Frame ändern, zb ObjectToWorld-Matrix
						CB_OBJECT,
						CB_LIGHT,
						CB_TERRAIN,
						CB_VALUE,
						NumConstantBuffers
};

struct SDirectXSettings
{
	// Device Swap Chain (Tafeldrehvorrichtung)
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;
	IDXGISwapChain* m_swapChain = nullptr;

	// Render Target View des Backbuffers (Rückseite der Tafel)
	ID3D11RenderTargetView* m_renderTargetView = nullptr;

	// Depth Stencil (Tiefenordnung für Pixel, welcher Pixel ist zu sehen und verdeckt welchen)
	ID3D11Texture2D* m_depthStencilBuffer = nullptr;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
	ID3D11DepthStencilState* m_depthStencilState = nullptr;	// Konfiguration für Depthtest
	ID3D11RasterizerState* m_rasterizerStateSolidFront = nullptr;
	ID3D11RasterizerState* m_rasterizerStateSolidFull = nullptr;
	ID3D11RasterizerState* m_rasterizerStateWireframeFront = nullptr;
	ID3D11RasterizerState* m_rasterizerStateWireframeFull = nullptr;

	SStandardConstantBuffer m_objectConstantBuffer;
	bool m_useWireframe;
	bool m_useBackfaces;
	ID3D11BlendState* m_blendstate = nullptr;
	ID3D11BlendState* m_blendstateTransparent = nullptr;
	inline ID3D11RasterizerState* GetCurrentRasterizer()
	{
		if (m_useWireframe)
		{
			if (m_useBackfaces)
			{
				return m_rasterizerStateWireframeFull;
			}
			else
			{
				return m_rasterizerStateWireframeFront;
			}
		}
		else
		{
			if (m_useBackfaces)
			{
				return m_rasterizerStateSolidFull;
			}
			else
			{
				return m_rasterizerStateSolidFront;
			}
		}
	}
	inline ID3D11BlendState* GetCurrentBlendState() { return m_transparentAlpha ? m_blendstateTransparent : m_blendstate; }

	bool m_xrayMode = false;
	bool m_transparentAlpha = true;

	float m_blendFactor[5] = { 0.25f, 0.25f, 0.25f, 0.25f,0.25f };
	UINT m_sampleMask = 0xffffffff;

	// Viewport
	D3D11_VIEWPORT m_viewport;

	ID3D11Buffer* m_constantBuffers[NumConstantBuffers];

	ID3D11InputLayout* m_simpleInputLayout;
	ID3D11VertexShader* m_simpleVertexShader;
	ID3D11PixelShader* m_simplePixelShader;

	ID3D11InputLayout* m_texturedInputLayout;
	ID3D11VertexShader* m_texturedVertexShader;
	ID3D11PixelShader* m_texturedPixelShader;

	ID3D11InputLayout* m_osnmInputLayout;
	ID3D11VertexShader* m_osnmVertexShader;
	ID3D11PixelShader* m_osnmPixelShader;

	ID3D11InputLayout* m_terrainInputLayout;
	ID3D11VertexShader* m_terrainVertexShader;
	ID3D11PixelShader* m_terrainPixelShader;
};