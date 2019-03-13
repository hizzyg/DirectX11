#include "PCH.h"
#include "AssetManager.h"
#include "Game.h"


CAssetManager::CAssetManager()
{
}


CAssetManager::~CAssetManager()
{
}

STextureData * CAssetManager::LoadTexture(LPCWSTR _fileName)
{
	if (_fileName == nullptr)
	{
		return nullptr;
	}

	// Es wurde nicht gefunden, neue Texture muss hinzugefügt werden
	if (m_textures.find(_fileName) == m_textures.end())
	{
		ID3D11Resource* t;
		ID3D11ShaderResourceView* srv;
		ID3D11SamplerState* ts;

		CreateWICTextureFromFile(DXS.m_device, _fileName, &t, &srv);
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));

		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;	// TODO: rumspielen
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		//desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		DXS.m_device->CreateSamplerState(&desc, &ts);

		m_textures[_fileName] = new STextureData(t, srv, ts);
	}
	
	return m_textures[_fileName];
}
