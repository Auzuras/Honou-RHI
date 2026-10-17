cbuffer ModelViewProjectionCB : register(b0)
{
    matrix Model;
    matrix ProjectionView;
};

struct VertexShaderInput
{
    float3 Position     :   POSITION;
    float3 Normal       :   NORMAL;
    float2 TexCoords    :   TEXCOORD;
};

struct VertexShaderOutput
{
    float4 Position     :   SV_Position;
    float2 TexCoords    :   TEXCOORD;
};

VertexShaderOutput main(VertexShaderInput IN)
{
    VertexShaderOutput OUT;
    
    OUT.TexCoords   =   IN.TexCoords;
    float4 worldP   =   mul(Model, float4(IN.Position, 1.0f));
    OUT.Position    =   mul(ProjectionView, worldP);
    
    return OUT;
}