#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, location = 0) in vec4 inPos;
layout (set = 0, location = 1) in vec4 inVel;
layout (set = 0, location = 2) in vec4 inCol;

layout (binding = 0) uniform UBO
{
	mat4 view;
	mat4 projection;
} ubo;

layout (location = 0) out vec4 outColor;

void main()
{
	gl_PointSize = 1.0;
	outColor = inCol;
	gl_Position = vec4(inPos);
}
