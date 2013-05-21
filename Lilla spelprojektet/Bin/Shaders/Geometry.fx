
Texture2DArray textures   : register(t0);
Texture2DArray glowMaps	  : register(t1);
SamplerState anisoSampler
{
	AddressU = WRAP;
	AddressV = WRAP;
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
	float4 posCS   : SV_Position;
	float4 posW    : POSITION;
	float4 normalW : TEXTCOORD0;
	float2 uv      : TEXTCOORD1;
	float2 depth   : DEPTH;

	uint textureID : TEXTUREID;
};

struct PSOut
{
	float4 light          : SV_TARGET0;
	float4 diffuseAlbedo  : SV_TARGET1;
	float4 normal         : SV_TARGET2;
	float4 glow			  : SV_TARGET3;

};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn VSScene(VSIn input)
{
	PSIn output = (PSIn)0;
	output.posCS = mul(float4(input.pos, 1) , input.world);
	output.posCS = mul(output.posCS, mul(view, proj));
	output.posW  = mul(float4(input.pos, 1) , input.world);
	output.normalW = normalize(mul(float4(input.normal, 0) , input.world));
	output.uv = input.uv;
	output.textureID = input.textureID;
	output.depth.x = output.posCS.z;
	output.depth.y = output.posCS.w;
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
PSOut PSScene(PSIn input)
{
	PSOut output = (PSOut)0;

	float3 diffuseAlbedo = textures.Sample( anisoSampler , float3(input.uv.x, input.uv.y, input.textureID)).rgb;

	float3 glow = glowMaps.Sample(anisoSampler, float3(input.uv.x, input.uv.y, input.textureID)).rgb;
	if(((glow.x < 1.0f) && (glow.y < 1.0f)) && (glow.z < 1.0f))
	{
		glow = float3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		glow = diffuseAlbedo;
	}

	output.diffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.glow = float4(glow, 1.0f);
	float4 normalW = float4( 0.5f * (normalize(input.normalW).rgb + 1.0f), specularPower);
	
	output.diffuseAlbedo = float4(diffuseAlbedo, 1.0f);
	output.normal = normalW;
	output.light = float4(0.0f, 0.0f , 0.0f, 0.0f);

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




