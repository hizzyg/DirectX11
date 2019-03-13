#include "PCH.h"
#include "OSNMCube.h"
#include "Game.h"


COSNMCube::COSNMCube(LPCWSTR _diffuseFileName, LPCWSTR _normalFileName, XMFLOAT3 _pos)
	: COSNMEntity(_diffuseFileName, _normalFileName, _pos)
{
	m_vertexCount = 4 * 6;
	m_indexCount = 6 * 6;

	m_vertices = new VertexPosColor[m_vertexCount];
	m_indices = new WORD[m_indexCount];

	XMFLOAT4 col = XMFLOAT4(1, 1, 1, 1);

	int i = 0;
	// vorne
	m_vertices[i++] = { XMFLOAT3(-0.5f, 0.5f, -0.5), col, XMFLOAT3(0,0,-1), XMFLOAT2(0,0.5f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, 0.5f, -0.5), col, XMFLOAT3(0,0,-1), XMFLOAT2(0.5f,0.5f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, -0.5f, -0.5), col, XMFLOAT3(0,0,-1), XMFLOAT2(0.5f,1) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, -0.5f, -0.5), col, XMFLOAT3(0,0,-1), XMFLOAT2(0,1) };

	// hinten
	m_vertices[i++] = { XMFLOAT3(0.5f, 0.5f, 0.5), col, XMFLOAT3(0,0,1), XMFLOAT2(0, 0.0f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, 0.5f, 0.5), col, XMFLOAT3(0,0,1), XMFLOAT2(0.25f, 0.0f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, -0.5f, 0.5), col, XMFLOAT3(0,0,1), XMFLOAT2(0.25f, 0.25f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, -0.5f, 0.5), col, XMFLOAT3(0,0,1), XMFLOAT2(0.0f, 0.25f) };

	// links
	m_vertices[i++] = { XMFLOAT3(-0.5f, 0.5f, 0.5), col, XMFLOAT3(-1,0,0), XMFLOAT2(0.0f, 0.5f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, 0.5f, -0.5), col, XMFLOAT3(-1,0,0), XMFLOAT2(0.0f, 0.25f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, -0.5f, -0.5), col, XMFLOAT3(-1,0,0), XMFLOAT2(0.25f, 0.25f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, -0.5f, 0.5), col, XMFLOAT3(-1,0,0), XMFLOAT2(0.25f, 0.5f) };

	// rechts
	m_vertices[i++] = { XMFLOAT3(0.5f, 0.5f, -0.5), col, XMFLOAT3(1,0,0), XMFLOAT2(0.25f, 0.0f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, 0.5f, 0.5), col, XMFLOAT3(1,0,0), XMFLOAT2(0.5f, 0.0f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, -0.5f, 0.5), col, XMFLOAT3(1,0,0), XMFLOAT2(0.5f, 0.5f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, -0.5f, -0.5), col, XMFLOAT3(1,0,0), XMFLOAT2(0.25f, 0.5f) };

	// oben
	m_vertices[i++] = { XMFLOAT3(-0.5f, 0.5f, -0.5), col, XMFLOAT3(0,1,0), XMFLOAT2(0.5f, 0.0f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, 0.5f, 0.5), col, XMFLOAT3(0,1,0), XMFLOAT2(1.0f, 0.0f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, 0.5f, 0.5), col, XMFLOAT3(0,1,0), XMFLOAT2(1.0f, 0.5f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, 0.5f, -0.5), col, XMFLOAT3(0,1,0), XMFLOAT2(0.5f, 0.5f) };

	// unten
	m_vertices[i++] = { XMFLOAT3(-0.5f, -0.5f, -0.5), col, XMFLOAT3(0,-1,0), XMFLOAT2(0.5f, 0.5f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, -0.5f, -0.5), col, XMFLOAT3(0,-1,0), XMFLOAT2(1.0f, 0.5f) };
	m_vertices[i++] = { XMFLOAT3(0.5f, -0.5f, 0.5), col, XMFLOAT3(0,-1,0), XMFLOAT2(1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(-0.5f, -0.5f, 0.5), col, XMFLOAT3(0,-1,0), XMFLOAT2(0.5f, 1.0f) };

	i = 0;
	for (int side = 0; side < 6; side++)
	{
		m_indices[i++] = 4 * side;
		m_indices[i++] = 4 * side + 1;
		m_indices[i++] = 4 * side + 3;

		m_indices[i++] = 4 * side + 1;
		m_indices[i++] = 4 * side + 2;
		m_indices[i++] = 4 * side + 3;
	}
}

COSNMCube::~COSNMCube()
{
}

void COSNMCube::Update(float _deltaTime)
{
	if (IPM.GetKey(DIK_LEFTARROW))
	{
		m_rotation.y -= 60 * _deltaTime;
	}
	if (IPM.GetKey(DIK_RIGHTARROW))
	{
		m_rotation.y += 60 * _deltaTime;
	}
	if (IPM.GetKey(DIK_UPARROW))
	{
		m_rotation.x -= 60 * _deltaTime;
	}
	if (IPM.GetKey(DIK_DOWNARROW))
	{
		m_rotation.x += 60 * _deltaTime;
	}
}
