#version 400

uniform mat4 WorldViewProjection;
uniform mat4 ProjectionMatrix;

in vec4 Position;
in vec2 TextureCoordinate;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
} OUT;

void main()
{	
	gl_Position = ProjectionMatrix * WorldViewProjection * Position;
	OUT.TextureCoordinate = TextureCoordinate;
}