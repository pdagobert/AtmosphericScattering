#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform mat4 world;

void main()
{
	gl_Position = vp * world * vec4( position, 1.0 );
}