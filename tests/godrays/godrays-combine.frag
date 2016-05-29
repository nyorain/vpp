#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

//input uniform vars
layout (set = 0, binding = 0) uniform UBO
{
	vec2 lightPosition;
	float exposure;
	uint samples;
} ubo;

//stencil buffer of scene uniform input
layout (set = 0, binding = 1) uniform sampler2D stencil;
layout (input_attachment_index = 0, set = 0, binding = 2) uniform subpassInput color;

//resulting output color
layout (location = 0) out vec4 outFragColor;

void main()
{
	vec2 texCoord = gl_FragCoord.xy;
	vec2 delta = ubo.lightPosition - texCoord;
	delta /= ubo.samples;

	float colorFac = texture(stencil, texCoord).r;
	float decay = 1.f;

	for(uint i = 0; i < ubo.samples; ++i)
	{
		texCoord += delta;
		colorFac += texture(stencil, texCoord).r * decay / ubo.samples;
		decay *= 0.95;
	}

	vec4 fragColor = subpassLoad(color);
	//outFragColor = ubo.exposure * fragColor + clamp(colorFac, 0.0, 1.0) * vec4(1.0, 1.0, 1.0, 1.0);
	outFragColor = texture(stencil, gl_FragCoord.xy).rrrr;
	//outFragColor = fragColor;
}
