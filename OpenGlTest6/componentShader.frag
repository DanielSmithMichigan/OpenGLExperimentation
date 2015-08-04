#version 400

uniform sampler2D ColorTextureSampler;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
} IN;

out vec4 Diffuse; 
out vec3 Position;
out vec3 Normal;

void main()
{	
	Diffuse = vec4(texture(ColorTextureSampler, IN.TextureCoordinate).rgb, 0);
	Position = vec3(IN.WorldPosition.xyz);
	Normal = vec3(IN.Normal.xyz);
}
