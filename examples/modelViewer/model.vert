#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 ipos;
layout(location = 1) in vec3 inormal;
layout(location = 2) in vec2 iuv;

layout(location = 0) out vec3 opos;
layout(location = 1) out vec3 onormal;
layout(location = 2) out vec2 ouv;
layout(location = 3) out vec4 oshadowpos;

layout(set = 0, binding = 0) uniform UBO
{
	mat4 transform;
} ubo;

layout(set = 0, binding = 1) uniform UboScene
{
	vec3 viewPos;
	vec3 lightDir;
	vec3 lightCol;
	mat4 shadowTransform;
} scene;

void main()
{
	ouv = iuv;
	onormal = inormal;
	opos = ipos;
	gl_Position = ubo.transform * vec4(ipos, 1.0);
	oshadowpos = scene.shadowTransform * vec4(ipos, 1.0);
}
