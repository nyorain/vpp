#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec2 iTexCoords;

layout(location = 0) out vec2 oTexCoords;

void main()
{
	oTexCoords = (iTexCoords) / vec2(256, 256);
	// gl_Position = vec4(2.0 * (iPos / vec2(1024, 1024)) - 1.0, 0.0, 1.0);
	gl_Position = vec4(2.0 * (iPos / vec2(1400, 900)) - 1.0, 0.0, 1.0);
}
