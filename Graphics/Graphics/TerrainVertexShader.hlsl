// Wandelt Vertices vom Objekt in den Screenspace um, läuft einmal für jeden Vertex

Texture2D heightTex : register(t0);
SamplerState heightSampler : register(s0);

cbuffer PerApplication : register(b0)
{
	matrix projectionMatrix;
}

cbuffer PerFrame : register(b1)
{
	matrix viewMatrix;
}

cbuffer PerObject : register(b2)
{
	matrix worldMatrix;
}

cbuffer Terrain : register(b3)
{
	float height;
}

struct VertexShaderInput
{
	float3 pos : POSITION;
	float4 col : COLOR;
	float2 uv : TEXCOORD;
};

struct VertexShaderOutput
{
	float4 pos : SV_POSITION;		// SV steht für Systemvariable
	float4 col : COLOR;
	float4 worldPos : POSITION1;
	float2 uv : TEXCOORD;
};

VertexShaderOutput TerrainVertexShader(VertexShaderInput _in)
{
	VertexShaderOutput o;

	// Model View Projection Matrix
	matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));

	// Im AlphaKanal steht die Höhe, RGB ist für Normale reserviert
	float heightS = heightTex.SampleLevel(heightSampler, _in.uv, 0).a;

	float4 pos = float4(_in.pos + float3(0,1,0) * heightS * height, 1);
	// Bei Positionen Vektor mit 1 erweitern
	o.pos = mul(mvp, pos);
	o.col = _in.col;
	// Lichtberechnung wird im Worldspace vorgenommen
	// Bei Richtungen mit 0
	o.worldPos = mul(worldMatrix, pos);
	o.uv = _in.uv;

	return o;
}
