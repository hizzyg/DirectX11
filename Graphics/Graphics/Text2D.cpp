#include "PCH.h"
#include "Text2D.h"
#include "Game.h"

CText2D::CText2D(LPCWSTR _text, XMFLOAT2 _pos)
	: CEntity2D(_pos)
{
	m_text = _text;
}

CText2D::~CText2D()
{
}

void CText2D::Render()
{
	CTM.m_spriteFont->DrawString(CTM.m_spriteBatch, m_text, m_position, Colors::White, m_rotation);
}
