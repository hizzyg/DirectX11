#pragma once

struct STextureData
{
	ID3D11Resource* m_texture;
	ID3D11ShaderResourceView* m_shaderRessourceView;
	ID3D11SamplerState* m_textureSampler;

	STextureData();
	STextureData(ID3D11Resource* _texture, ID3D11ShaderResourceView* _shaderRessourceView, ID3D11SamplerState* _sampler);
};
