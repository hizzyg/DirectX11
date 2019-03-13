#pragma once
#include "Entity.h"
#include "TextureData.h"

class CTerrain :
	public CEntity
{
public:
	CTerrain(LPCWSTR _heightMap, LPCWSTR _textures[5], int _length, int _width, float _height, XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~CTerrain();

	virtual void Render() override;

protected:
	STextureData* m_heightMapData;
	STextureData* m_textures[5];

	STerrainConstantBuffer m_terrainConstantBuffer;

	int m_length;
	int m_width;
	float m_height;
};

