#pragma once
#include "Entity.h"
#include "Game.h"
class CTarget :
	public CEntity
{
public:
	CTarget(float _sliceCount, float _ySize = 0.25f, float _radius = 5.0f, XMFLOAT3 _pos = {0,0,0});
	virtual ~CTarget();
	virtual void Render() override;
	virtual void Update(float _deltaTime) override;
	float m_timer = 0;
};