#version 330

layout( location = 0 ) in vec3 position;
layout( location = 1 ) in vec2 texCoords;

out vec2 TexCoords;

uniform mat4 wvp;

void main()
{
	gl_Position = wvp * vec4( position, 1.0 );
	TexCoords = texCoords;
}