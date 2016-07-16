#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) out vec2 oTexCoords;

void main()
{
	//positions are constant
	//could also be done with if/else (more efficient?)
	const vec4 positions[6] = vec4[6](
		vec4(-1, -1, 0, 1),
		vec4( 1, -1, 0, 1),
		vec4(-1,  1, 0, 1),

		vec4( 1,  1, 0, 1),
		vec4( -1,  1, 0, 1),
		vec4( 1,  -1, 0, 1)
	);

	const vec2 texCoords[6] = vec2[6](
		vec2(0, 0),
		vec2(1, 0),
		vec2(0, 1),

		vec2(1, 1),
		vec2(0, 1),
		vec2(1, 0)
	);

	gl_Position = positions[gl_VertexIndex];

	//font inversion
	// oTexCoords = vec2(0, 1) - vec2(-1, 1) * texCoords[gl_VertexIndex];

	oTexCoords = texCoords[gl_VertexIndex];
}
