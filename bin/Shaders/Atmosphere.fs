#version 330 core

uniform vec3 lightDirU;
uniform vec3 cameraPos;
uniform float far;

uniform bool toneMapping;
uniform bool gamma;

in vec3 Position;
in float flogz;

out vec4 Color;

#include "Atmosphere.glsl"

float test( float value )
{
	if( value < 1.413 )
		return pow( value * 0.38317, 1.0 / 2.2 );
	else
		return 1.0 - exp( -value );
}

vec3 hdr( vec3 color )
{
	color.r = test( color.r );
	color.g = test( color.g );
	color.b = test( color.b );

	return color;
}
	
const float pattern[] = float[](
    0.0, 32.0,  8.0, 40.0,  2.0, 34.0, 10.0, 42.0,   /* 8x8 Bayer ordered dithering  */
    48.0, 16.0, 56.0, 24.0, 50.0, 18.0, 58.0, 26.0,  /* pattern.  Each input pixel   */
    12.0, 44.0,  4.0, 36.0, 14.0, 46.0,  6.0, 38.0,  /* is scaled to the 0..63 range */
    60.0, 28.0, 52.0, 20.0, 62.0, 30.0, 54.0, 22.0,  /* before looking in this table */
    3.0, 35.0, 11.0, 43.0,  1.0, 33.0,  9.0, 41.0,   /* to determine the action.     */
    51.0, 19.0, 59.0, 27.0, 49.0, 17.0, 57.0, 25.0,
    15.0, 47.0,  7.0, 39.0, 13.0, 45.0,  5.0, 37.0,
    63.0, 31.0, 55.0, 23.0, 61.0, 29.0, 53.0, 21.0 );

void main()
{
	float invFarPlane = 2.0 / log2( far + 1 );
	gl_FragDepth = log2( flogz ) * invFarPlane * 0.5;

	vec3 skyColor = getSkyColor( lightDirU );

	//skyColor = hdr( skyColor );

	int index = int( gl_FragCoord.x / 8.0 ) + 8 * int( gl_FragCoord.y / 8.0 );
	float dither = pattern[ index ];

	//skyColor += vec3( dither / 32.0 - ( 1.0 / 128.0 ) );

	if( toneMapping )
		skyColor = vec3( 1.0 ) - exp( -skyColor );
	
	if( gamma )
		skyColor = pow( skyColor, vec3( 1.0 / 2.2 ) );

	Color = vec4( skyColor, 1.0 );
}