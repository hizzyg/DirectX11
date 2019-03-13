#pragma once
#include "TexturedEntity.h"

// Object Space Normal Map
// Normal Map wird im Object space dargestellt, dadurch ist die Berechnung einfacher 
// als im Tangent Space (wir müssen keine Umrechnung vornehmen)
class COSNMEntity :
	public CTexturedEntity
{
public:
	COSNMEntity(LPCWSTR _diffuseFileName, LPCWSTR _normalFileName, XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~COSNMEntity();

	virtual void Render() override;
protected:
	STextureData* m_normalTextureData;
};

