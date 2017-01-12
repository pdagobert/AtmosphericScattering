#version 330

uniform sampler2D tex;

uniform int type;

in vec2 TexCoords;

out vec4 Color;

void main()
{
	vec3 color = vec3( 0.0 );

	if( type == 0 )
	{
		float height = texture( tex, TexCoords ).r;
		color = vec3( height );
	}
	else if( type == 1 )
	{
		color = texture( tex, TexCoords ).rgb;
	}
	else if( type == 2 )
	{
		color = texture( tex, TexCoords ).rgb;
	}
	
	Color = vec4( color, 1.0 );
}