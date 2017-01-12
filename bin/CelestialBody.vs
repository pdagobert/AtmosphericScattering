#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;
layout( location = 2 ) in vec3 normal;

uniform mat4 vp;
uniform mat4 world;
uniform float far;

uniform vec3 worldPos;
uniform float radius;

out vec3 Position;
out vec2 TexCoords;
out vec3 Normal;

void main()
{
	gl_Position = vp * world * vec4( position, 1.0 );

	float invFarPlane = 2.0 / ( log2( far + 1 ) );
	gl_Position.z = log2( max( 1e-6, 1.0 + gl_Position.w ) ) * invFarPlane - 1.0;
	gl_Position.z *= gl_Position.w;

	Position = position * radius + worldPos;
	TexCoords = texCoords;
	Normal = position;
}