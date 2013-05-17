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

struct PSOut
{
	float4 pos            : SV_TARGET0;
	float4 diffuseAlbedo  : SV_TARGET1;
	float4 normal         : SV_TARGET2;
};

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

PSOut PSScene(PSIn input)
{
	PSOut output;

	output.pos = input.pos;
	output.diffuseAlbedo = float4(input.color, 1.0f);
	output.normal = float4(input.normal , 1.0f);

	return output;
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