#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;

layout (binding = 0) uniform UBO
{
	mat4 model;
	mat4 view;
	mat4 projection;
} ubo;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec3 outFragPos;

void main()
{
	outNormal = mat3(transpose(inverse(ubo.model))) * inNormal;
	outFragPos = vec3(ubo.model * vec4(inPos, 1.0f));
	gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPos, 1.0);
	//gl_Position = ubo.model * vec4(inPos, 1.0);
}
