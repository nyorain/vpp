#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inSpeed;
layout (location = 2) in vec4 inColor;

layout (binding = 0) uniform UBO
{
	mat4 view;
	mat4 projection;
} ubo;

layout (location = 0) out vec4 outColor;

void main()
{
	outColor = inColor;
	gl_Position = vec4(inPos, 1.0);
}
