#version 330 core

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;

uniform mat4 wvp;

out vec2 TexCoords;

void main()
{
	gl_Position = wvp * vec4( position, 1.0 );
	TexCoords = texCoords * 33.0 / 32.0; 
}