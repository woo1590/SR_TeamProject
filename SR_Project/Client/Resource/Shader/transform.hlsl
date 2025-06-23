extern matrix WorldViewMatrix;
extern matrix WorldViewProjMatrix;

extern vector Color;

extern vector LightDirection;

static vector Black = { 0.f, 0.f, 0.f, 0.f };



struct VS_INPUT
{
    vector position : POSITION;
    vector normal : NORMAL;
};

struct VS_OUTPUT
{
    vector position : POSITION; 
    float2 uvCoords : TEXCOORD;
    vector diffuse : COLOR;
};


VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    return output;
}