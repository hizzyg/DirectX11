#include "PCH.h"
#include "Terrain.h"
#include "Game.h"

CTerrain::CTerrain(LPCWSTR _heightMap, LPCWSTR _textures[5], int _length, int _width, float _height, XMFLOAT3 _pos)
	: CEntity(_pos)
{
	m_heightMapData = ASM.LoadTexture(_heightMap);
	int i;
	for (i = 0; i < 5; i++)
	{
		m_textures[i] = ASM.LoadTexture(_textures[i]);
	}

	m_length = _length;
	m_width = _width;
	m_height = _height;

	m_vertexCount = (_width + 1) * (_length + 1);
	m_indexCount = _width * _length * 6;

	m_vertices = new VertexPosColor[m_vertexCount];
	m_indices = new WORD[m_indexCount];

	XMFLOAT4 col = XMFLOAT4(1, 1, 1, 1);
	XMFLOAT3 norm = XMFLOAT3(0, 0, 0);

	i = 0;
	for (float x = 0; x <= _width; x++)
	{
		for (float z = 0; z <= _length; z++)
		{
			m_vertices[i++] = { XMFLOAT3(x, 0, z), col , norm, XMFLOAT2(x / _width, z / _length) };
		}
	}

	i = 0;
	for (int x = 0; x < _width; x++)
	{
		for (int z = 0; z < _width; z++)
		{
			m_indices[i++] = z * (_width + 1) + x;				// 0
			m_indices[i++] = z * (_width + 1) + x + 1;			// 1
			m_indices[i++] = (z + 1) * (_width + 1) + x + 1;	// 5

			m_indices[i++] = z * (_width + 1) + x;				// 0
			m_indices[i++] = (z + 1) * (_width + 1) + x + 1;	// 5
			m_indices[i++] = (z + 1) * (_width + 1) + x;		// 4
		}
	}
}

CTerrain::~CTerrain()
{
}

void CTerrain::Render()
{
	// prepare matrix
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotation.x),
		XMConvertToRadians(m_rotation.y),
		XMConvertToRadians(m_rotation.z));
	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	m_objectConstantBuffer.m_matrix = scale * rotation * translation;

	DXS.m_deviceContext->UpdateSubresource(DXS.m_constantBuffers[CB_OBJECT],
		0, nullptr, &m_objectConstantBuffer, 0, 0);

	m_terrainConstantBuffer.m_height = m_height;
	DXS.m_deviceContext->UpdateSubresource(DXS.m_constantBuffers[CB_TERRAIN],
		0, nullptr, &m_terrainConstantBuffer, 0, 0);


	unsigned int offset = 0;
	unsigned int stride = sizeof(VertexPosColor);

	// Input Assembler
	DXS.m_deviceContext->IASetInputLayout(DXS.m_terrainInputLayout);
	DXS.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DXS.m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	DXS.m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, offset);

	// Vertex Shader
	DXS.m_deviceContext->VSSetConstantBuffers(0, 3, DXS.m_constantBuffers);
	DXS.m_deviceContext->VSSetConstantBuffers(3, 1, &DXS.m_constantBuffers[CB_TERRAIN]);
	DXS.m_deviceContext->VSSetShader(DXS.m_terrainVertexShader, nullptr, 0);
	DXS.m_deviceContext->VSSetShaderResources(0, 1, &m_heightMapData->m_shaderRessourceView);
	DXS.m_deviceContext->VSSetSamplers(0, 1, &m_heightMapData->m_textureSampler);

	// Rasterizer
	DXS.m_deviceContext->RSSetState(DXS.GetCurrentRasterizer());
	DXS.m_deviceContext->RSSetViewports(1, &DXS.m_viewport);

	// Pixel Shader
	DXS.m_deviceContext->PSSetConstantBuffers(0, 1, &DXS.m_constantBuffers[CB_LIGHT]);
	DXS.m_deviceContext->PSSetConstantBuffers(1, 1, &DXS.m_constantBuffers[CB_OBJECT]);
	DXS.m_deviceContext->PSSetShader(DXS.m_terrainPixelShader, nullptr, 0);
	DXS.m_deviceContext->PSSetShaderResources(0, 1, &m_heightMapData->m_shaderRessourceView);
	DXS.m_deviceContext->PSSetSamplers(0, 1, &m_heightMapData->m_textureSampler);
	for (int i = 0; i < 5; i++)
	{
		DXS.m_deviceContext->PSSetShaderResources(1 + i, 1, &m_textures[i]->m_shaderRessourceView);
		DXS.m_deviceContext->PSSetSamplers(1 + i, 1, &m_textures[i]->m_textureSampler);
	}

	// Outputmerger
	DXS.m_deviceContext->OMSetRenderTargets(1, &DXS.m_renderTargetView, DXS.m_depthStencilView);
	DXS.m_deviceContext->OMSetDepthStencilState(DXS.m_depthStencilState, 1);

	// Objekt zeichnen
	DXS.m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
}
