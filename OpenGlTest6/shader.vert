#version 400

uniform mat4 WorldViewProjection;

in vec4 Position;
in vec4 Color;

out VS_OUTPUT
{
	vec4 Color;
} OUT;

void main()
{	
	gl_Position = WorldViewProjection * Position;
	OUT.Color = Color;
}

