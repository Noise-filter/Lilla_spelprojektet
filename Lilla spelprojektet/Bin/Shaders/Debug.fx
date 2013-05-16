Texture2D debugMap          : register(t0);
matrix world;

SamplerState anisoSampler : register(s0);


struct VSIn
{
	float4 pos : POSITION;
	float2 uv  : UV;
};

struct PSIn
{
	float4 pos : SV_Position;
	float2 uv  : UV;
};

PSIn VSScene(VSIn input)
{	
	PSIn output =(PSIn)0;
	output.pos = mul(input.pos, world);
	output.uv  = input.uv;
	return output;
}


float4 PSScene(PSIn input) : SV_Target
{
	int3 sampleIndices = int3(input.pos.xyz);

	//float4 color = debugMap.Load(sampleIndices);
	//float4 color = float4(0, 1 , 0 , 1);
	float4 color = debugMap.Sample(anisoSampler, input.uv);

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

