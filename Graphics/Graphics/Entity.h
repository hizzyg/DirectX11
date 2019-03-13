#pragma once
#include "VertexPosColor.h"
#include "ConstantBuffer.h"

class CEntity
{
public:
	CEntity(XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f));
	virtual ~CEntity();

	virtual bool Initialize();
	virtual void Update(float _deltaTime);
	virtual void Render();

	// Der Datentyp XMFLOAT3 muss alligned (speicherausrichtung) 16 sein aus performancegründen
	static void* operator new(size_t _size)
	{
		return _aligned_malloc(_size, 16);
	}

	static void operator delete(void* _memory)
	{
		_aligned_free(_memory);
	}

	XMFLOAT3 m_position;
	XMFLOAT3 m_rotation;
	XMFLOAT3 m_scale;

protected:
	VertexPosColor* m_vertices;
	WORD* m_indices;
	int m_vertexCount;
	int m_indexCount;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	SStandardConstantBuffer m_objectConstantBuffer;
};

