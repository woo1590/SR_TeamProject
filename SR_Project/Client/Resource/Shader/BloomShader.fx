
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

texture BloomMap : register(t1);
sampler BloomSampler = sampler_state
{
    Texture = <BloomMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};

texture BlurredBloomMap : register(t2); 
sampler BlurredBloomSampler = sampler_state
{
    Texture = <BlurredBloomMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
};
float g_fBloomThreshold = 1.0f;

static const float gaussianWeights[9] =
{
    0.05f, 0.10f, 0.15f, 0.20f, 0.25f, 0.20f, 0.15f, 0.10f, 0.05f
};
float SizeX = (1.f / 1280)*5.f;

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


VS_Output VS_BrightPass(VS_Input input)
{
    VS_Output output;
    
    output.position = input.position;
    output.uv = input.uv;
    
    return output;
}

float4 PS_BrightPass(VS_Output input) : COLOR0
{
    float4 color = tex2D(AlbedoSampler, input.uv);

    float channelValueToCheck = color.r; 

    if (channelValueToCheck >= g_fBloomThreshold)
    {
        return float4(color.r, 0.0, 0.0, color.a);
    }
    else
    {
        return float4(0, 0, 0, 1); 
    }
}


float4 PS_GaussianBlur(VS_Output input) : COLOR0
{
    float4 finalColor = float4(0, 0, 0, 0);
    float totalWeight = 0.0;

    for (int i = 0; i < 9; ++i)
    {
        float offset = (float) i - (9 / 2);

        float2 sampleUV = input.uv + float2(offset * SizeX, 0.0);
        
        float4 sampledColor = tex2D(BloomSampler, sampleUV);
        float weight = gaussianWeights[i];

        finalColor += sampledColor * weight;
        totalWeight += weight;
    }

    return finalColor / totalWeight;
}

float4 PS_Composite(VS_Output input) : COLOR0
{
    float4 originalColor = tex2D(AlbedoSampler, input.uv);

    float4 blurredBloomColor = tex2D(BlurredBloomSampler, input.uv);

    return saturate(originalColor + blurredBloomColor);
}

technique BloomShader
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_BrightPass();
        PixelShader = compile ps_3_0 PS_BrightPass();
    }

    pass P1
    {
        VertexShader = compile vs_3_0 VS_BrightPass();
        PixelShader = compile ps_3_0 PS_GaussianBlur();
    }

    pass P2
    {
        VertexShader = compile vs_3_0 VS_BrightPass();
        PixelShader = compile ps_3_0 PS_Composite();
    }
}
