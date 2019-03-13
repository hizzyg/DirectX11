#pragma once
#include "Entity.h"

class COktaeder :
	public CEntity
{
public:
	COktaeder(XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~COktaeder();

	virtual void Update(float _deltaTime) override;

	float m_timer = 0;
};

