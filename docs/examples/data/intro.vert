#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec4 inCol;

layout (location = 0) out vec4 outColor;

void main()
{
	outColor = inCol;
	gl_Position = vec4(inPos, 0.0, 1.0);
}
