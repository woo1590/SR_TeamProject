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
};


VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 worldPos = mul(float4(input.Position, 1.f), g_World);
    float4x4 viewproj = mul(g_View, g_Proj);
    
    output.Position = mul(worldPos, viewproj);
    output.TexCoord = input.TexCoord;
    
    return output;
}

float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    return tex2D(AlbedoSampler, input.TexCoord);
}

technique Chunk
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
