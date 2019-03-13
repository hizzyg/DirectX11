#include "PCH.h"
#include "TextureData.h"

STextureData::STextureData()
{
	m_texture = nullptr;
	m_shaderRessourceView = nullptr;
	m_textureSampler = nullptr;
}

STextureData::STextureData(ID3D11Resource * _texture, ID3D11ShaderResourceView * _shaderRessourceView, ID3D11SamplerState * _sampler)
{
	m_texture = _texture;
	m_shaderRessourceView = _shaderRessourceView;
	m_textureSampler = _sampler;
}
