#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform mat4 world;
uniform vec3 normal;
uniform vec3 tangent;
uniform float radius;
uniform float far;
uniform mat4 world2;

out float Height;
out vec3 Normal;

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

float getHeight( vec3 normal )
{
	return fbm2( normal, 8, 0.1, 0.8 ) * 1.0;
}

vec3 cubeToSphere( vec3 position )
{
	return normalize( position ) * radius;// + getHeight( normalize( position ) );
}

void main()
{
	/*vec3 pos = vec3( world * vec4( position, 1.0 ) );
	
	vec3 wpos = normal * radius;
	wpos += tangent * pos.x;
	wpos += normalize( cross( normal, tangent ) ) * pos.z;

	wpos = cubeToSphere( wpos );

	gl_Position = vp * vec4( pos, 1.0 );
	//Height = getHeight( normalize( wpos ) );
	Normal = position;//normalize( wpos );
	*/

	vec4 worldPos = world2 * vec4( position, 1.0 );
	vec4 pos = world * vec4( position, 1.0 );

	float h = getHeight( worldPos.xyz );

	//pos.y += h;

	gl_Position = vp * pos;
	
	float invFarPlane = 2.0 / ( log( far + 1 ) );
	gl_Position.z = ( log( 1 + gl_Position.w ) * invFarPlane - 1 ) * gl_Position.w;

	//gl_Position.z = log2( max( 1e-6, 1.0 + gl_Position.w ) ) * fCoef - 1.0;
	Height = h;
	Normal = position;
}