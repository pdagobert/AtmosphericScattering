#version 330

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform mat4 relativeWorld;

uniform float far;

///uniform vec2 regionPosition;
//uniform float regionSize;

uniform float radius;

uniform vec3 corners[ 4 ];
uniform vec3 normals[ 4 ];
uniform bool interpolateCorners;

vec3 getInterpolatedPos( vec2 uv )
{
	vec3 bot = mix( corners[ 0 ], corners[ 1 ], uv.x );
	vec3 top = mix( corners[ 2 ], corners[ 3 ], uv.x );

	return mix( bot, top, uv.y );
}

vec3 getInterpolatedNormal( vec2 uv )
{
	vec3 bot = normalize( mix( normals[ 0 ], normals[ 1 ], uv.x ) );
	vec3 top = normalize( mix( normals[ 2 ], normals[ 3 ], uv.x ) );

	return normalize( mix( bot, top, uv.y ) );
}

void main()
{
	/*vec2 texCoords = vec2( position.x, -position.z ) + 16.0;
	QuadSpacePosition = texCoords;
	texCoords = ( texCoords + 0.5 ) / 33.0;

	vec2 TexCoords = regionPosition + vec2( texCoords.x, texCoords.y ) * regionSize;*/

	vec2 uv = position.xz / 16.0 * 0.5 + vec2( 0.5 );

	if( interpolateCorners )
	{
		vec3 pos = getInterpolatedPos( uv );
		gl_Position = vp * vec4( pos, 1.0 );
	}
	else
	{
		vec3 normal = normalize( getInterpolatedNormal( uv ) );
		
		vec3 pos = normal * radius;
		gl_Position = vp * relativeWorld * vec4( pos, 1.0 );
	}

	float invFarPlane = 2.0 / ( log( far + 1 ) );
	gl_Position.z = ( log( 1 + gl_Position.w ) * invFarPlane - 1 ) * gl_Position.w;
}