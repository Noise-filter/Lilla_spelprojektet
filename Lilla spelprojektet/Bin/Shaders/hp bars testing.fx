
cbuffer EveryFrame
{
	matrix gWVP;
};

struct VSIn
{
	float4 pos : POSITION;
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