#include "PCH.h"
#include "Target.h"
#include "Game.h"

CTarget::CTarget(float _sliceCount, float _ySize, float _radius, XMFLOAT3 _pos)
	:CEntity(_pos)
{
	double degree = 360.0f;
	int word = 0;
	if (_sliceCount < 8)
	{
		_sliceCount = 8;
		degree = degree / _sliceCount;
	}
	degree = degree / _sliceCount;

	m_vertexCount = 4 * _sliceCount + 2;
	m_indexCount = 4 * _sliceCount * 3;

	m_vertices = (VertexPosColor*)malloc(m_vertexCount * sizeof(VertexPosColor));
	m_indices = new WORD[m_indexCount];

	XMFLOAT3 pos;
	XMFLOAT3 posY;
	XMFLOAT4 color = { 1,0,0,1 };
	float xAxis = 0;
	float zAxis = 0;
	/// KREIS (OBEN)
	// 0 Punkt (1)
	m_vertices[word++] = { XMFLOAT3(0,0,0), color, XMFLOAT3(0,1,0) };
	for (int i = 0; i < _sliceCount; i++)
	{
		// AUßENPUNKT(E) (1)
		xAxis = (cos((degree * i) / 180 * XM_PI)) * _radius;
		zAxis = (sin((degree * i) / 180 * XM_PI)) * _radius;
		pos = XMFLOAT3(xAxis, 0, zAxis);
		m_vertices[word++] = { pos, color,XMFLOAT3(0,1,0) };
	}
	for (int i = 0; i < _sliceCount; i++)
	{
		// AUßENPUNKT(E) (1.1)
		xAxis = (cos((degree * i) / 180 * XM_PI)) * _radius;
		zAxis = (sin((degree * i) / 180 * XM_PI)) * _radius;
		pos = XMFLOAT3(xAxis, 0, zAxis);
		m_vertices[word++] = { pos, color,pos };
	}
	m_vertices[word++] = { XMFLOAT3(0,-_ySize,0), color, XMFLOAT3(0,-1,0) };
	/// KREIS (UNTEN)
	// 0 Punkt (2)
	for (int i = 0; i < _sliceCount; i++)
	{
		// AUßENPUNKT(E) (2)
		xAxis = (cos((degree * i) / 180 * XM_PI))* _radius;
		zAxis = (sin((degree * i) / 180 * XM_PI))* _radius;
		pos = XMFLOAT3(xAxis, -_ySize, zAxis);
		m_vertices[word++] = { pos, color,pos };
	}
	for (int i = 0; i < _sliceCount; i++)
	{
		// AUßENPUNKT(E) (2.1)
		xAxis = (cos((degree * i) / 180 * XM_PI))* _radius;
		zAxis = (sin((degree * i) / 180 * XM_PI))* _radius;
		pos = XMFLOAT3(xAxis, -_ySize, zAxis);
		m_vertices[word++] = { pos, color,XMFLOAT3(0,-1,0) };
	}
	word = 0;
	for (int j = 1; j < _sliceCount; j++)
	{
		m_indices[word++] = 0;
		m_indices[word++] = j;
		m_indices[word++] = j + 1;
	}
	m_indices[word++] = _sliceCount;
	m_indices[word++] = 1;
	m_indices[word++] = 0;

	for (int j = 1; j < _sliceCount; j++)
	{
		m_indices[word++] = (_sliceCount * 2 + 1);
		m_indices[word++] = j + (_sliceCount * 2 + 1);
		m_indices[word++] = (_sliceCount * 2 + 1) + j + 1;
	}
	m_indices[word++] = (_sliceCount * 2 + 1);
	m_indices[word++] = (_sliceCount * 3 + 1);
	m_indices[word++] = (_sliceCount * 2 + 1) + 1;

	for (int j = 0; j < _sliceCount - 1; j++)
	{
		m_indices[word++] = (_sliceCount + 1) + j;
		m_indices[word++] = (_sliceCount + 1) + j + 1;
		m_indices[word++] = j + (_sliceCount * 3 + 2);
	}
	m_indices[word++] = (_sliceCount + 1) + 7;
	m_indices[word++] = (_sliceCount + 1);
	m_indices[word++] = 7 + (_sliceCount * 3 + 2);

	for (int j = 0; j < _sliceCount - 1; j++)
	{
		m_indices[word++] = (_sliceCount + 1) + j + 1;
		m_indices[word++] = (_sliceCount * 3 + 2) + j + 1;
		m_indices[word++] = j + (_sliceCount * 3 + 2);
	}
	m_indices[word++] = (_sliceCount + 1);
	m_indices[word++] = (_sliceCount * 3 + 2);
	m_indices[word++] = 7 + (_sliceCount * 3 + 2);
}

CTarget::~CTarget()
{
	free(m_vertices);
	delete m_indices;
}

void CTarget::Update(float _deltaTime)
{
	m_timer += _deltaTime * 5;
}

void CTarget::Render()
{
	XMMATRIX scale = XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(XMConvertToRadians(m_rotation.x), XMConvertToRadians(m_rotation.y), XMConvertToRadians(m_rotation.z));
	XMMATRIX translation = XMMatrixTranslation(m_position.x, m_position.y, m_position.z);

	DXS.m_objectConstantBuffer.m_matrix = scale * rotation * translation;
	DXS.m_deviceContext->UpdateSubresource(DXS.m_constantBuffers[CB_OBJECT], 0, nullptr, &DXS.m_objectConstantBuffer, 0, 0);
	// Time over deltaTime = m_timer
	DXS.m_deviceContext->UpdateSubresource(DXS.m_constantBuffers[CB_VALUE], 0, nullptr, &m_timer, 0, 0);

	unsigned int offset = 0;
	unsigned int stride = sizeof(VertexPosColor);

	// InputAssembler Stage
	DXS.m_deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);
	DXS.m_deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R16_UINT, offset);
	DXS.m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	DXS.m_deviceContext->IASetInputLayout(DXS.m_simpleInputLayout);

	// VertexShader Stage
	DXS.m_deviceContext->VSSetConstantBuffers(0, 3, DXS.m_constantBuffers);
	DXS.m_deviceContext->VSSetShader(DXS.m_simpleVertexShader, nullptr, 0);

	// Rasterizer Stage
	DXS.m_deviceContext->RSSetState(DXS.GetCurrentRasterizer());
	DXS.m_deviceContext->RSSetViewports(1, &DXS.m_viewport);

	// PixelShader Stage
	DXS.m_deviceContext->PSSetConstantBuffers(0, 1, &DXS.m_constantBuffers[CB_FRAME]);
	DXS.m_deviceContext->PSSetConstantBuffers(1, 1, &DXS.m_constantBuffers[CB_VALUE]);
	DXS.m_deviceContext->PSSetShader(DXS.m_simplePixelShader, nullptr, 0);

	// OutputMerger Stage
	DXS.m_deviceContext->OMSetRenderTargets(1, &DXS.m_renderTargetView, DXS.m_depthStencilView);
	DXS.m_deviceContext->OMSetBlendState(DXS.GetCurrentBlendState(), DXS.m_blendFactor, DXS.m_sampleMask);
	DXS.m_deviceContext->OMSetDepthStencilState(DXS.m_depthStencilState, 1);

	// zeug zeichnen
	DXS.m_deviceContext->DrawIndexed(m_indexCount, 0, 0);
}

