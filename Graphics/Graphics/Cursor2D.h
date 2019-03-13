#pragma once
#include "Image2D.h"
class CCursor2D :
	public CImage2D
{
public:
	CCursor2D(LPCWSTR _fileName);
	virtual ~CCursor2D();

	virtual void Update(float _deltaTime) override;

};

