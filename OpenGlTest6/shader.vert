#version 400

uniform mat4 WorldViewProjection;
uniform mat4 ProjectionMatrix;
uniform vec3 LightDirection;
uniform mat4 World;

in vec4 Position;
in vec2 TextureCoordinate;
in vec3 Normal;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 LightDirection;
} OUT;

void main()
{	
	gl_Position = ProjectionMatrix * WorldViewProjection * Position;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.Normal = (World * vec4(Normal, 0.0f)).xyz;
	OUT.LightDirection = -LightDirection;
}