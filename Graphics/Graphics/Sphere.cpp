#include"PCH.h"
#include "Sphere.h"
#include "Helper.h"

CSphere::CSphere(XMFLOAT3 _pos, int _slices, int _rows)
	: CEntity(_pos)
{
	if (_slices < 3)
	{
		_slices = 3;
	}
	if (_rows < 1)
	{
		_rows = 1;
	}

	m_vertexCount = (1 + _slices * _rows) * 2 - _slices;
	m_indexCount = 3 * 2 * _slices + 2 * 3 * 2 * _slices * (_rows - 1) +3; // 2 * (_slices + (_rows - 1) * _slices) * 3;

	m_vertices = new VertexPosColor[m_vertexCount];
	m_indices = new WORD[m_indexCount];

	// oben
	// pol
	int i = 0;
	float yValue;
	float yPosCos;
	float yPosSin;
	XMFLOAT3 pos;
	XMFLOAT4 topCol = XMFLOAT4(0, 1, 0, 1);
	XMFLOAT4 bottomCol = XMFLOAT4(1, 1, 1, 1) - topCol;
	bottomCol.w = 1;
	m_vertices[i++] = { XMFLOAT3(0, 1, 0), topCol, XMFLOAT3(0,1,0) };
	float tmp;
	for (float row = 1; row <= _rows; row++)
	{
		yValue = row / _rows * XM_PIDIV2;
		yPosCos = cosf(yValue);
		yPosSin = sinf(yValue);
		for (float slice = 0; slice < _slices; slice++)
		{
			tmp = slice / _slices * XM_2PI;
			pos = XMFLOAT3(sinf(tmp) * yPosSin, yPosCos, cosf(tmp) * yPosSin);
			m_vertices[i++] = { pos, Lerp(bottomCol, topCol, (yPosCos + 1) * 0.5f), pos };
		}
	}

	// unten
	for (float row = 1; row < _rows; row++)
	{
		yValue = (row + _rows) / _rows * XM_PIDIV2;
		yPosCos = cosf(yValue);
		yPosSin = sinf(yValue);
		for (float slice = 0; slice < _slices; slice++)
		{
			tmp = (slice) / _slices * XM_2PI;
			pos = XMFLOAT3(sinf(tmp) * yPosSin, yPosCos, cosf(tmp) * yPosSin);
			m_vertices[i++] = { pos, Lerp(bottomCol, topCol, (yPosCos + 1) * 0.5f), pos };
		}
	}

	m_vertices[i++] = { XMFLOAT3(0, -1, 0), bottomCol, XMFLOAT3(0,-1,0) };

	i = 0;
	// Obere Polregion
	for (int slice = 0; slice < _slices - 1; slice++)
	{
		m_indices[i++] = 0;
		m_indices[i++] = slice + 1;
		m_indices[i++] = slice + 2;
	}
	m_indices[i++] = 0;
	m_indices[i++] = _slices;
	m_indices[i++] = 1;

	// Oberer Mantel

	for (int row = 0; row < _rows - 1; row++)
	{
		for (int slice = 0; slice < _slices - 1; slice++)
		{
			m_indices[i++] = row * _slices + slice + 1;
			m_indices[i++] = (row + 1) * _slices + slice + 1;
			m_indices[i++] = row  *_slices + slice + 2;
	
			m_indices[i++] = row *_slices + slice + 2;
			m_indices[i++] = (row + 1) *_slices + slice + 1;
			m_indices[i++] = (row + 1) *_slices + slice + 2;
		}
	
		m_indices[i++] = row * _slices + 0 + 1;
		m_indices[i++] = row * _slices + _slices;
		m_indices[i++] = row * _slices + _slices * 2;
	
		m_indices[i++] = row * _slices + 0 + 1;
		m_indices[i++] = row * _slices + _slices * 2;
		m_indices[i++] = row * _slices + _slices + 1;
	}

	int offset = _rows * _slices - _slices;
	// unterer Mantel
	for (int row = 0; row < _rows - 1; row++)
	{
		for (int slice = 0; slice < _slices - 1; slice++)
		{
			m_indices[i++] = row * _slices + slice + 1 + offset;
			m_indices[i++] = (row + 1) * _slices + slice + 1 + offset;
			m_indices[i++] = row  *_slices + slice + 2 + offset;
	
			m_indices[i++] = row *_slices + slice + 2 + offset;
			m_indices[i++] = (row + 1) *_slices + slice + 1 + offset;
			m_indices[i++] = (row + 1) *_slices + slice + 2 + offset;
		}
	
		m_indices[i++] = row * _slices + 0 + 1 + offset;
		m_indices[i++] = row * _slices + _slices + offset;
		m_indices[i++] = row * _slices + _slices * 2 + offset;
	
		m_indices[i++] = row * _slices + 0 + 1 + offset;
		m_indices[i++] = row * _slices + _slices * 2 + offset;
		m_indices[i++] = row * _slices + _slices + 1 + offset;
	}

	// untere Polregion
	for (int slice = 0; slice < _slices; slice++)
	{
		m_indices[i++] = m_vertexCount - 1;
		m_indices[i++] = m_vertexCount - 1 - slice;
		m_indices[i++] = m_vertexCount - 2 - slice;
	}
	
	m_indices[i++] = m_vertexCount - 1;
	m_indices[i++] = m_vertexCount - _slices - 1;
	m_indices[i++] = m_vertexCount - 2;
}

CSphere::~CSphere()
{
}
