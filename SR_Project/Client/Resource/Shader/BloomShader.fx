
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

float g_fBloomThreshold = 1.0f;

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
    float4 color = tex2D(AlbedoSampler, input.uv);

    float luminance = dot(color.rgb, float3(0.2126f, 0.7152f, 0.0722f));

    if (luminance > g_fBloomThreshold)
    {
        return color; 
    }
    else
    {
        return float4(0, 0, 0, 1); 
    }
    return color;
}

technique BloomShader
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
