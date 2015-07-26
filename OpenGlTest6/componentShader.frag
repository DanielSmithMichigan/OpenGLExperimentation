#version 400

uniform sampler2D ColorTextureSampler;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 WorldPosition;
} IN;

out vec4 Diffuse; 
out vec4 Position;
out vec4 Normal;

void main()
{	
	Diffuse = vec4(texture(ColorTextureSampler, IN.TextureCoordinate).rgb, 0);
	Position = vec4(IN.WorldPosition.xyz,0);
	Normal = vec4(IN.Normal.xyz,0);
}
