#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 ipos;
layout(location = 1) in vec3 inormal;
layout(location = 2) in vec2 iuv;

layout(location = 0) out vec3 opos;
layout(location = 1) out vec3 onormal;
layout(location = 2) out vec2 ouv;

layout(set = 0, binding = 0) uniform UBO
{
	mat4 transform;
} ubo;

void main()
{
	ouv = iuv;
	onormal = inormal;
	opos = ipos;
	gl_Position = ubo.transform * vec4(ipos, 1.0);
	// gl_Position = vec4(ipos, 1.0);
}
