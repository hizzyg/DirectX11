// Wandelt PerPixelDaten in Farbe um, läuft einmal für jeden Pixel

Texture2D heightTex : register(t0);
SamplerState heightSampler : register(s0);
Texture2D splatTex : register(t1);
SamplerState splatSampler : register(s1);

Texture2D Tex1 : register(t2);
SamplerState Sampler1 : register(s2);
Texture2D Tex2 : register(t3);
SamplerState Sampler2 : register(s3);
Texture2D Tex3 : register(t4);
SamplerState Sampler3 : register(s4);
Texture2D Tex4 : register(t5);
SamplerState Sampler4 : register(s5);

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

float4 TerrainPixelShader(PixelShaderInput _in) : SV_TARGET
{
	float4 splat = splatTex.Sample(splatSampler, _in.uv);
	// Mittelwert sorgt für Abdunklung bei mehr als einer aktiven Farbe
	splat = splat / (splat.r + splat.g + splat.b + splat.a);
	
	float4 col1 = Tex1.Sample(Sampler1, _in.worldPos.xz);
	float4 col2 = Tex2.Sample(Sampler2, _in.worldPos.xz * 2.1);
	float4 col3 = Tex3.Sample(Sampler3, _in.worldPos.xz * 0.33);
	float4 col4 = Tex4.Sample(Sampler4, _in.worldPos.xz * 1.337);

	float4 col = _in.col * (col1 * splat.r + col2 * splat.g + col3 * splat.b + col4 * splat.a);

	float4 ambient = saturate(col * ambientColor);
	// Normalen müssen im Pixelshader noch mal normalisiert werden wegen dem Rasterizer

	float3 normal = heightTex.Sample(heightSampler, _in.uv).xyz;
	normal = normalize(normal * 2 - float3(1,1,1));
	normal = mul(worldMatrix, float4(normal, 0));
	normal = normalize(normal);

	float d = dot(normalize(lightDir), normal);
	float4 diffuse = saturate(col * diffuseColor * d);

	float3 halfVector = normalize(normalize(camPos - _in.worldPos) + normalize(lightDir));
	d = dot(halfVector, normal);
	d = pow(d, 50);
	float4 specular = saturate(specularColor * d * 0.1);

	// Saturate clampt einen Wert zwischen 0 und 1
	return saturate(ambient + diffuse + specular);
}
