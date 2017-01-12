#include "Skybox.hpp"
#include "RenderTarget.hpp"
#include "Camera.hpp"
#include "Loader.hpp"
#include "Geometry.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Skybox::Skybox( const std::vector< std::string >& fileNames, double atmosphereRadius, double sunDiameter, double sunDistance ) :
	m_atmosphereRadius( atmosphereRadius ),
	m_sunRadius( sunDiameter / 2.0 ),
	m_sunDistance( sunDistance ),
	m_target( Viewport( 2048, 2048 ) ),
	m_shader( Loader::loadShader( "Shaders/Skybox.vs", "Shaders/Skybox.fs" ) ),
	m_prepassShader( Loader::loadShader( "Shaders/Prepass.vs", "Shaders/Prepass.fs" ) ),
	m_billboardShader( Loader::loadShader( "Shaders/Billboard.vs", "Shaders/Sun.fs" ) )
{
	CubeMapParameters params;
	params.min = params.mag = TextureFilter::Linear;
	params.s = params.t = params.r = TextureWrap::Clamp;

	m_texture = Loader::loadCubeMap( fileNames, params );

	// TODO: fix m_texture.getSize() and empty cubemap
	std::vector< const void* > pixels( 6, nullptr );
	m_skyboxCubeMap = CubeMap( 2048, pixels, InternalFormat::RGBAFloat, PixelFormat::RGBA, PixelType::Float );
	m_skyboxCubeMap.setParameters( params );

	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );

	//m_cube = Mesh( Geometry::createUVSphere( 1.0f, 500, layout ), Primitive::Triangles );
	m_cube = Mesh( Geometry::createBox( glm::vec3( 1.0f ), layout ), Primitive::Triangles );

	layout.add( VertexAttribute::TexCoords, 2 );
	m_quad = Mesh( Geometry::createUnitQuad( layout ), Primitive::Triangles );

	Texture2DParameters params2;
	params2.min = params2.mag = TextureFilter::Linear;
	params2.s = params2.t = TextureWrap::Clamp;

	m_starGlow = Loader::loadTexture2D( "Textures/star_glow.png", params2 );
}

void Skybox::firstPass( const Camera& camera, const glm::vec3& lightDir )
{
	glm::mat4 proj = glm::perspective( glm::radians( 90.0f ), 1.0f, 0.1f, 1000.0f );
	
	glm::vec3 cameraDirs[] = 
	{
		glm::vec3( 1.0f, 0.0f, 0.0f ),
		glm::vec3( -1.0f, 0.0f, 0.0f ),
		glm::vec3( 0.0f, 1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f )
	};

	glm::vec3 cameraUps[] =
	{
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, 0.0f, 1.0f ),
		glm::vec3( 0.0f, 0.0f, -1.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f ),
		glm::vec3( 0.0f, -1.0f, 0.0f )
	};

	for( int i = 0; i < 6; i++ )
	{
		m_target.attach( Attachment::Color0, m_skyboxCubeMap, i );
		m_target.prepare();

		glm::mat4 view = glm::lookAt( glm::vec3( 0.0 ), cameraDirs[ i ], cameraUps[ i ] );
		glm::mat4 vp = proj * view;

		glEnable( GL_DEPTH_TEST );
		glDepthFunc( GL_LEQUAL );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

		m_prepassShader.bind();
		m_prepassShader.setUniform( "vp", vp );

		m_prepassShader.setUniform( "skybox", 0 );

		m_texture.bind( 0 );

		m_cube.render( m_target );

		glEnable( GL_BLEND );
		glBlendFunc( GL_ONE, GL_ONE );

		m_billboardShader.bind();
		m_billboardShader.setUniform( "vp", vp );
		m_billboardShader.setUniform( "view", view );
		m_billboardShader.setUniform( "far", 1e+14f );
		m_billboardShader.setUniform( "starGlow", 0 );

		//m_billboardShader.setUniform( "lightDirU", lightDir );
		m_billboardShader.setUniform( "center", lightDir * (float)m_sunDistance );
		m_billboardShader.setUniform( "radius", (float)m_sunRadius * 0.6f );

		m_starGlow.bind( 0 );

		m_quad.render( m_target );

		glDisable( GL_BLEND );
		glDepthFunc( GL_LESS );
		glDisable( GL_DEPTH );
	}
}

void Skybox::render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const glm::vec3& betaR, float betaM ) const
{
	m_shader.bind();
	m_shader.setUniform( "skybox", 0 );
	m_shader.setUniform( "vp", camera.getProjectionMatrix() * camera.getViewMatrix() );

	m_shader.setUniform( "cameraPos", camera.getPosition() );
	//m_shader.setUniform( "far", 1e+14f );
	m_shader.setUniform( "lightDirU", lightDir );
	m_shader.setUniform( "cameraPos", glm::vec3( camera.getPosition() ) );
	//m_shader.setUniform( "atmosphereRadius", (float)m_atmosphereRadius );
	m_shader.setUniform( "toneMapping", toneMapping );
	m_shader.setUniform( "gamma", gamma );

	m_shader.setUniform( "rayleighExtinctionAtSeaLevel", betaR );
	m_shader.setUniform( "mieExtinctionAtSeaLevel", glm::vec3( betaM ) );

	m_skyboxCubeMap.bind( 0 );

	m_cube.render( target );
}

void Skybox::bind( int unit ) const
{
	m_texture.bind( unit );
}