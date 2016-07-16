#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec2 iTexCoords;
layout(location = 0) out vec4 oColor;

layout(set = 0, binding = 0) uniform sampler2D uTex;
const vec3 fontColor = vec3(0.0, 0.0, 0.0);

float median(float a, float b, float c)
{
	return max(min(a,b), min(max(a,b),c));
}

void main()
{
	// oColor = vec4(1.0, 0.0, 0.0, 1.0);

	// float dist = texture(uTex, iTexCoords).r;
	// float alpha = smoothstep(0.3, 0.4, dist);
	// //uncomment this line to outline instead of fill
	// // alpha = smoothstep(0.0, 0.3, 0.5 - abs(0.5 - alpha));
	// oColor = vec4(fontColor, alpha);

	// Bilinear sampling of the distance field
    vec3 s = texture(uTex, iTexCoords).rgb;
    // Acquire the signed distance
    float d = median(s.r, s.g, s.b) - 0.5;
    // Weight between inside and outside (anti-aliasing)
    float w = clamp(d / fwidth(d) + 0.5, 0.0, 1.0);
	// float w = d + 0.5;
    // float w = smoothstep(0.4, 0.6, d + 0.5);
    // Combining the background and foreground color
	oColor = vec4(fontColor, w);
}
