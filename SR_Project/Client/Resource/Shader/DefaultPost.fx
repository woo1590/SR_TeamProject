//Default postprocessing

texture AlbedoMap : register(t0);
sampler AlbedoSampler = sampler_state
{
    Texture = <AlbedoMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};


struct VS_Input
{
    float4 position : POSITIONT;
    float2 uv : TEXCOORD0;
};

struct VS_Output
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output output;
    
    output.position = input.position;
    output.uv = input.uv;
    
    return output;
}

float4 PS_Main(VS_Output input) : COLOR0
{
    float4 tex = tex2D(AlbedoSampler, input.uv);
    
    return tex;
}