#version 330 core

uniform vec2 position;
uniform float size;

in vec2 TexCoords;

out float Height;

#include "Noise/noise3D.glsl"

void main()
{
	vec2 samplePos = position + TexCoords * size;	
	 
	Height = snoise( vec3( samplePos * 0.01, 0.0 ) );
	//Height = 0.0;
}