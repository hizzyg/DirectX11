#pragma once
#include "TexturedEntity.h"
class CTexturedCube :
	public CTexturedEntity
{
public:
	CTexturedCube(LPCWSTR _fileName, XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~CTexturedCube();

	virtual void Update(float _deltaTime) override;
};

