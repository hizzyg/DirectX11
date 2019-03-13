// Wandelt PerPixelDaten in Farbe um, läuft einmal für jeden Pixel

Texture2D mainTex : register(t0);
SamplerState mainSampler : register(s0);
Texture2D normalTex : register(t1);
SamplerState normalSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDir;
	float dirPadding;
	float4 specularColor;
	float3 camPos;
}

cbuffer PerObject : register(b1)
{
	matrix worldMatrix;
}

struct PixelShaderInput
{
	float4 pos : SV_POSITION;		// SV steht für Systemvariable
	float4 col : COLOR;
	float4 worldPos : POSITION1;
	float2 uv : TEXCOORD;
};

float4 OSNMPixelShader(PixelShaderInput _in) : SV_TARGET
{
	float4 col = _in.col * mainTex.Sample(mainSampler, _in.uv);

	float4 ambient = saturate(col * ambientColor);
	// Normalen müssen im Pixelshader noch mal normalisiert werden wegen dem Rasterizer

	float3 normal = normalTex.Sample(normalSampler, _in.uv);
	normal = normalize(normal * 2 - float3(1,1,1));
	normal = mul(worldMatrix, float4(normal, 0));
	normal = normalize(normal);

	float d = dot(normalize(lightDir), normal);
	float4 diffuse = saturate(col * diffuseColor * d);

	float3 halfVector = normalize(normalize(camPos - _in.worldPos) + normalize(lightDir));
	d = dot(halfVector, normal);
	d = pow(d, 50);
	float4 specular = saturate(specularColor * d);

	// Saturate clampt einen Wert zwischen 0 und 1
	return saturate(ambient + diffuse + specular);
}
