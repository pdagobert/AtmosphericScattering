#include "CelestialBody.hpp"
#include "Geometry.hpp"
#include "Loader.hpp"
#include "RenderTarget.hpp"
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

CelestialBody::CelestialBody( double diameter, double distance, const glm::dvec3& direction, const std::string& fragmentFileName, const std::string& textureName ) :
	m_radius( diameter / 2.0 ),
	m_distance( distance ),
	m_direction( direction ),
	m_shader( Loader::loadShader( "CelestialBody.vs", fragmentFileName ) )
{
	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );
	layout.add( VertexAttribute::TexCoords, 2 );
	layout.add( VertexAttribute::Normal, 3 );

	m_sphere = Mesh( Geometry::createUVSphere( 1.0f, 100, layout ), Primitive::Triangles );

	Texture2DParameters params;
	params.min = TextureFilter::LinearMipMapLinear;
	params.mag = TextureFilter::Linear;
	params.s = params.t = TextureWrap::Repeat;
	params.mipmaps = true;

	m_texture = Loader::loadTexture2D( "Textures/"+ textureName, params );
}

void CelestialBody::render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const glm::vec3& betaR, float betaM ) const
{
	m_shader.bind();
	m_shader.setUniform( "diffuseMap", 0 );
	m_shader.setUniform( "vp", camera.getMatrix() );
	
	glm::dvec3 position = m_direction * m_distance;

	glm::mat4 world = glm::translate( glm::mat4(), glm::vec3( position - camera.getPosition() ) );
	world = glm::scale( world, glm::vec3( m_radius ) );

	m_shader.setUniform( "world", world );
	m_shader.setUniform( "far", 1e+14f );
	m_shader.setUniform( "cameraPos", glm::vec3( camera.getPosition() ) );
	m_shader.setUniform( "lightDirU", lightDir );
	m_shader.setUniform( "toneMapping", toneMapping );
	m_shader.setUniform( "gamma", gamma );
	m_shader.setUniform( "rayleighExtinctionAtSeaLevel", betaR );
	m_shader.setUniform( "mieExtinctionAtSeaLevel", glm::vec3( betaM ) );

	m_shader.setUniform( "worldPos", glm::vec3( position ) );
	m_shader.setUniform( "radius", (float)m_radius );

	m_texture.bind( 0 );

	m_sphere.render( target );
}