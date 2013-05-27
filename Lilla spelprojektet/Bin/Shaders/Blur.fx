Texture2D inputTex   : register(t0);

static const float TEXEL_SIZE_WIDTH = 1.0f/1920;
static const float TEXEL_SIZE_HEIGHT = 1.0f/1080;
static const float WEIGHT[9] =
{
	0.05f, 0.05f, 0.1f, 0.15f, 0.3f, 0.15f, 0.1f, 0.05f, 0.05f
};

static const int BLURRADIUS = 4;

cbuffer everyCall
{
	float blurScalar;
}

SamplerState blurSampler
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

struct VSin
{
	float3 posH : POSITION;
	float2 tex : UV;
};

struct VSout
{
	float4 pos : SV_Position;
	float2 tex : UV;
};

struct PSout
{
	float4 blur : SV_TARGET;
};

VSout VSScene(VSin input)
{
	VSout output = (VSout)0;
	output.pos = float4(input.posH, 1.0f);
	output.tex = input.tex;

	return output;
}

PSout PSScene(VSout input, uniform bool blur)
{
	PSout output = (PSout)0;

	float2 texOffset;
	float4 glowResult = float4(0.0f, 0.0f, 0.0f, 0.0f);
	
	if(blur)
	{
		texOffset = float2(TEXEL_SIZE_WIDTH, 0.0f);
	}
	else
	{
		texOffset = float2(0.0f, TEXEL_SIZE_HEIGHT);
	}

	[unroll]
	//for(int j = 0; j < 2; j++)
		for(int i = -BLURRADIUS; i < BLURRADIUS; i++)
		{
			float2 uv = input.tex + i*texOffset * blurScalar;
			glowResult += (inputTex.Sample(blurSampler, uv) * WEIGHT[i+BLURRADIUS]);
		}

	output.blur = glowResult;

	return output;
}

technique11 BlurTech
{
	pass HorizontalBlur
	{
		SetVertexShader(CompileShader(vs_4_0, VSScene()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSScene(true)));
	}

	pass VerticalBlur
	{
		SetVertexShader(CompileShader(vs_4_0, VSScene()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PSScene(false)));
	}
};