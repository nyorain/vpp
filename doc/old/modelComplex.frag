#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//structs
struct PointLight
{
	vec3 position;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight
{
	vec3 direction;
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//in/outs
layout(location = 0) in vec2 ipos;
layout(location = 1) in vec2 iuv;
layout(location = 2) in vec3 inormal;

layout(location = 0) out vec4 ocol;

//material
layout(set = 0, binding = 1) uniform UBOA
{
	vec3 diffuse;
	bool diffuseMap;
	vec3 specular;
	bool specularMap;
	vec3 ambient;
	float shininess;
} material;

layout(set = 0, binding = 3) uniform sampler2D diffuseMap;
layout(set = 0, binding = 4) uniform sampler2D specularMap;

//lights
layout(set = 0, binding = 5) uniform UboPointLights
{
	PointLight pointLights[];
};

layout(set = 0, binding = 6) uniform UboDirLights
{
	PointLight dirLights[];
};


int main()
{
	if(ubo.texture)
	{
		ocol = texture(diffuseMap, iuv);
	}

	vec3 diffuseCol = material.diffuse.a ? material.diffuse : texture(diffuseMap, iuv);
	float specularFac = material.specular.a ? material.diffuse : texture(diffuseMap, iuv);
	vec3 diff = material.diffuse.a ? material.diffuse : texture(diffuseMap, iuv);

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
