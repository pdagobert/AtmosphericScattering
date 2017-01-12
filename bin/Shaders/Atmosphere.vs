#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform mat4 world;
uniform float far;

uniform float atmosphereRadius;

out vec3 Position;
out float flogz;

void main()
{
	gl_Position = vp * world * vec4( position, 1.0 );

	float invFarPlane = 2.0 / ( log2( far + 1 ) );
	gl_Position.z = log2( max( 1e-6, 1.0 + gl_Position.w ) ) * invFarPlane - 1.0;
	gl_Position.z *= gl_Position.w;

	Position = position.xyz * atmosphereRadius;
	flogz = 1.0 + gl_Position.w;
}