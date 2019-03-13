#include "PCH.h"
#include "Cursor2D.h"
#include "Game.h"
#include "Helper.h"


CCursor2D::CCursor2D(LPCWSTR _fileName)
	: CImage2D(_fileName)
{
}


CCursor2D::~CCursor2D()
{
}

void CCursor2D::Update(float _deltaTime)
{
	m_position = IPM.GetMousePos() - m_size * 0.5f;
}
