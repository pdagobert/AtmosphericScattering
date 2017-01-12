#version 330 core

uniform samplerCube skybox;

in vec3 TexCoords;

out vec4 Color;

void main()
{
	vec3 color = pow( texture( skybox, TexCoords ).rgb, vec3( 2.2 ) );
	Color = vec4( color, 1.0 );
}