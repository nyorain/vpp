#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 iTexCoords;
layout(location = 0) out vec4 oColor;

layout(set = 0, binding = 0) uniform sampler2D tex;
const vec3 fontColor = vec3(1.0, 0.0, 1.0);

float median(float a, float b, float c)
{
	return max(min(a,b), min(max(a,b),c));
}

void main()
{
	// oColor = vec4(texture(tex, iTexCoords).rgb, 1.0);

	// Bilinear sampling of the distance field
    vec3 s = texture(tex, iTexCoords).rgb;

    // Acquire the signed distance
	//remove - 0.5 for outline
    float d = median(s.r, s.g, s.b) - 0.5;

	// fucking intereseting
    // float d = median(s.r, s.g, s.b) - (s.r + s.g + s.b) / 3;

    // Weight between inside and outside (anti-aliasing)
    float w = clamp(d / fwidth(d) + 0.5, 0.0, 1.0);

	//for outline
	// w = smoothstep(0.1, 0.9, w);
	// float w = smoothstep(0.40, 0.6, 0.5 - abs(0.5 - d));

    // Combining the background and foreground color
    // oColor = mix(vec4(fontColor, 1.0), vec4(fontColor, 0.0), 1.0 - alpha);
	oColor = vec4(fontColor, w);
}
