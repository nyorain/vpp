#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 ipos;
layout(location = 1) in vec3 inormal;
layout(location = 2) in vec2 iuv;

layout(location = 0) out vec4 ocol;

layout(set = 0, binding = 1) uniform UboScene
{
	vec3 viewPos;
	vec3 lightPos;
	vec3 lightCol;
} scene;

layout(set = 0, binding = 2) uniform UboMaterial
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
	bool texture;
} material;

layout(set = 0, binding = 3) uniform sampler2D diffuseMap;

void main()
{
	// if(material.texture)
	// {
	// 	ocol = texture(diffuseMap, iuv);
	// }

	vec3 ambientCol = scene.lightCol * material.ambient.rgb;

	vec3 norm = normalize(inormal);

	vec3 lightDir = normalize(scene.lightPos - ipos);

	float diffuseFac = max(dot(norm, lightDir), 0.0);
	// float diffuseFac = 1.0;
	vec3 diffuseCol = diffuseFac * material.diffuse.rgb;
	// vec3 diffuseCol = 0.5 + 0.5 * normalize(inormal);
	// vec3 diffuseCol = material.diffuse.rgb;
	// vec3 diffuseCol = scene.lightCol;

	vec3 viewDir = normalize(scene.viewPos - ipos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specFac = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specularCol = scene.lightCol * (specFac * material.specular.rgb);

	// vec3 result = specularCol + ambientCol + diffuseCol;
	vec3 result = vec3(0.2, 0.2, 0.2) + diffuseCol;
	ocol = vec4(result, 1.0);

	// ocol = vec4(1.0, 1.0, 1.0, 1.0);
}
