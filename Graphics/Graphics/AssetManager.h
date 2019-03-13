#pragma once
#include "TextureData.h"
#include <map>

class CAssetManager
{
public:
	CAssetManager();
	~CAssetManager();

	STextureData* LoadTexture(LPCWSTR _fileName);

private:
	std::map<LPCWSTR, STextureData*> m_textures;
};

