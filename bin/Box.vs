#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform mat4 world;
uniform float far;

void main()
{
	gl_Position = vp * world * vec4( position, 1.0 );

	float invFarPlane = 2.0 / ( log( far + 1 ) );
	gl_Position.z = ( log( 1 + gl_Position.w ) * invFarPlane - 1 ) * gl_Position.w;
}