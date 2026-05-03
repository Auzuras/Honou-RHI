#version 450

#define HONOU_SHADER_VULKAN

#include "Common.sf"

float2 positions[3] = float2[](
	float2(0.0, -0.5),
	float2(0.5, 0.5),
	float2(-0.5, 0.5)
);

float3 colors[3] = float3[](
	float3(1.0, 0.0, 0.0),
	float3(0.0, 1.0, 0.0),
	float3(0.0, 0.0, 1.0)
);

S_OUTPUT



E_OUTPUT

layout(location = 0) out float3 fragColor;

MAIN_ENTRY()
{
	sc_Position = float4(positions[sc_VertexIndex], 0.0, 1.0);
	fragColor = colors[sc_VertexIndex];
}