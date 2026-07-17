#version 450

layout(location = 0) in vec3 inPosition;

layout(push_constant, std430) uniform constantBuffer
{
	mat4x4 Model;
	mat4x4 ProjectionView;
};

void main()
{
	vec4 position = Model * vec4(inPosition, 1.0);
	gl_Position = ProjectionView * position;
}