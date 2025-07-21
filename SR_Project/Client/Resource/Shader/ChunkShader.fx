// ChunkShader.fx

// AlbedoMap texture and sampler
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

float4x4 g_World : WORLD;
float4x4 g_View : VIEW;
float4x4 g_Proj : PROJECTION;
float3 lightDir = { -1.f, -1.f, -1.f };
float1 ambient = 0.4f;

struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 TexCoord : TEXCOORD0;
};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
    float1 Diffuse : TEXCOORD1;
};


VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(float4(input.Position, 1.f), g_World);
    float4x4 viewproj = mul(g_View, g_Proj);
    float3 light = normalize(-1.f * lightDir);
    
    output.Position = mul(worldPos, viewproj);
    output.TexCoord = input.TexCoord;
    
    float3 worldNormal = mul(input.Normal, (float3x3) g_World);
    float1 diff = max(dot(worldNormal, light), 0);
    output.Diffuse = saturate(ambient + (1.f - ambient) * diff);
    
    return output;
}

float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    return tex2D(AlbedoSampler, input.TexCoord) * input.Diffuse;
}

technique Chunk
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
