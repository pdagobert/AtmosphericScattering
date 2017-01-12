#version 330

uniform sampler2D heightmap;
uniform sampler2D normalmap;

in vec2 QuadSpacePosition;

uniform vec2 regionPosition;
uniform float regionSize;

uniform int normalsFilter;

in vec2 DTexCoords;

//in vec3 Normal;

out vec4 Color;

vec3 sampleNormalMap( vec2 texCoords, vec2 offset )
{
	const float pixelSize = 1.0 / ( 33.0 * 64.0 );
	return texture( normalmap, texCoords + offset * pixelSize + pixelSize * 0.5 ).rgb;
}

vec3 hermite( vec3 a, vec3 b, vec3 c, vec3 d, float t )
{
	float t2 = t * t;
	float t3 = t2 * t;

	vec3 a2 = -a / 2.0 + ( 3.0 * b ) / 2.0 - ( 3.0 * c ) / 2.0 + d / 2.0;
	vec3 b2 = a - ( 5.0 * b ) / 2.0 + 2.0 * c - d / 2.0;
	vec3 c2 = -a / 2.0 + c / 2.0;
	vec3 d2 = b;

	return a2 * t3 + b2 * t2 + c2 * t + d2;
}

vec3 testbicubic( vec2 t )
{
	vec3 red = vec3( 1.0, 0.0, 0.0 );
	vec3 green = vec3( 0.0, 1.0, 0.0 );
	vec3 blue = vec3( 0.0, 0.0, 1.0 );
	vec3 purple = vec3( 1.0, 0.0, 1.0 );
	vec3 yellow = vec3( 1.0, 1.0, 0.0 );

	vec3 C00 = red;
	vec3 C10 = blue;
	vec3 C20 = red;
	vec3 C30 = purple;

	vec3 C01 = blue;
	vec3 C11 = red;
	vec3 C21 = purple;
	vec3 C31 = yellow;

	vec3 C02 = blue;
	vec3 C12 = yellow;
	vec3 C22 = blue;
	vec3 C32 = green;

	vec3 C03 = red;
	vec3 C13 = purple;
	vec3 C23 = blue;
	vec3 C33 = purple;

	vec3 CP0X = hermite( C00, C10, C20, C30, t.x );
	vec3 CP1X = hermite( C01, C11, C21, C31, t.x );
	vec3 CP2X = hermite( C02, C12, C22, C32, t.x );
	vec3 CP3X = hermite( C03, C13, C23, C33, t.x );

	return hermite( CP0X, CP1X, CP2X, CP3X, t.y );
}

vec3 bicubic( vec2 texCoords, vec2 t )
{
	vec3 C00 = sampleNormalMap( texCoords, vec2( -1.0, -1.0 ) );
	vec3 C10 = sampleNormalMap( texCoords, vec2( 0.0, -1.0 ) );
	vec3 C20 = sampleNormalMap( texCoords, vec2( 1.0, -1.0 ) );
	vec3 C30 = sampleNormalMap( texCoords, vec2( 2.0, -1.0 ) );

	vec3 C01 = sampleNormalMap( texCoords, vec2( -1.0, 0.0 ) );
	vec3 C11 = sampleNormalMap( texCoords, vec2( 0.0, 0.0 ) );
	vec3 C21 = sampleNormalMap( texCoords, vec2( 1.0, 0.0 ) );
	vec3 C31 = sampleNormalMap( texCoords, vec2( 2.0, 0.0 ) );

	vec3 C02 = sampleNormalMap( texCoords, vec2( -1.0, 1.0 ) );
	vec3 C12 = sampleNormalMap( texCoords, vec2( 0.0, 1.0 ) );
	vec3 C22 = sampleNormalMap( texCoords, vec2( 1.0, 1.0 ) );
	vec3 C32 = sampleNormalMap( texCoords, vec2( 2.0, 1.0 ) );

	vec3 C03 = sampleNormalMap( texCoords, vec2( -1.0, 2.0 ) );
	vec3 C13 = sampleNormalMap( texCoords, vec2( 0.0, 2.0 ) );
	vec3 C23 = sampleNormalMap( texCoords, vec2( 1.0, 2.0 ) );
	vec3 C33 = sampleNormalMap( texCoords, vec2( 2.0, 2.0 ) );

	vec3 CP0X = hermite( C00, C10, C20, C30, t.x );
	vec3 CP1X = hermite( C01, C11, C21, C31, t.x );
	vec3 CP2X = hermite( C02, C12, C22, C32, t.x );
	vec3 CP3X = hermite( C03, C13, C23, C33, t.x );

	return normalize( hermite( CP0X, CP1X, CP2X, CP3X, t.y ) );
}

vec3 bilinear( vec2 texCoords, vec2 t )
{
	vec3 a = mix( sampleNormalMap( texCoords, vec2( 0.0, 0.0 ) ), sampleNormalMap( texCoords, vec2( 1.0, 0.0 ) ), t.x );
	vec3 b = mix( sampleNormalMap( texCoords, vec2( 0.0, 1.0 ) ), sampleNormalMap( texCoords, vec2( 1.0, 1.0 ) ), t.x );

	return normalize( mix( normalize( a ), normalize( b ), t.y ) );
}

vec3 getNormal()
{
	/*int overflowLevel = 0;
	if( ceil( QuadSpacePosition.x ) > 31.0 || ceil( QuadSpacePosition.y ) > 31.0 )
	{
		overflowLevel = 2;
		return vec3( 0.2 );
	}
	else if( ceil( QuadSpacePosition.x ) > 30.0 || ceil( QuadSpacePosition.y ) > 30.0 )
	{
		overflowLevel = 1;
		return vec3( 0.4 );
	}*/

	vec2 texCoords1 = floor( QuadSpacePosition ) / 33.0;

	vec2 texCoords = regionPosition + vec2( texCoords1.x, texCoords1.y ) * regionSize;

	vec2 t = fract( QuadSpacePosition );

	//vec2 t4test = QuadSpacePosition / 33.0;
	//return testbicubic( t4test );

	if( normalsFilter == 2 )
		return bicubic( texCoords, t );
	else if( normalsFilter == 1 )
		return bilinear( texCoords, t );
	else
		return sampleNormalMap( texCoords, vec2( 0.0 ) );
}

void main()
{
	vec3 lightDir = vec3( 1.0, 3.0, 1.0 );
	lightDir = normalize( lightDir );

	vec3 normal = getNormal();

	float diffuse = max( dot( lightDir, normal ), 0.0 );
	/*if( TexCoords.x > 0.99 )
		Color = vec4( 1.0 );
	else*/
	//Color = vec4( vec3( diffuse ), 1.0 );
	Color = vec4( 1.0 );
}