#version 330

layout( location = 0 ) in vec3 position;

uniform mat4 vp;
uniform mat4 relativeWorld;

uniform float far;

uniform vec2 regionPosition;
uniform float regionSize;

uniform float heightFactor;

uniform sampler2D heightmap;

out vec2 QuadSpacePosition;
out vec2 DTexCoords;
//flat out vec2 TexCoords;

//out vec3 Normal;

void main()
{
	vec2 texCoords = vec2( position.x, -position.z ) + 16.0;
	QuadSpacePosition = texCoords;
	texCoords = ( texCoords + 0.5 ) / 33.0;

	vec2 TexCoords = regionPosition + vec2( texCoords.x, texCoords.y ) * regionSize;

	vec4 pos = relativeWorld * vec4( position, 1.0 );
	pos.y += texture( heightmap, TexCoords ).r;

	gl_Position = vp * pos;
	float invFarPlane = 2.0 / ( log( far + 1 ) );
	gl_Position.z = ( log( 1 + gl_Position.w ) * invFarPlane - 1 ) * gl_Position.w;

	DTexCoords = TexCoords; 
	//Normal = texture( normalmap, TexCoords ).rgb;

	/*vec3 noisePos = vec3( world * vec4( position, 1.0 ) );

	vec4 pos = relativeWorld * vec4( position, 1.0 );
	pos.y += getHeight( noisePos ); // relative to camera so += instead of =

	gl_Position = vp * pos;

	float invFarPlane = 2.0 / ( log( far + 1 ) );
	gl_Position.z = ( log( 1 + gl_Position.w ) * invFarPlane - 1 ) * gl_Position.w;

	Normal = getNormal();*/
}