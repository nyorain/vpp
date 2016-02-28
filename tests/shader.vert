#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inColor;

layout (binding = 0) uniform UBO
{
	mat4 transform;
} ubo;

layout (location = 0) out vec3 outColor;

void main()
{
	outColor = inColor;
	gl_Position = ubo.transform * vec4(inPos.xyz, 1.0);
}
