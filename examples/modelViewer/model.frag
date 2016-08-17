#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 ipos;
layout(location = 1) in vec2 iuv;
layout(location = 2) in vec3 inormal;

layout(location = 0) out vec4 ocol;

layout(set = 0, binding = 1) uniform UboScene
{
	vec3 lightColor;
	vec3 lightPos;
	vec3 viewPos;
} scene;

layout(set = 0, binding = 2) uniform UboMaterial
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	bool texture;
} material;

layout(set = 0, binding = 3) sampler2D diffuseMap;

int main()
{
	if(ubo.texture)
	{
		ocol = texture(diffuseMap, iuv);
	}

	vec3 ambientColor = scene.lightColor * material.ambient.rgb;

	vec3 norm = normalize(inormal);
	vec3 lightDir = normalize(ubo.lightPos - ipos);

	float diffuseFac = max(dot(norm, lightDir), 0.0);
	vec3 diffuseColor = diffuseFac * material.diffuse;

	vec3 viewDir = normalize(ubo.viewPos - ipos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specFac = pow(max(dot(viewDir, reflectDir)), 0.0), material.shininess);
	vec3 specularCol = scene.lightCol * (specFac * material.specular);

	vec3 result = specularCol + ambientCol + diffuseCol;
	ocol = vec4(result, 1.0);
}
