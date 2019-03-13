#pragma once
#include "Entity2D.h"
class CText2D :
	public CEntity2D
{
public:
	CText2D(LPCWSTR _text, XMFLOAT2 _pos = XMFLOAT2(0,0));
	virtual ~CText2D();

	virtual void Render() override;

private:
	LPCWSTR m_text;
};

