#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 ipos;
layout(location = 1) in vec3 inormal_unused;
layout(location = 2) in vec2 iuv_unused;

layout(set = 0, binding = 0) uniform UBO
{
	vec3 a;
	vec3 b;
	vec3 c;
	mat4 transform;
} ubo;

void main()
{
	// gl_Position = vec4(ipos, 1.0);
	gl_Position = ubo.transform * vec4(ipos, 1.0);
}
