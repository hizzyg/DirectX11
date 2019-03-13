// Wandelt Vertices vom Objekt in den Screenspace um, läuft einmal für jeden Vertex

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

VertexShaderOutput OSNMVertexShader(VertexShaderInput _in)
{
	VertexShaderOutput o;

	// Model View Projection Matrix
	matrix mvp = mul(projectionMatrix, mul(viewMatrix, worldMatrix));

	// Bei Positionen Vektor mit 1 erweitern
	o.pos = mul(mvp, float4(_in.pos, 1));
	o.col = _in.col;
	// Lichtberechnung wird im Worldspace vorgenommen
	// Bei Richtungen mit 0
	o.worldPos = mul(worldMatrix, float4(_in.pos, 1));
	o.uv = _in.uv;

	return o;
}
