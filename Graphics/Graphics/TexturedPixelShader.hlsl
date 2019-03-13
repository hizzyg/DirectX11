// Wandelt PerPixelDaten in Farbe um, läuft einmal für jeden Pixel

Texture2D mainTex : register(t0);
SamplerState mainSampler : register(s0);

cbuffer LightBuffer : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDir;
	float dirPadding;
	float4 specularColor;
	float3 camPos;
}

struct PixelShaderInput
{
	float4 pos : SV_POSITION;		// SV steht für Systemvariable
	float4 col : COLOR;
	float4 normal : NORMAL;			// normal ist in Worldspace
	float4 worldPos : POSITION1;
	float2 uv : TEXCOORD;
};

float4 TexturedPixelShader(PixelShaderInput _in) : SV_TARGET
{
	float4 col = _in.col * mainTex.Sample(mainSampler, _in.uv);

	float4 ambient = saturate(col * ambientColor);
	// Normalen müssen im Pixelshader noch mal normalisiert werden wegen dem Rasterizer

	float d = dot(normalize(lightDir), normalize(_in.normal));
	float4 diffuse = saturate(col * diffuseColor * d);

	float3 halfVector = normalize(normalize(camPos - _in.worldPos) + normalize(lightDir));
	d = dot(halfVector, normalize(_in.normal));
	d = pow(d, 50);
	float4 specular = saturate(specularColor * d);

	// Saturate clampt einen Wert zwischen 0 und 1
	return saturate(ambient + diffuse + specular);
}
