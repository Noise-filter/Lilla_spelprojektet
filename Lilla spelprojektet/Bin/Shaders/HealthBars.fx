cbuffer EveryFrame
{
	Matrix vp;
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
	float4 pos : SV_Position;
};

RasterizerState NoCulling
{
	CullMode = NONE;
	FillMode = Solid;
};

PSIn VSScene(VSIn input)
{
	PSIn output = (PSIn)0;

	output.pos = mul(float4(input.pos, 1), input.world);
	output.pos = mul(output.pos, vp);

	return output;
}

float4 PSScene(PSIn input) : SV_Target
{
	return float4(0, 1, 0, 1);
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