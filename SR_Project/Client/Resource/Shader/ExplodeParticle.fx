//Explode Particle

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

float4x4 g_View : VIEW;
float4x4 g_Proj : PROJECTION;

struct VS_INPUT
{
    float3 position : POSITION;
    float1 size : PSIZE;
    float4 color : COLOR0;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float1 size : PSIZE;
    float4 color : COLOR0;
};

VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 pos = float4(input.position, 1.f);
    pos = mul(pos, g_View);
    pos = mul(pos, g_Proj);
    
    output.position = pos;
    output.size = input.size;
    output.color = input.color;
    
    return output;
}

float4 PS_Main(VS_OUTPUT input, float2 uv : TEXCOORD0) : COLOR0
{
    float1 mask = tex2D(AlbedoSampler, uv).r;
    if(mask<=0.01f)
        discard;
    
    float4 tex = tex2D(AlbedoSampler, uv) * input.color;
    tex.a *= mask;
    
    return tex;
}

technique Particle
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}