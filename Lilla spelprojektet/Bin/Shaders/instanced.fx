//
//	Simple texture shader
//

cbuffer EveryFrame
{
	matrix gVP;
};

struct VSIn
{
	float3 pos : POSITION;
	row_major float4x4 world : WORLD;
	uint instanceID : SV_InstanceID;
};

struct PSIn
{
	float4 pos : SV_POSITION;
};

SamplerState ss;

RasterizerState NoCulling
{
	CullMode = NONE;
	FillMode = Solid;
};

PSIn VSScene(VSIn input)
{
	PSIn output;

	output.pos = mul(float4(input.pos, 1), input.world);
	output.pos = mul(output.pos, gVP);

	return output;
}

float4 PSScene(PSIn input) : SV_Target
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