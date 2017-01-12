#version 330 core

uniform sampler2D diffuseMap;

uniform vec3 cameraPos;
uniform vec3 lightDirU;

uniform bool toneMapping;
uniform bool gamma;

in vec3 Position;
in vec2 TexCoords;
in vec3 Normal;

out vec4 Color;

#include "Shaders/Atmosphere.glsl"

void main()
{
	vec3 normal = normalize( Normal );
	float diffuse = max( dot( normal, normalize( lightDirU ) ), 0.0 );

	float opticalDepthR, opticalDepthM;
	vec3 skyColor = getSkyColor2( lightDirU, opticalDepthR, opticalDepthM );

	vec3 t = exp( -( rayleighExtinctionAtSeaLevel * opticalDepthR + mieExtinctionAtSeaLevel * opticalDepthM ) );

	vec3 color = pow( texture( diffuseMap, TexCoords ).rgb, vec3( 2.2 ) ) * diffuse * t + skyColor;


	if( toneMapping )
		color = vec3( 1.0 ) - exp( -color );
	
	//if( gamma )
		color = pow( color, vec3( 1.0 / 2.2 ) );
	
	Color = vec4( color, 1.0 );
}