#include "PCH.h"
#include "Oktaeder.h"
#include "Game.h"

COktaeder::COktaeder(XMFLOAT3 _pos)
	:CEntity(_pos)
{
	m_vertexCount = 24;
	m_indexCount = 24; // 8 * 3

	// C-Style
	m_vertices = (VertexPosColor*) malloc(m_vertexCount * sizeof(VertexPosColor));
	// C++-Style
	m_indices = new WORD[m_indexCount];

	int i = 0;
	m_vertices[i++] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(-1.0f, 1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f,0.5f,0,1), XMFLOAT3(-1.0f, 1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1,0,0,1), XMFLOAT3(-1.0f, 1.0f, -1.0f) };

	m_vertices[i++] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(-1.0f, 1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1,0,0,1), XMFLOAT3(-1.0f, 1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0,1,0,1), XMFLOAT3(-1.0f, 1.0f, 1.0f) };

	m_vertices[i++] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(1.0f, 1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0,1,0,1), XMFLOAT3(1.0f, 1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.5f,0.5f,1,1), XMFLOAT3(1.0f, 1.0f, 1.0f) };

	m_vertices[i++] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(1.0f, 1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.5f,0.5f,1,1), XMFLOAT3(1.0f, 1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f,0.5f,0,1), XMFLOAT3(1.0f, 1.0f, -1.0f) };


	m_vertices[i++] = { XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(-1.0f, -1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1,0,0,1), XMFLOAT3(-1.0f, -1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f,0.5f,0,1), XMFLOAT3(-1.0f, -1.0f, -1.0f) };

	m_vertices[i++] = { XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(-1.0f, -1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0,1,0,1), XMFLOAT3(-1.0f, -1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1,0,0,1), XMFLOAT3(-1.0f, -1.0f, 1.0f) };

	m_vertices[i++] = { XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(1.0f, -1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.5f,0.5f,1,1), XMFLOAT3(1.0f, -1.0f, 1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0,1,0,1), XMFLOAT3(1.0f, -1.0f, 1.0f) };

	m_vertices[i++] = { XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(1.0f, -1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f,0.5f,0,1), XMFLOAT3(1.0f, -1.0f, -1.0f) };
	m_vertices[i++] = { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.5f,0.5f,1,1), XMFLOAT3(1.0f, -1.0f, -1.0f) };


	//m_vertices[i++] = { XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(0,0,1,1), XMFLOAT3(0.0f, 1.0f, 0.0f) };
	//m_vertices[i++] = { XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1,0,0,1), XMFLOAT3(-1.0f, 0.0f, 0.0f) };
	//m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT4(0,1,0,1), XMFLOAT3(0.0f, 0.0f, 1.0f) };
	//m_vertices[i++] = { XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(0.5f,0.5f,1,1), XMFLOAT3(1.0f, 0.0f, 0.0f) };
	//m_vertices[i++] = { XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT4(1.0f,0.5f,0,1), XMFLOAT3(0.0f, 0.0f, -1.0f) };
	//m_vertices[i++] = { XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1.0f,1.0f,0.0f,1), XMFLOAT3(0.0f, -1.0f, 0.0f) };

	for (i = 0; i < m_vertexCount; )
	{
		m_indices[i++] = i;
	}
}

COktaeder::~COktaeder()
{
	free(m_vertices);
	delete m_indices;
}

void COktaeder::Update(float _deltaTime)
{
	m_timer += _deltaTime;

	m_rotation.x += (_deltaTime * 30);
	m_rotation.y += (_deltaTime * 50);

	if (IPM.GetKey(DIK_LEFTARROW))
	{
		m_position.x -= _deltaTime * 2.0f;
	}
	if (IPM.GetKey(DIK_RIGHTARROW))
	{
		m_position.x += _deltaTime * 2.0f;
	}

	//m_scale = XMFLOAT3(sin(m_timer), sin(m_timer), sin(m_timer));
}
