


struct VS_Input
{
    float4 position : POSITION;
    float3 color : COLOR0;
};

struct VS_Output
{
    float4 position : SV_Position;
    float3 color : COLOR0;
};

VS_Output VS_Main(VS_Input input)
{
    VS_Output output;
    
    output.position = input.position;
    output.color = input.color;
    
    return output;
}

float1 alpha;

float4 PS_Main(VS_Output input):COLOR0
{
    return float4(input.color.rgb, alpha);
}

technique overlay
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}