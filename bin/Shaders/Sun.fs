/*#version 330 core

uniform sampler2D intensity;
uniform samplerCube skybox;

uniform vec3 lightDirU;
uniform vec3 cameraPos;

uniform bool toneMapping;
uniform bool gamma;

in vec3 Position;
in vec2 TexCoords;

out vec4 Color;

vec3 getColor( vec3 viewDir )
{
	vec3 background = pow( texture( skybox, viewDir ).rgb, vec3( 2.2 ) );
	float f = pow( texture( intensity, TexCoords ).r, 2.2 );

	return mix( background, vec3( 1.0 ), f );
}

void main()
{
	vec3 viewDir = normalize( Position - cameraPos );

	float opticalDepthR, opticalDepthM;

	vec3 start = cameraPos;

	if( length( cameraPos ) > atmosphereRadius )
		start = getNearestVertex( start, viewDir );
	
	vec3 end = getFarthestVertex( cameraPos, viewDir );

	vec3 skyColor = getSkyColor( start, end, lightDirU, viewDir, opticalDepthR, opticalDepthM );

	vec3 t = exp( -( rayleighExtinctionAtSeaLevel * opticalDepthR + mieExtinctionAtSeaLevel * opticalDepthM ) );

	vec3 color = getColor( viewDir ) * t + skyColor;

	if( toneMapping )
		color = vec3( 1.0 ) - exp( -color );
	
	if( gamma )
		color = pow( color, vec3( 1.0 / 2.2 ) );

	Color = vec4( viewDir, 1.0 );
}*/

#version 330 core

uniform sampler2D starGlow;

in vec2 TexCoords;

out vec4 Color;

void main()
{
	vec3 color = vec3( pow( texture( starGlow, TexCoords ).r * 2.0, 2.2 ) );
	Color = vec4( color, 1.0 );
}