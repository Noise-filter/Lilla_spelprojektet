//0 = Position
//1 = DiffuseAlbedo
//2 = Normal

Texture2D resources[3];
Texture2D normalMap;
Texture2D diffuseAlbedoMap;
Texture2D  positionMap;

struct VSIn
{
	float3 pos : SV_Position;
};

struct PSIn
{
	float3 pos : POSITION;
};

PSIn VSScene(VSIn input)
{	
	PSIn output;
	output.pos = input.pos;

	return output;
}


float4 PSScene(PSIn input) : SV_Target
{
	int3 sampleIndices = int3(input.pos.xy, 0);

	return float4( diffuseAlbedoMap.Load(sampleIndices).xyz, 1.0f);
}

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

