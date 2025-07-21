//Default postprocessing

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


struct VS_Input
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_Position;
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

technique DefaultPost
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}