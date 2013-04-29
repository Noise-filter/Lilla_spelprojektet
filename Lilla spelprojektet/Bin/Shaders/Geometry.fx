cbuffer EveryFrame
{
	matrix view;
	matrix proj;
};

struct VSIn
{
	float3 pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXTCOORD;

	float textureID : TEXTUREID;
	row_major float4x4 world : WORLD;
	uint instanceID : SV_InstanceID;
};

struct PSIn
{
	float4 posCS  : SV_Position;
	float3 posW : worldPos;
	float3 normalW : TEXTCOORD0;
	float2 uv : TEXTCOORD2;

	//float textureID : TEXTUREID;
};

struct PSOut
{
	float4 position       : SV_TARGET0;
	float4 diffuseAlbedo  : SV_TARGET1;
	float4 normal         : SV_TARGET2;
	
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSIn VSScene(VSIn input)
{
	PSIn output = (PSIn)0;

	
	output.posCS = mul(float4(input.pos, 1), ( proj * view *  input.world ));
	output.posW =  mul(float4(input.pos, 1), input.world);
	
	output.normalW = normalize(mul(float4(input.normal, 0), input.world));
	output.uv = input.uv;
	//output.textureID = input.textureID;
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
PSOut PSScene(PSIn input)
{
	PSOut output = (PSOut)0;
	output.position = input.posCS;

	float3 diffuseAlbedo = float3(1,0,0);//textures[input.textureID].Sample( anisoSampler, input.uv).rgb;

	float3 normalW = float3(0,1,0);// normalize(input.normalW);

	output.position = float4(input.posW, 1.0f);
	output.diffuseAlbedo = float4 ( diffuseAlbedo, 1.0f);
	output.normal = float4(normalW , 1.0f);

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




