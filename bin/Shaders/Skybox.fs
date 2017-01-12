#version 330 core

uniform samplerCube skybox;

uniform vec3 lightDirU;
uniform vec3 cameraPos;

uniform bool toneMapping;
uniform bool gamma;

in vec3 Position;
in vec3 TexCoords;

out vec4 Color;

#include "Atmosphere.glsl"

void main()
{
	vec3 texCoords = normalize( TexCoords );
	vec3 textureColor = pow( texture( skybox, texCoords ).rgb, vec3( 1.0 ) );

	/*if( !intersectsWithAtmosphere( cameraPos, texCoords ) )
	{
		Color = vec4( pow( textureColor, vec3( 1.0 / 2.2 ) ), 1.0 );
		return;
	}*/

	float opticalDepthR, opticalDepthM;

	vec3 start = cameraPos;

	if( length( cameraPos ) > atmosphereRadius )
		start = getNearestVertex( start, texCoords );
	
	vec3 end = getFarthestVertex( cameraPos, texCoords );

	vec3 skyColor = getSkyColor( start, end, lightDirU, texCoords, opticalDepthR, opticalDepthM );

	vec3 t = exp( -( rayleighExtinctionAtSeaLevel * opticalDepthR + mieExtinctionAtSeaLevel * opticalDepthM ) );

	vec3 color = textureColor * t + skyColor;

	if( toneMapping )
		color = vec3( 1.0 ) - exp( -color * 1.0 );
	
	if( gamma )
		color = pow( color, vec3( 1.0 / 2.2 ) );

	Color = vec4( color, 1.0 );
}