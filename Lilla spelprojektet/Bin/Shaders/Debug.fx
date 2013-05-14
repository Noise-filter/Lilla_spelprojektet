Texture2D debugMap          : register(t0);
matrix world;


struct VSIn
{
	float4 pos : POSITION;
};

struct PSIn
{
	float4 pos : SV_Position;
};

PSIn VSScene(VSIn input)
{	
	PSIn output =(PSIn)0;
	output.pos = mul(input.pos, world);

	return output;
}


float4 PSScene(PSIn input) : SV_Target
{
	int3 sampleIndices = int3(input.pos.xyz);

	float4 color = debugMap.Load(sampleIndices);

	return color;
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

