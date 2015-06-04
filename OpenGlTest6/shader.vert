#version 400

uniform mat4 WorldViewProjection;
uniform mat4 ProjectionMatrix;

in vec4 Position;
in vec4 Color;

out VS_OUTPUT
{
	vec4 Color;
} OUT;

void main()
{	
	gl_Position = ProjectionMatrix * WorldViewProjection * Position;
	OUT.Color = Color;
}

