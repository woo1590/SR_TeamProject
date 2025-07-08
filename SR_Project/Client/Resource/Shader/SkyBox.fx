// SkyboxPriority.fx

// Cubemap texture and sampler
texture CubeMap : register(t0);
sampler CubeSampler = sampler_state
{
    Texture = <CubeMap>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Clamp;
    AddressV = Clamp;
    AddressW = Clamp;
};

// Shader constant matrices
float4x4 g_World : WORLD;
float4x4 g_View : VIEW;
float4x4 g_Proj : PROJECTION;

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


VS_OUTPUT VS_Skybox(VS_INPUT input)
{
    VS_OUTPUT output;
    
    float4x4 viewNoTrans = g_View;
    viewNoTrans[3].xyz = float3(0.f, 0.f, 0.f);
    
    float4 worldPos = mul(float4(input.Position, 1.0f), g_World);
    
    float4 viewPos = mul(worldPos, viewNoTrans);
    output.Position = mul(viewPos, g_Proj);
    
    output.TexCoord = input.Position;
    return output;
}

// Pixel Shader: sample from cubemap
float4 PS_Skybox(VS_OUTPUT input) : SV_Target
{
    return texCUBE(CubeSampler, input.TexCoord);
}

// Technique for priority rendering of skybox
technique SkyboxPriority
{
    pass P0
    {
        // Compile and set shaders
        VertexShader = compile vs_3_0 VS_Skybox();
        PixelShader = compile ps_3_0 PS_Skybox();
    }
}
