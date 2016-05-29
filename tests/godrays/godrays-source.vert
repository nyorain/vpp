#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (set = 0, location = 0) in vec2 position;

void main()
{
	gl_Position = vec4(position, 0, 1);
	return;

	//positions are constant
	//could also be done with if/else (more efficient?)
	const vec4 positions[6] = vec4[6](
		vec4(-0.5, -0.5, 0, 0.5),
		vec4( 0.5, -0.5, 0, 0.5),
		vec4(-0.5,  0.5, 0, 0.5),

		vec4( 0.5,  0.5, 0, 0.5),
		vec4( -0.5,  0.5, 0, 0.5),
		vec4( 0.5,  -0.5, 0, 0.5)
	);

	gl_Position = positions[gl_VertexIndex];
}
