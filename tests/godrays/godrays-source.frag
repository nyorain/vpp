#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0, location = 0) uniform UBO
{
	float time;
} ubo;

layout(location = 0) out vec4 outFragColor;

void main()
{
	outFragColor = vec4(sin(ubo.time), 0.0, 0.0, 1.0);
}
