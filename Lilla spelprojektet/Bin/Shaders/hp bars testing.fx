cbuffer EveryFrame
{
	matrix gWVP;
};

struct VSIn
{
	float3 pos : POSITION;
	float2 uv : UV;
	row_major float4x4 world : WORLD;
	uint instanceID : SV_InstanceID;
};

struct PSIn
{
	float4 pos : SV_POSITION;
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
	output.pos = input.pos;

	return output;
}

float4 PSScene(PSIn input) : SV_TARGET
{
	return float4(1, 1, 1, 1);
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