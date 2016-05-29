#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

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

	gl_Position = positions[gl_VertexIndex];
}
