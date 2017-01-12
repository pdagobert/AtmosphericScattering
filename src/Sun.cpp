#include "Sun.hpp"
#include "Geometry.hpp"
#include "Loader.hpp"
#include "RenderTarget.hpp"
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Skybox.hpp"

Sun::Sun( double diameter, double distance ) :
	m_radius( diameter / 2.0 ),
	m_distance( distance ),
	m_shader( Loader::loadShader( "Shaders/Billboard.vs", "Shaders/Sun.fs" ) )
{
	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );
	layout.add( VertexAttribute::TexCoords, 2 );

	m_quad = Mesh( Geometry::createUnitQuad( layout ), Primitive::Triangles );
	
	Texture2DParameters params;
	params.min = params.mag = TextureFilter::Linear;
	params.s = params.t = TextureWrap::Clamp;

	m_texture = Loader::loadTexture2D( "Textures/star_glow.png", params );
}

void Sun::render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const Skybox& skybox ) const
{
	m_shader.bind();
	m_shader.setUniform( "vp", camera.getMatrix() );
	m_shader.setUniform( "view", camera.getViewMatrix() );
	m_shader.setUniform( "far", 1e+14f );
	m_shader.setUniform( "intensity", 0 );
	m_shader.setUniform( "skybox", 1 );
	m_shader.setUniform( "cameraPos", camera.getPosition() );
	m_shader.setUniform( "lightDirU", lightDir );
	m_shader.setUniform( "toneMapping", toneMapping );
	m_shader.setUniform( "gamma", gamma );

	m_shader.setUniform( "center", lightDir * (float)m_distance );
	m_shader.setUniform( "radius", (float)m_radius );

	m_texture.bind( 0 );
	skybox.bind( 1 );

	m_quad.render( target );
}