SamplerState linearSampler  :   register(s0);
Texture2D albedoTexture     :   register(t0);

struct PixelShaderInput
{
    float4 Position     :   SV_Position;
    float2 TexCoords    :   TEXCOORD;
};

float4 main(PixelShaderInput IN) : SV_Target
{
    return albedoTexture.Sample(linearSampler, IN.TexCoords);
}
