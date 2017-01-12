#include "Atmosphere.hpp"
#include "RenderTarget.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "Loader.hpp"
#include <glm/gtc/matrix_transform.hpp>

Atmosphere::Atmosphere( double planetRadius, double atmosphereHeight ) :
	m_radius( planetRadius + atmosphereHeight ),
	m_shader( Loader::loadShader( "Shaders/Atmosphere.vs", "Shaders/Atmosphere.fs" ) )
{
	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );

	m_sphere = Mesh( Geometry::createUVSphere( 1.0f, 100, layout ), Primitive::Triangles );
}

void Atmosphere::render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma ) const
{
	/*glDisable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_FRONT_FACE );

	m_shader.bind();
	m_shader.setUniform( "vp", camera.getMatrix() );

	glm::mat4 relativeWorld = glm::translate( glm::mat4(), glm::vec3( glm::dvec3( 0.0 ) - camera.getPosition() ) );
	relativeWorld = glm::scale( relativeWorld, glm::vec3( m_radius ) );

	//glm::mat4 world = glm::translate( glm::mat4(), glm::vec3( 0.0f ) );
	//world = glm::scale( world, glm::vec3( m_radius ) );

	m_shader.setUniform( "world", relativeWorld );
	m_shader.setUniform( "far", 1e+14f );
	m_shader.setUniform( "lightDirU", lightDir );
	m_shader.setUniform( "cameraPos", glm::vec3( camera.getPosition() ) );
	m_shader.setUniform( "atmosphereRadius", (float)m_radius );
	m_shader.setUniform( "toneMapping", toneMapping );
	m_shader.setUniform( "gamma", gamma );

	m_sphere.render( target );*/

	glDisable( GL_CULL_FACE );
	glEnable( GL_DEPTH_TEST );
}