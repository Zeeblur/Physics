#version 410

// The model matrix
uniform mat4 M;
// The transformation matrix
uniform mat4 MVP;
// The normal matrix
uniform mat3 N;

// Incoming position
layout (location = 0) in vec3 position;
// Incoming normal
layout (location = 1) in vec3 normal;

// Outgoing position
layout (location = 0) out vec3 vertex_position;
// Outgoing normal
layout (location = 1) out vec3 transformed_normal;

void main()
{
	// Set position
	gl_Position = MVP * vec4(position, 1);
	// **************************************
	// Output other values to fragment shader
	// **************************************
	vertex_position = (M * vec4(position, 1)).xyz;
	transformed_normal = N * normal;
}