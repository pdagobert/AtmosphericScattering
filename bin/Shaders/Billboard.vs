#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;

uniform mat4 vp;
uniform mat4 view;
uniform float far;

uniform vec3 center;
uniform float radius;

out vec3 Position;
out vec2 TexCoords;

void main()
{
	vec3 right = vec3( view[ 0 ][ 0 ], view[ 1 ][ 0 ], view[ 2 ][ 0 ] );
	vec3 up = vec3( view[ 0 ][ 1 ], view[ 1 ][ 1 ], view[ 2 ][ 1 ] );

	vec3 pos = center + right * position.x * radius + up * position.y * radius;
	gl_Position = vp * vec4( pos, 1.0 );

	float invFarPlane = 2.0 / ( log2( far + 1 ) );
	gl_Position.z = log2( max( 1e-6, 1.0 + gl_Position.w ) ) * invFarPlane - 1.0;
	gl_Position.z *= gl_Position.w;

	Position = pos;
	TexCoords = texCoords;
}