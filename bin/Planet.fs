#version 330 core

uniform sampler2D snowrock;

uniform vec3 lightDirU;

uniform bool toneMapping;
uniform bool gamma;

uniform vec3 cameraPos;
uniform sampler2D heightmap;
uniform sampler2D normalmap;
uniform sampler2D tangentmap;

uniform float average;

uniform vec3 patchTangent;

uniform vec2 regionPosition;
uniform float regionSize;

uniform int normalsFilter;

in vec3 Position;
in vec2 TexCoords;
in vec2 QuadSpacePosition;
in vec3 Normal;
in float UseTangentMap;
in float DebugHeight;

out vec4 Color;

#include "Shaders/Atmosphere.glsl"

float sampleHeightMap( vec2 texCoords, vec2 offset )
{
	const float pixelSize = 1.0 / ( 33.0 * 64.0 );
	return texture( heightmap, texCoords + offset * pixelSize + pixelSize * 0.5 ).r;
}

vec3 sampleNormalMap( vec2 texCoords, vec2 offset )
{
	const float pixelSize = 1.0 / ( 33.0 * 64.0 );
	return texture( normalmap, texCoords + offset * pixelSize + pixelSize * 0.5 ).rgb;
}

vec3 sampleTangentMap( vec2 texCoords, vec2 offset )
{
	const float pixelSize = 1.0 / ( 33.0 * 64.0 );
	return texture( tangentmap, texCoords + offset * pixelSize + pixelSize * 0.5 ).rgb;
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

float hermiteH( float a, float b, float c, float d, float t )
{
	float t2 = t * t;
	float t3 = t2 * t;

	float a2 = -a / 2.0 + ( 3.0 * b ) / 2.0 - ( 3.0 * c ) / 2.0 + d / 2.0;
	float b2 = a - ( 5.0 * b ) / 2.0 + 2.0 * c - d / 2.0;
	float c2 = -a / 2.0 + c / 2.0;
	float d2 = b;

	return a2 * t3 + b2 * t2 + c2 * t + d2;
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

vec3 bicubicT( vec2 texCoords, vec2 t )
{
	vec3 C00 = sampleTangentMap( texCoords, vec2( -1.0, -1.0 ) );
	vec3 C10 = sampleTangentMap( texCoords, vec2( 0.0, -1.0 ) );
	vec3 C20 = sampleTangentMap( texCoords, vec2( 1.0, -1.0 ) );
	vec3 C30 = sampleTangentMap( texCoords, vec2( 2.0, -1.0 ) );

	vec3 C01 = sampleTangentMap( texCoords, vec2( -1.0, 0.0 ) );
	vec3 C11 = sampleTangentMap( texCoords, vec2( 0.0, 0.0 ) );
	vec3 C21 = sampleTangentMap( texCoords, vec2( 1.0, 0.0 ) );
	vec3 C31 = sampleTangentMap( texCoords, vec2( 2.0, 0.0 ) );

	vec3 C02 = sampleTangentMap( texCoords, vec2( -1.0, 1.0 ) );
	vec3 C12 = sampleTangentMap( texCoords, vec2( 0.0, 1.0 ) );
	vec3 C22 = sampleTangentMap( texCoords, vec2( 1.0, 1.0 ) );
	vec3 C32 = sampleTangentMap( texCoords, vec2( 2.0, 1.0 ) );

	vec3 C03 = sampleTangentMap( texCoords, vec2( -1.0, 2.0 ) );
	vec3 C13 = sampleTangentMap( texCoords, vec2( 0.0, 2.0 ) );
	vec3 C23 = sampleTangentMap( texCoords, vec2( 1.0, 2.0 ) );
	vec3 C33 = sampleTangentMap( texCoords, vec2( 2.0, 2.0 ) );

	vec3 CP0X = hermite( C00, C10, C20, C30, t.x );
	vec3 CP1X = hermite( C01, C11, C21, C31, t.x );
	vec3 CP2X = hermite( C02, C12, C22, C32, t.x );
	vec3 CP3X = hermite( C03, C13, C23, C33, t.x );

	return normalize( hermite( CP0X, CP1X, CP2X, CP3X, t.y ) );
}

float bicubicH( vec2 texCoords, vec2 t )
{
	float C00 = sampleHeightMap( texCoords, vec2( -1.0, -1.0 ) );
	float C10 = sampleHeightMap( texCoords, vec2( 0.0, -1.0 ) );
	float C20 = sampleHeightMap( texCoords, vec2( 1.0, -1.0 ) );
	float C30 = sampleHeightMap( texCoords, vec2( 2.0, -1.0 ) );

	float C01 = sampleHeightMap( texCoords, vec2( -1.0, 0.0 ) );
	float C11 = sampleHeightMap( texCoords, vec2( 0.0, 0.0 ) );
	float C21 = sampleHeightMap( texCoords, vec2( 1.0, 0.0 ) );
	float C31 = sampleHeightMap( texCoords, vec2( 2.0, 0.0 ) );

	float C02 = sampleHeightMap( texCoords, vec2( -1.0, 1.0 ) );
	float C12 = sampleHeightMap( texCoords, vec2( 0.0, 1.0 ) );
	float C22 = sampleHeightMap( texCoords, vec2( 1.0, 1.0 ) );
	float C32 = sampleHeightMap( texCoords, vec2( 2.0, 1.0 ) );

	float C03 = sampleHeightMap( texCoords, vec2( -1.0, 2.0 ) );
	float C13 = sampleHeightMap( texCoords, vec2( 0.0, 2.0 ) );
	float C23 = sampleHeightMap( texCoords, vec2( 1.0, 2.0 ) );
	float C33 = sampleHeightMap( texCoords, vec2( 2.0, 2.0 ) );

	float CP0X = hermiteH( C00, C10, C20, C30, t.x );
	float CP1X = hermiteH( C01, C11, C21, C31, t.x );
	float CP2X = hermiteH( C02, C12, C22, C32, t.x );
	float CP3X = hermiteH( C03, C13, C23, C33, t.x );

	return hermiteH( CP0X, CP1X, CP2X, CP3X, t.y );
}

vec3 bilinear( vec2 texCoords, vec2 t )
{
	vec3 a = mix( sampleNormalMap( texCoords, vec2( 0.0, 0.0 ) ), sampleNormalMap( texCoords, vec2( 1.0, 0.0 ) ), t.x );
	vec3 b = mix( sampleNormalMap( texCoords, vec2( 0.0, 1.0 ) ), sampleNormalMap( texCoords, vec2( 1.0, 1.0 ) ), t.x );

	return normalize( mix( normalize( a ), normalize( b ), t.y ) );
}

vec3 bilinearT( vec2 texCoords, vec2 t )
{
	vec3 a = mix( sampleTangentMap( texCoords, vec2( 0.0, 0.0 ) ), sampleTangentMap( texCoords, vec2( 1.0, 0.0 ) ), t.x );
	vec3 b = mix( sampleTangentMap( texCoords, vec2( 0.0, 1.0 ) ), sampleTangentMap( texCoords, vec2( 1.0, 1.0 ) ), t.x );

	return normalize( mix( normalize( a ), normalize( b ), t.y ) );
}

vec3 getNormal()
{
	vec2 texCoords1 = floor( QuadSpacePosition ) / 33.0;
	vec2 texCoords = regionPosition + vec2( texCoords1.x, texCoords1.y ) * regionSize;
	vec2 t = fract( QuadSpacePosition );

	//if( normalsFilter == 2 )
		return bicubic( texCoords, t );
	/*else if( normalsFilter == 1 )
		return bilinear( texCoords, t );
	else*/
	//	return sampleNormalMap( texCoords, vec2( 0.0 ) );
}

vec3 getTangent()
{
	if( UseTangentMap > 0.5 )
	{
		vec2 texCoords1 = floor( QuadSpacePosition ) / 33.0;
		vec2 texCoords = regionPosition + vec2( texCoords1.x, texCoords1.y ) * regionSize;
		vec2 t = fract( QuadSpacePosition );

		return bicubicT( texCoords, vec2( 0.0 ) );
	}
	else
		return patchTangent;
}

/*vec3 getColor()
{
	vec3 DeepWater = vec3( 0.0, 0.0, 0.5 );
	vec3 ShallowWater = ivec3( 25, 25, 150 ) / 255.0;
	vec3 Sand = ivec3( 240, 240, 64 ) / 255.0;
	vec3 Grass = ivec3( 50, 200, 20 ) / 255.0;
	vec3 Forest = ivec3( 16, 160, 0 ) / 255.0;
	vec3 Rock = vec3( 0.5 );
	vec3 Snow = vec3( 1.0 );


	float height = DebugHeight / ( 500000.0 );

	if( height < 0.2 )
		return DeepWater;
	else if( height < 0.4 )
		return ShallowWater;
	else if( height < 0.5 )
		return Sand;
	else if( height < 0.7 )
		return Grass;
	else if( height < 0.8 )
		return Forest;
	else if( height < 0.9 )
		return Rock;
	else
		return Snow;
}*/

vec3 getColor()
{
	return vec3( 0.0 );
}

vec3 getGroundColor()
{
	return vec3( 0.3, 0.8, 0.3 );
	//return texture( snowrock, TexCoords ).rgb;
}

void main()
{
	vec3 normal = normalize( Normal );
	vec3 tangent = getTangent();

	mat3 tbn = mat3( tangent, normalize( cross( tangent, normal ) ), normal );

	normal = normalize( tbn * getNormal() );

	float diffuse = max( dot( normal, normalize( lightDirU ) ), 0.0 );

	//vec3 color = vec3( diffuse );//getColor() * diffuse;

	//vec3 t = exp( -( rayleighExtinctionAtSeaLevel * opticalDepthR + mieExtinctionAtSeaLevel * opticalDepthM ) );
	float opticalDepthR, opticalDepthM;
	vec3 skyColor = getSkyColor2( lightDirU, opticalDepthR, opticalDepthM );
	//skyColor = vec3( 1.0 ) - exp( -skyColor );

	vec3 t = exp( -( rayleighExtinctionAtSeaLevel * opticalDepthR + mieExtinctionAtSeaLevel * opticalDepthM ) );

	vec3 color = getGroundColor() * diffuse * t + skyColor;

	if( toneMapping )
		color = vec3( 1.0 ) - exp( -color );
	
	if( gamma )
		color = pow( color, vec3( 1.0 / 2.2 ) );

	if( gamma )
		Color = vec4( color, 1.0 );
	else
		Color = vec4( vec3( normal.x, 0.0, normal.z ), 1.0 );
	//else
	//	Color = vec4( getNormal, 1.0 );
}