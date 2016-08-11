#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 ipos;
layout(location = 1) in vec3 inormal;
layout(location = 2) in vec2 iuv;

layout(location = 0) out vec2 ouv;
layout(location = 1) out vec3 onormal;
layout(location = 2) out vec3 opos;

layout(set = 0, binding = 0) uniform UBO
{
	mat4 viewMat;
	mat4 transformMat;
} ubo;

int main()
{
	ouv = iuv;
	onormal = inormal;
	gl_Position = ubo.viewMat * ubo.transformMat * ipos;
}
