#version 400

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 WorldMatrix;

in vec4 Position;
in vec2 TextureCoordinate;
in vec3 Normal;

out VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
} OUT;

void main()
{	
	vec4 WorldPosition = WorldMatrix * Position;
	gl_Position = ProjectionMatrix * ViewMatrix * WorldPosition;
	OUT.TextureCoordinate = TextureCoordinate;
	OUT.Normal = (WorldMatrix * vec4(Normal, 0.0f)).xyz;
	OUT.WorldPosition = WorldPosition.xyz;
}