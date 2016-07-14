#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inVel;
layout(location = 2) in vec2 inCol;

layout (binding = 0) uniform UBO
{
	mat4 view;
	mat4 projection;
} ubo;

layout (location = 0) out vec2 outColor;

void main()
{
	gl_PointSize = 1.0;
	outColor = inCol	;
	gl_Position = vec4(inPos, 0.0, 1.0);
}
