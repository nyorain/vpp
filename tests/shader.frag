#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec3 inFragPos;

layout (location = 0) out vec4 outFragColor;

void main()
{
	vec3 objectColor = vec3(1., 0., 0.);
	vec3 lightPos = vec3(1.2f, 1.0f, 2.0f);
	vec3 lightColor = vec3(.4, .8, 1.);
	vec3 viewPos = vec3(0., 0., 2.0f);

	vec3 norm = normalize(inNormal);
    vec3 lightDir = normalize(lightPos - inFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5f;
	vec3 viewDir = normalize(viewPos - inFragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * lightColor;

	outFragColor = vec4((diffuse + specular) * objectColor, 1.);
}
