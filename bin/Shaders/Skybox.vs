#version 330 core

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
//uniform float atmosphereRadius;

out vec3 Position;
out vec3 TexCoords;

void main()
{
	gl_Position = vp * vec4( position, 1.0 );
	gl_Position = gl_Position.xyww;

	//Position = normalize( position ) * atmosphereRadius;
	TexCoords = position;
}