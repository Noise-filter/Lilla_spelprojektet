
Texture2DArray textures   : register(t0);
Texture2DArray glowMaps	  : register(t1);
SamplerState anisoSampler : register(s0);
static const float TEXEL_SIZE = 1.0f/2048;
static const float WEIGHT[9] =
{
	0.05f, 0.05f, 0.1f, 0.15f, 0.3f, 0.15f, 0.1f, 0.05f, 0.05f
};

static const float2 OFFSET[9] = 
{
	float2(-TEXEL_SIZE, -TEXEL_SIZE),	float2(0.0f, -TEXEL_SIZE),	float2(TEXEL_SIZE, -TEXEL_SIZE),
	float2(-TEXEL_SIZE, 0.0f),			float2(0.0f, 0.0f),			float2(TEXEL_SIZE, 0.0f),
	float2(-TEXEL_SIZE, TEXEL_SIZE),		float2(0.0f, TEXEL_SIZE),	float2(TEXEL_SIZE, TEXEL_SIZE)
};

float specularIntensity = 0.8f;
float specularPower     = 0.5f;

cbuffer EveryFrame
{
	matrix view;
	matrix proj;
};
//--------------------------
//input for VSScene
//--------------------------
struct VSIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXTCOORD;

	row_major float4x4 world : WORLD;
	uint textureID : TEXTUREID;
	uint instanceID : SV_InstanceID;
};

struct PSIn
{
	float4 posCS  : SV_Position;
	float4 posW   : POSITION;
	float4 normalW : TEXTCOORD0;
	float2 uv : TEXTCOORD1;

	uint textureID : TEXTUREID;
};

struct PSOut
{
	float4 position       : SV_TARGET0;
	float4 diffuseAlbedo  : SV_TARGET1;
	float4 normal         : SV_TARGET2;
	float4 blur			  : SV_TARGET3;
	
};

float3 CalcBlur(float2 pos, uint texID, float2 uv)
{
	float3 glowRet = float3(0.0f, 0.0f, 0.0f);
	float2 blurTex = float2(uv.x, uv.y);

	[unroll]
	for(int i = 0; i < 9; i++)
	{
		glowRet += (glowMaps.Sample(anisoSampler, float3((blurTex.x + OFFSET[i].x), (blurTex.y + OFFSET[i].y), texID)).rgb * WEIGHT[i]);
	}

	return glowRet/=9;
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn VSScene(VSIn input)
{
	PSIn output = (PSIn)0;

	output.posCS = mul(float4(input.pos, 1), mul(input.world, view));
	output.posW  = mul(float4(input.pos, 1), input.world);
	output.normalW = normalize(mul(float4(input.normal, 0), input.world));
	output.uv = input.uv;
	output.textureID = input.textureID;
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
PSOut PSScene(PSIn input)
{
	PSOut output = (PSOut)0;

	float3 diffuseAlbedo = textures.Sample( anisoSampler , float3(input.uv.x, input.uv.y, input.textureID)).rgb;
	float3 glow = CalcBlur(input.posW.xy, input.textureID, input.uv);
	float4 normalW = float4((normalize(input.normalW)).rgb, specularPower);
	
	float4 pos = input.posCS;
	pos.w = input.posCS.z / input.posCS.w;
	output.diffuseAlbedo = float4(diffuseAlbedo, 1.0f);

	//else
	//{
	//	float3 sum = saturate(diffuseAlbedo + (glow*0.5));
	//	output.diffuseAlbedo = float4(sum, 1.0f);
	//}

	output.normal = normalW;
	output.blur = float4(glow, 1.0f);

	return output;
}

RasterizerState NoCulling
{
	CullMode = NONE;
};
RasterizerState wire
{
	CullMode = NONE;
	FillMode = Wireframe;
};

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    
	    SetRasterizerState( NoCulling );
    }

}




