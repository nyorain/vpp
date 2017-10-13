#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 outColor;

struct Value {
	vec2 pos;
	vec4 color;
};

const Value[] values = {
	{{-0.75, -.75}, {.9, .3, .3, 1.0}},
	{{0, .75}, {.3, .9, .3, 1.0}},
	{{.75, -.75}, {.3, .3, .9, 1.0}},
};

void main()
{
	gl_Position = vec4(values[gl_VertexIndex].pos, 0.0, 1.0);
	gl_Position.y *= -1;
	outColor = values[gl_VertexIndex].color;
}
