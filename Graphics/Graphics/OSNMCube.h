#pragma once
#include "OSNMEntity.h"
class COSNMCube :
	public COSNMEntity
{
public:
	COSNMCube(LPCWSTR _diffuseFileName, LPCWSTR _normalFileName, XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~COSNMCube();

	virtual void Update(float _deltaTime) override;
};

