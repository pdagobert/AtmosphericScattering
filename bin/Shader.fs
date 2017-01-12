#version 330 core

uniform float mixValue;

out vec4 Color;

in float Height;
in vec3 Normal;

#include "Shaders/Noise/noise3D.glsl"

float fbm( vec3 vec, int octaves, float frequency, float persistence )
{
	float value = 0.0;
	float amplitude = 1.0;
	
	float maxAmplitude = 0.0;
	
	for( int i = 0; i < octaves; i++ )
	{
		value += snoise( vec * frequency ) * amplitude;
		frequency *= 2.0;
		
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}
	
	return value / maxAmplitude;
}

float fbm2( vec3 vec, int octaves, float frequency, float persistence )
{
	float value = 0.0;
	float amplitude = 1.0;
	
	float maxAmplitude = 0.0;
	
	for( int i = 0; i < octaves; i++ )
	{
		float n = ( 1.0 - abs( snoise( vec * frequency ) ) ) * 2.0 - 1.0;
		value += n * amplitude;
		frequency *= 2.0;
		
		maxAmplitude += amplitude;
		amplitude *= persistence;
	}
	
	return value / maxAmplitude;
}

void main()
{
	//Color = vec4( vec3( fbm( Normal, 8, 1.0, 0.8 ) ), 1.0 );
	//Color = vec4( vec3( Height / 1000.0 ), 1.0 );

	vec3 offset = vec3( 2.0, 7.0, 2.0 );
	vec3 offset2 = vec3( 46.0, -15.0, 11.0 );

	float n = fbm2( offset + Normal, 8, 1.0, 0.8 );
	float n2 = fbm2( offset2 + Normal, 8, 1.0, 0.8 );

	float nf = mix( n, n2, mixValue );

	Color = vec4( nf );
}