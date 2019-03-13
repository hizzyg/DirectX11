#include "PCH.h"
#include "Image2D.h"
#include "Game.h"



CImage2D::CImage2D(LPCWSTR _fileName, XMFLOAT2 _pos)
	: CEntity2D(_pos)
{
	//ID3D11Resource* texture;
	//ID3D11ShaderResourceView* srv;

	//CreateWICTextureFromFile(DXS.m_device, _fileName, &texture, &srv);

	m_textureData = *ASM.LoadTexture(_fileName);
	ID3D11Texture2D* tex2D = (ID3D11Texture2D*) m_textureData.m_texture;

	D3D11_TEXTURE2D_DESC tdesc;
	tex2D->GetDesc(&tdesc);

	m_size.x = tdesc.Width;
	m_size.y = tdesc.Height;

	m_sourceRect.left = 0;
	m_sourceRect.top = 0;
	m_sourceRect.right = m_size.x;
	m_sourceRect.bottom = m_size.y;

}

CImage2D::~CImage2D()
{
	delete m_textureData.m_texture;
	delete m_textureData.m_shaderRessourceView;
}

void CImage2D::Render()
{
	CTM.m_spriteBatch->Draw(m_textureData.m_shaderRessourceView, m_position, 
		&m_sourceRect, Colors::White, XMConvertToRadians(m_rotation));
}
