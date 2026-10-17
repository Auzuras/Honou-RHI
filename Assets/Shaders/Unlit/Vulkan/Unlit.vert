#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTextCoords;

layout(location = 0) out vec2 textCoords;

layout(push_constant, std430) uniform constantBuffer
{
	mat4x4 Model;
	mat4x4 ProjectionView;
};

void main()
{
	textCoords = inTextCoords;

	vec4 position = Model * vec4(inPosition, 1.0);
	gl_Position = ProjectionView * position;
}