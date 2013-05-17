Texture2D normalMap        : register(t0);
Texture2D diffuseAlbedoMap : register(t1);
Texture2D depthMap         : register(t2);

float4 cameraPos; //float3
float4 halfPixel; //float2
float lightIntensity = 1.0f;

SamplerState diffuseSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState depthSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

SamplerState normalSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};


cbuffer EveryFrame
{
	matrix view;
	matrix projection;
	matrix invertViewProjection;
};

struct VSIn
{
	float3 pos    : POSITION;
	float3 normal : NORMAL; //behövs igentligen inte. 
	float2 uv     : TEXTCOORD; //behövs igentligen inte. 

	row_major float4x4 world  : WORLD;
	float3 lightPos           : LIGHTPOS;
	float lightRadius         : LIGHTRADIUS;
	float3 lightColor         : LIGHTCOLOR;
};

struct PSSceneIn
{
	float4 screenPos     : SV_Position;
	float4 worldPos      : worldPos;

	float4 worldLightPos : LIGHTPOS;
	float3 lightColor    : LIGHTCOLOR;
	float  lightRadius   : LIGHTRADIUS;
};

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output = (PSSceneIn)0;

	output.screenPos = mul(float4(input.pos, 1), mul(input.world , mul(view , projection)));
	output.worldPos  =  mul(float4(input.pos, 1), input.world);

	output.worldLightPos = mul(float4(input.lightPos, 1) ,  mul(input.world , mul(view , projection)));
	output.lightColor = input.lightColor;
	output.lightRadius = input.lightRadius;
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{
	//räkna ut UV koordinater för sampling
	input.screenPos.xy /= input.screenPos.w;
	float2 uv = 0.5f * (float2(input.screenPos.x, -input.screenPos.y) +1 );
	uv -= halfPixel;

	//sampling av texturer
	float4 normalData = normalMap.Sample(normalSampler, uv);
	float3 normal = 2.0f * normalData.xyz - 1.0f;
	float specularPower = normalData.a * 255;
	float specularIntensity = diffuseAlbedoMap.Sample(diffuseSampler, uv).a;
	float depth = depthMap.Sample(depthSampler, uv).r;

	//uträkning av världsposition
	float4 position;
	position.xy = input.screenPos.xy;
	position.z = depth;
	position.w = 1.0f;
	position = mul(position, invertViewProjection);
	position /= position.w;


	//ljusberäkningar
	//--------------------------------------------------------------------------------
		//Attenuation
		float3 lightVector = input.worldLightPos - position;
		float attenuation = saturate(1.0f - length(lightVector)/input.lightRadius);

		//diffuseLight
		lightVector = normalize(lightVector);
		float nDL = max(0, dot(normal, lightVector));
		float3 diffuseLight = nDL * input.lightColor.rgb;

		//specularLight
		float3 directionToCamera = normalize(cameraPos - position);
		float3 reflectionVector = normalize(reflect(-lightVector, normal));
		float specularLight = specularIntensity * pow( saturate(dot(reflectionVector , directionToCamera)), specularPower);

	//--------------------------------------------------------------------------------



	return attenuation * lightIntensity * float4(diffuseLight.rgb, specularLight);
}


//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState LightCulling
{
	//CullMode = //CullCounterClockwiseFace;
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
	    
	    SetRasterizerState( LightCulling );
    } 
}
