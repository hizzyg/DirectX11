// Wandelt PerPixelDaten in Farbe um, läuft einmal für jeden Pixel

cbuffer LightBuffer : register(b0)
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDir;
	float dirPadding;
	float4 specularColor;
	float3 camPos;
}
cbuffer Value : register(b1)
{
	float delta;
}
struct PixelShaderInput
{
	float4 pos : SV_POSITION;		// SV steht für Systemvariable
	float4 col : COLOR;
	float4 normal : NORMAL;			// normal ist in Worldspace
	float4 worldPos : POSITION1;
};

float4 SimplePixelShader(PixelShaderInput _in) : SV_TARGET
{
	float4 m_color1 = float4(1,0,0,1);
	float4 m_color2 = float4(0,0,1,1);
	float3 diffuseDir = normalize(float3(1, -1, 1));
	float d = abs(length(_in.worldPos.xz * 10) - delta);
	return m_color1 * (((sign(fmod(d, 5) - 2)) + 1) * 0.5f) + m_color2 * (1 - (((sign(fmod(d, 5) - 2)) + 1) * 0.5f));
	
	float4 ambient = saturate(ambientColor * _in.col);
	float4 diffuse = saturate(_in.col * diffuseColor * (dot(normalize(lightDir), normalize(_in.normal))));
	float3 halfVector = normalize(normalize(camPos - _in.worldPos) + normalize(lightDir));
	float4 specular = saturate(specularColor * pow(dot(halfVector,normalize(_in.normal)),50));
	return float4(saturate(ambient + diffuse + specular));
}
