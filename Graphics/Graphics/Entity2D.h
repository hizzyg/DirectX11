#pragma once
class CEntity2D
{
public:
	CEntity2D(XMFLOAT2 _pos = XMFLOAT2(0, 0));
	~CEntity2D();

	virtual bool Initialize();
	virtual void Update(float _deltaTime);
	virtual void Render() = 0;	// purely virtual ^= abstrakt aus C#

	XMFLOAT2 m_position;
	float m_rotation;
	XMFLOAT2 m_size;


};

