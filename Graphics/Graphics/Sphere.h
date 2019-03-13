#pragma once
#include "Entity.h"
class CSphere :
	public CEntity
{
public:
	CSphere(XMFLOAT3 _pos, int _slices, int _rows);
	virtual ~CSphere();
};

