#version 440
uniform vec4 colour_override;
layout (location = 0) out vec4 out_colour;

void main()
{
	out_colour = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}