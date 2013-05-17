
Texture2D normalMap        : register(t0);
Texture2D diffuseAlbedoMap : register(t1);
Texture2D depthMap         : register(t2);

float3 cameraPosition;
float lightIntensity = 1.0f;

cbuffer EveryFrame
{
	matrix view;
	matrix projection;
	matrix invertViewProjection;
};

struct VSIn
{
	float3 pos    : POSITION;
	float3 normal : NORMAL;
	float2 uv     : TEXTCOORD;

	row_major float4x4 world  : WORLD;
	float3 lightPos           : LIGHTPOS;
	float lightRadius         : LIGHTRADIUS;
	float3 lightColor         : LIGHTCOLOR;
};

struct PSSceneIn
{
	float4 pos  : SV_Position;
	float4 worldPos : worldPos;
	float3 normal : TEXTCOORD0;
	float2 uv : TEXTCOORD2;
	float4 projUV : TEXTCOORD3;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	//CullMode = NONE;
};
RasterizerState wire
{
	CullMode = NONE;
	FillMode = Wireframe;
};



//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output = (PSSceneIn)0;

	output.pos = mul(float4(input.pos, 1), mul(input.world , mul(view , projection)));
	output.worldPos =  mul(float4(input.pos, 1), input.world);
	
	output.normal = mul(input.normal, input.world);
	output.normal = normalize(output.normal);
	output.uv = input.uv;
	//output.projUV = mul(float4(input.Pos, 1.0f) , lightWVP);
		
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	return float4(0,1,0,1);
}

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
