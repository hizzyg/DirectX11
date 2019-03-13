#pragma once

struct SStandardConstantBuffer
{
	XMMATRIX m_matrix;
};

struct SLightConstantBuffer
{
	XMFLOAT4 m_ambientColor;
	XMFLOAT4 m_diffuseColor;
	XMFLOAT3 m_lightDir;
	float m_dirPadding;
	XMFLOAT4 m_specularColor;
	XMFLOAT3 m_cameraPos;
	float m_cameraPadding;
};

struct STerrainConstantBuffer
{
	float m_height;
	XMFLOAT3 m_terrainPadding;
};