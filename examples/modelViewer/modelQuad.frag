#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 iuv;
layout(location = 0) out vec4 ocol;

layout(set = 0, binding = 0) uniform sampler2D tex;

float linearize(float depth)
{
	float n = 0.1; //z near
	float f = 100.0; //z far
	float z = depth;
	return (2.0 * n) / (f + n - z * (f - n));
}


void main()
{
	float val = 1.0 - linearize(texture(tex, iuv).r);
	ocol = vec4(val, val, val, 1.0);
	// ocol = vec4(0.0, iuv, 0.0);
}
