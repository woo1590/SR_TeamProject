//SpriteShader.fx

//Albedomap texture and sampler

texture AlbedoMap : register(t0);
sampler AlbedoSampler = sampler_state
{
    Texture = <AlbedoMap>;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

float4x4 g_World;
float4x4 g_View;
float4x4 g_Proj;

float3 color;
float time;

float2 UVScrollSpeed = float2(0.1f, 0.f);

struct VS_Input
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD0;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output output;
    
    float4 pos = float4(input.position, 1.f);
    output.position = mul(mul(mul(pos, g_World), g_View), g_Proj);
    output.texCoord = input.texCoord;
    
    return output;
}

float4 PS_Main(VS_Output input):SV_Target0
{
    float2 scrolledUV = input.texCoord + (UVScrollSpeed * time);
    
    float4 texColor = tex2D(AlbedoSampler, scrolledUV);
    //float4 result = tex2D(AlbedoSampler, input.texCoord);
    
    texColor.rgb *= color;
    
    return texColor;
}

technique Sprite
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}