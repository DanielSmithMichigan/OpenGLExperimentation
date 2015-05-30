#version 400

in vec4 Position;
in vec4 Color;

out VS_OUTPUT
{
	vec4 Color;
} OUT;

void main()
{	
	gl_Position = Position;
	OUT.Color = Color;
}