#version 400

in vec2 position;

uniform sampler2D DiffuseTexture; 
uniform sampler2D PositionTexture;
uniform sampler2D NormalsTexture;
uniform vec4 AmbientColor;
uniform vec4 LightColor;
uniform vec3 CameraPosition;
uniform vec4 SpecularColor;
uniform float SpecularPower;
uniform vec3 LightDirection;

out vec4 Color;

void main( void )
{
	vec4 SampledColor = texture( DiffuseTexture, position.xy );
	vec3 WorldPosition = texture( PositionTexture, position.xy ).rgb;
	vec3 Normal = normalize( texture( NormalsTexture, position.xy ).xyz );

	vec3 LightDirection = -normalize(LightDirection);
	vec3 ViewDirection = normalize(CameraPosition + WorldPosition);
	
	float n_dot_l = dot(LightDirection, Normal);
	vec3 halfVector = normalize(LightDirection + ViewDirection);
	float n_dot_h = clamp(dot(Normal, halfVector), 0.0f, 1.0f);

	vec3 ambient = SampledColor.rgb * AmbientColor.rgb;
	vec3 diffuse = clamp(LightColor.rgb * n_dot_l * SampledColor.rgb, 0.0f, 1.0f);

	vec3 specular = SpecularColor.rgb * min(pow(n_dot_h, SpecularPower), SampledColor.w);

	Color.rgb = ambient + diffuse + specular;
	Color.a = SampledColor.a;
}
