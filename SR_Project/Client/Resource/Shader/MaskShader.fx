// MaskShader.fx

// Cubemap texture and sampler
texture CubeMap : register(t0);
sampler CubeSampler = sampler_state
{
    Texture = <CubeMap>;
    MinFilter = Point;
    MagFilter = Point;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
};

// Shader constant matrices
float4x4 g_World : WORLD;
float4x4 g_View : VIEW;
float4x4 g_Proj : PROJECTION;

int emissive;
float3 emissivecolor;
float3 color;
float emissivePow;

// Vertex input/output matching xyz, normal, cubetex declaration
struct VS_INPUT
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};
struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 TexCoord : TEXCOORD0;
};


VS_OUTPUT VS_Main(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4 worldPos = mul(float4(input.Position, 1.0f), g_World);
    
    float4 viewPos = mul(worldPos, g_View);
    output.Position = mul(viewPos, g_Proj);
    
    output.TexCoord = input.Position;
    return output;
}

float4 PS_Main(VS_OUTPUT input) : SV_Target
{
    float4 result =  texCUBE(CubeSampler, input.TexCoord);

    if (emissive != 0 && emissivePow > 0)
    {
        result.rgb += emissivecolor.rgb * emissivePow;
    }
    return result;

}
// Technique for priority rendering of skybox
technique SkyboxPriority
{
    pass P0
    {
        // Compile and set shaders
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
