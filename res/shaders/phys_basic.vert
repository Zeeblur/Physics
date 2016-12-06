#version 440
uniform mat4 MVP;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normals;

layout (location = 0) out vec3 vertex_position;
//layout (location = 1) out vec3 transformed_normal;

void main()
{
	gl_Position = MVP * vec4(position, 1.0);
}
