Texture2DArray textures: register(t0);
SamplerState ss
{

};
struct VSIn
{
	float3 pos : POSITION;
	float2 uv : UV;
	
	row_major float4x4 world : WORLD;
	uint textureID : TEXTUREID;
	uint instanceID : SV_InstanceID;
};

struct PSIn
{
	float4 pos : SV_Position;
	uint textureID : TEXTUREID;
	float2 uv : TEXTCOORD;
};

RasterizerState NoCulling
{
	CullMode = NONE;
	FillMode = Solid;
};

PSIn VSScene(VSIn input)
{
	PSIn output;
	output.pos = mul(float4(input.pos, 1), input.world);
	output.textureID = input.textureID;
	output.uv = input.uv;
	return output;
}

float4 PSScene(PSIn input) : SV_Target
{
	return float4(textures.Sample( ss , float3(input.uv.x, input.uv.y, input.textureID)).rgb, 1.0f);
	
}

technique11 BasicTech
{
	pass p0
	{
		SetVertexShader(CompileShader(vs_4_0, VSScene()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSScene()));

		SetRasterizerState(NoCulling);
	}
}