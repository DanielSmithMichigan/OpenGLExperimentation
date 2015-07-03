#version 400

uniform sampler2D ColorTextureSampler;
uniform vec4 AmbientColor;
uniform vec4 LightColor;
uniform vec3 CameraPosition;
uniform vec4 SpecularColor;
uniform float SpecularPower;

in VS_OUTPUT
{
	vec2 TextureCoordinate;
	vec3 Normal;
	vec3 LightDirection;
	vec3 WorldPosition;
} IN;

out vec4 Color;

void main()
{
	vec3 lightDirection = normalize(IN.LightDirection);
	vec3 viewDirection = normalize(CameraPosition + IN.WorldPosition);
	
	vec3 normal = normalize(IN.Normal);
	float n_dot_l = dot(lightDirection, normal);
	vec3 halfVector = normalize(lightDirection + viewDirection);
	float n_dot_h = clamp(dot(normal, halfVector), 0.0f, 1.0f);

	vec4 sampledColor = texture(ColorTextureSampler, IN.TextureCoordinate);
	vec3 ambient = sampledColor.rgb * AmbientColor.rgb;
	vec3 diffuse = clamp(LightColor.rgb * n_dot_l * sampledColor.rgb, 0.0f, 1.0f);

	vec3 specular = SpecularColor.rgb * min(pow(n_dot_h, SpecularPower), sampledColor.w);

	Color.rgb = ambient + diffuse + specular;
	Color.a = sampledColor.a;
}