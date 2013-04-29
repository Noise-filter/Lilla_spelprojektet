//
//	Simple texture shader
//

cbuffer EveryFrame
{
	matrix gWVP;
};

struct VSIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float3 color : COLOR;
};

struct PSIn
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL2;
	float3 color : COLOR2;
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

	output.pos = mul(float4(input.pos, 1), gWVP);

	output.normal = input.normal;
	output.color = input.color;

	return output;
}

float4 PSScene(PSIn input) : SV_Target
{
	return float4(input.color, 1);
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