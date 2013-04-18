cbuffer EveryFrame
{
	matrix lightWVP;
	matrix WVP;
	matrix W;
};

struct VSIn
{
	float3 Pos : POSITION;
	float3 normal : NORMAL;
	float2 uv : TEXTCOORD;
};

struct PSSceneIn
{
	float4 Pos  : SV_Position;
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

	output.Pos = mul(float4(input.Pos, 1), WVP);
	output.worldPos =  mul(float4(input.Pos, 1), W);
	
	output.normal = mul(input.normal, W);
	output.normal = normalize(output.normal);
	output.uv = input.uv;
	output.projUV = mul(float4(input.Pos, 1.0f) , lightWVP);
		
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
