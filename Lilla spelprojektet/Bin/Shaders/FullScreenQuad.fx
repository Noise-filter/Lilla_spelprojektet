//0 = Position
//1 = DiffuseAlbedo
//2 = Normal

Texture2D resources[3];
Texture2D normalMap;
Texture2D diffuseAlbedoMap;
Texture2D  positionMap;

struct VSIn
{
	float3 pos : POSITION;
};

struct PSIn
{
	float4 pos : SV_Position;
};

PSIn VSScene(VSIn input)
{	
	PSIn output =(PSIn)0;
	output.pos = float4(input.pos, 1.0f);

	return output;
}


float4 PSScene(PSIn input) : SV_Target
{
	int3 sampleIndices = int3(input.pos.xy, 0);

	return float4( resources[1].Load(sampleIndices).xyz, 1.0f);

	//return float4( 1, 0, 0 , 1 );
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
        SetVertexShader( CompileShader(vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    
	    SetRasterizerState( NoCulling );
    } 
}

