#include "PCH.h"
#include "Button2D.h"
#include "Helper.h"
#include "Game.h"


CButton2D::CButton2D(LPCWSTR _fileName, BTNCALLBACK _callback, XMFLOAT2 _pos)
	: CImage2D(_fileName, _pos)
{
	m_size = m_size * 0.5f;
	m_callback = _callback;
}

CButton2D::~CButton2D()
{
}

void CButton2D::Update(float _deltaTime)
{
	if (PointInRect(m_position, m_size, IPM.GetMousePos()))
	{
		if (IPM.GetMouseKey(0))
		{
			m_sourceRect = { 0, (LONG) m_size.y, (LONG) m_size.x, (LONG) (m_size.y * 2)};
		}
		else
		{
			if (m_callback && IPM.GetMouseKeyUp(0))
			{
				m_callback(this);
			}
			m_sourceRect = { (LONG) m_size.x, 0, (LONG) (m_size.x * 2), (LONG) m_size.y };
		}
	}
	else
	{
		m_sourceRect = { 0, 0, (LONG) m_size.x, (LONG) m_size.y };
	}
}
