#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform float far;

uniform float radius;
uniform mat4 relativeWorld;

uniform vec3 corners[ 5 ];
uniform vec3 normals[ 5 ];
uniform bool interpolateCorners;

uniform vec3 patchNormal;
uniform float average;

uniform vec2 patchPosition;
uniform float patchSize;

uniform vec3 faceNormal;
uniform vec3 faceTangent;

uniform vec2 regionPosition;
uniform float regionSize;

uniform sampler2D heightmap;

out vec3 Position;
out vec2 TexCoords;
out vec2 QuadSpacePosition;
out vec3 Normal;
out float UseTangentMap;
out float DebugHeight;

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

vec3 getNormal()
{
	vec2 p = vec2( position.x, -position.z ) + vec2( 16.0 );
	
	vec2 pos = ( patchPosition - patchSize / 2.0 ) + p / 32.0 * patchSize;

	vec3 normal = faceNormal * radius;
	normal += faceTangent * pos.x;
	normal += normalize( cross( faceNormal, faceTangent ) ) * pos.y;

	return normalize( normal );
}

void main()
{
	vec2 uv = position.xz / 16.0 * 0.5 + vec2( 0.5 );

	vec2 heightmapUV = vec2( position.x, -position.z ) + 16.0;
	QuadSpacePosition = heightmapUV;

	heightmapUV = ( heightmapUV + 0.5 ) / 33.0;

	TexCoords = heightmapUV;
	heightmapUV = regionPosition + heightmapUV * regionSize;

	float height = texture( heightmap, heightmapUV ).r;

	if( interpolateCorners )
	{
		vec3 pos = getInterpolatedPos( uv );
		pos += patchNormal * height;


		if( position.y < 0.0 )
		{
			pos -= patchNormal * ( abs( height ) * 2.0 );
		}
			
		//else
		//	pos += patchNormal * ( average + height ) position.y );
		gl_Position = vp * vec4( pos, 1.0 );

		Normal = patchNormal;
		UseTangentMap = 0.0;
	}
	else
	{
		vec3 normal = getNormal();//normalize( getInterpolatedNormal( uv ) );
		
		vec3 pos = normal * ( radius + ( average + height ) * position.y );
		gl_Position = vp * relativeWorld * vec4( pos, 1.0 );

		Normal = normal;
		UseTangentMap = 1.0;
	}

	float invFarPlane = 2.0 / ( log( far + 1 ) );
	gl_Position.z = ( log( 1 + gl_Position.w ) * invFarPlane - 1 ) * gl_Position.w;

	Position = getNormal() * ( radius + average + height );
	DebugHeight = average + height;
}