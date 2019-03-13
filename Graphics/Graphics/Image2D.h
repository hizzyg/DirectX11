#pragma once
#include "Entity2D.h"
#include "TextureData.h"

class CImage2D :
	public CEntity2D
{
public:
	CImage2D(LPCWSTR _fileName, XMFLOAT2 _pos = XMFLOAT2(0, 0));
	virtual ~CImage2D();

	virtual void Render() override;

protected:
	STextureData m_textureData;
	RECT m_sourceRect;
};

