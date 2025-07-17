//AnimationParticle.fx

//Albedo Texture and Sampler

#pragma enable_d3d11_debug_symbols

texture AlbedoMap : register(t0);
sampler AlbedoSampler = sampler_state
{
    Texture = <AlbedoMap>;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

float4x4 g_View;
float4x4 g_Proj;

float3 uvScale;
float3 uvOffset;

struct VS_Input
{
    float3 position : POSITION;
    float size : PSIZE;
    float4 color : COLOR0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float size : PSIZE;
    float4 color : COLOR0;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output output;
    
    float4 pos = mul(float4(input.position,1.f), g_View);
    output.position = mul(pos, g_Proj);
    output.size = input.size;
    output.color = input.color;
    
    return output;
}

float4 PS_Main(VS_Output input, float2 uv :TEXCOORD0) : COLOR0
{
    float2 localUVOffset = float2(uvOffset.xy);
    float2 localUVScale = float2(uvScale.xy);
    
    uv = uv * localUVScale + localUVOffset;
    
    float4 tex = tex2D(AlbedoSampler, uv);
    return tex * input.color;
}

technique AnimationParticle
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}