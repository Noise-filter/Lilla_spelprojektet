
Texture2D normalMap                 : register(t0);
Texture2D diffuseAlbedoMap          : register(t1);
Texture2D  positionMap              : register(t2);

float4 PSScene(in float4 screenPos : SV_Position) : SV_Target
{
	int3 sampleIndices = int3(screenPos.xy, 0);

	return float4( diffuseAlbedoMap.Load(sampleIndices).xyz, 1.0f);
}

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( NULL );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
	    
	    SetRasterizerState( NoCulling );
    } 
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
