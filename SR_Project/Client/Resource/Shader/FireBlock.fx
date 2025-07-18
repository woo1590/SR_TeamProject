//Fireblock.fx

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
    float3 position : POSITION;
    float3 normal : NORMAL;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 uv : TEXCOORD0;
};

float4x4 g_World : WORLD;
float4x4 g_View : VIEW;
float4x4 g_Proj : PROJECTION;

float2 ComputUV(float3 normal)
{
    float2 uv;
    
    if(abs(normal.x)>abs(normal.z))
    {
        uv.x = normal.x > 0 ? -normal.z : normal.z;
        uv.y = normal.y;
    }
    else
    {
        uv.x = normal.z > 0 ? normal.x : -normal.x;
        uv.y = normal.y;
    }
    
    return uv * 0.5f + 0.5f;
}

VS_Output VS_Main(VS_Input input)
{
    VS_Output output;
    
    float4 pos = float4(input.position.xyz, 1.f);
    output.position = mul(mul(mul(pos, g_World), g_View), g_Proj);
    output.uv = input.position;
    
    return output;
}

float3 uvOffset;
float3 uvScale;
float3 color;

float4 PS_Main(VS_Output input) : COLOR0
{
    float3 normal = normalize(input.uv);
    
    if (abs(normal.y) > max(abs(normal.x), abs(normal.z)))
        discard;
    
    float2 uv = ComputUV(normal);
    float2 offset = uvOffset.xy;
    float2 scale = uvScale.xy;
    
    uv = uv * scale + offset;
    uv.y = 1.f - uv.y;
    
    float4 col = tex2D(AlbedoSampler, uv);
    
    if(col.r>=0.99 && col.g>= 0.99 && col.b>=0.99)
        return col;
    
    float1 t = col.r;
    return float4(color.rgb * t, col.a);
}

technique fireblock
{
    pass P0
    {
        CullMode = None;
        
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}

