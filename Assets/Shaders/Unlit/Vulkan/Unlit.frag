#version 450

layout(set = 0, binding = 0) uniform sampler linearSampler;
layout(set = 0, binding = 1) uniform texture2D albedoTexture;

layout(location = 0) in vec2 textCoords;

layout(location = 0) out vec4 outColor;

void main()
{
	//outColor = vec4(hashColor(uint(gl_PrimitiveID)), 1.0);
    outColor = texture(sampler2D(albedoTexture, linearSampler), textCoords);
}