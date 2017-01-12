#include "Application.hpp"
#include "Loader.hpp"
#include "BufferLayout.hpp"
#include "Geometry.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <iomanip>

//TODO: load textures


//const int Width = 800;
//const int Height = 600;
const int Width = 1440;
const int Height = 900;

// TODO: check for gl ext (see glext.txt)

Application::Application() :
	Window( Width, Height, "Demo 2.0" ),
	m_target( RenderTarget::defaultRenderTarget( Viewport( Width, Height ) ) ),
	m_shader( Loader::loadShader( "Shader.vs", "Shader.fs" ) ),
	m_debugShader( Loader::loadShader( "Debug.vs", "Debug.fs" ) ),
	m_boxShader( Loader::loadShader( "Box.vs", "Box.fs" ) ),
	m_flatTerrainShader( Loader::loadShader( "FlatTerrain.vs", "FlatTerrain.fs" ) ),
	m_atlasShader1( Loader::loadShader( "Shaders/TerrainAtlas.vs", "Shaders/TerrainHeightMap.fs" ) ),
	m_textureShader( Loader::loadShader( "Shaders/Texture.vs", "Shaders/Texture.fs" ) ),
	m_heightShader( Loader::loadShader( "Shaders/TerrainAtlas.vs", "Shaders/TerrainHeightMap.fs" ) ),
	m_planetShader( Loader::loadShader( "Shaders/SphericalTerrain.vs", "Shaders/SphericalTerrain.fs" ) ),
	m_camera( Width, Height, glm::dvec3( 0.0, 1073741824.0 / 2.0 + 322024.0, 0.0 ), 1.0, 1.0f, 15000.0f ),
	m_tweakBar( *this ),
	m_terrain( 1073741824.0 ),
	m_planet( 1073741824.0 ),
	m_atmosphere( 1073741824.0 / 2.0, 8000000.0 ),
	m_moon( 347400000.0, 3844000000.0 / 4.0, glm::normalize( glm::dvec3( 0.0, 1.0, 1.0 ) ), "Moon.fs", "moon.jpg" ),
	m_jupiter( 13982200000.0, 83844000000.0 / 1.0, glm::normalize( glm::dvec3( 1.0, 0.7, -1.0 ) ), "Moon.fs", "jupiter.jpg" ),
	m_skybox( { "Skybox1/PositiveX.png", "Skybox1/NegativeX.png", "Skybox1/NegativeY.png", "Skybox1/PositiveY.png", "Skybox1/PositiveZ.png", "Skybox1/NegativeZ.png" },
		1073741824.0 / 2.0 + 8000000.0, 139140000000, 14960000000000 / 100.0 )/*,
	m_sun( 139140000000, 14960000000000 / 100.0 )*/
	//m_planet( 64.0 )
	//m_terrain( 1024.0 )
{
	//glfwSwapInterval( 0 );
	BufferLayout layout;
	layout.add( VertexAttribute::Position, 3 );
	layout.add( VertexAttribute::TexCoords, 2 );

	m_quad = Mesh( Geometry::createQuad( glm::vec2( 0.5f ), layout, glm::vec2( 0.5f ) ), Primitive::Triangles );
	m_sphere = Mesh( Geometry::createUVSphere( 1.0f, 10, layout ), Primitive::Triangles );

	m_box = Mesh( Geometry::createBox( glm::vec3( 1.0f ), layout ), Primitive::Triangles );
}

void Application::render( float deltaTime )
{
	m_tweakBar.newFrame();

	// change later
	m_tweakBar.update( 0, m_planet.getAltitude( m_camera ), m_camera.getSpeed() );

	static float angle = 0.0;
	angle = m_tweakBar.parameters.sunAngle;
	//angle += glm::radians( deltaTime * 25.0f );
	//angle = glm::radians( glm::mod( angle, 360.0f ) );

	m_lightDir = glm::vec3( 0.0f, 1.0f, 0.0f );
	m_lightDir.x = glm::cos( angle );
	m_lightDir.y = glm::sin( angle );

	m_lightDir = glm::normalize( m_lightDir );

	m_planet.update( m_camera );

	// render sun on top of skybox
	m_skybox.firstPass( m_camera, m_lightDir );

	m_target.prepare();

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	if( m_tweakBar.parameters.wireframe == 1 )
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	//m_atmosphere.render( m_target, m_camera, m_lightDir, m_tweakBar.parameters.applyToneMapping, m_tweakBar.parameters.applyGamma );
	
	m_planet.renderPatches( m_target, m_camera, m_lightDir, m_tweakBar.parameters.applyToneMapping, m_tweakBar.parameters.applyGamma, m_tweakBar.parameters.betaR, m_tweakBar.parameters.betaM );

	m_moon.render( m_target, m_camera, m_lightDir, m_tweakBar.parameters.applyToneMapping, m_tweakBar.parameters.applyGamma, m_tweakBar.parameters.betaR, m_tweakBar.parameters.betaM );
	m_jupiter.render( m_target, m_camera, m_lightDir, m_tweakBar.parameters.applyToneMapping, m_tweakBar.parameters.applyGamma, m_tweakBar.parameters.betaR, m_tweakBar.parameters.betaM );

	//m_sun.render( m_target, m_camera, m_lightDir, m_tweakBar.parameters.applyToneMapping, m_tweakBar.parameters.applyGamma, m_skybox );

	m_skybox.render( m_target, m_camera, m_lightDir, m_tweakBar.parameters.applyToneMapping, m_tweakBar.parameters.applyGamma, m_tweakBar.parameters.betaR, m_tweakBar.parameters.betaM );

	glDepthFunc( GL_LESS );
	glDisable( GL_DEPTH_TEST );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	renderTerrainTexture();
	
	m_tweakBar.render();
}

void Application::renderTerrainTexture()
{
	int textureType = m_tweakBar.parameters.currentDebugTexture;
	if( textureType == 0 )
		return;
	
	textureType--;

	if( textureType >= (int)PlanetTexture::Max )
		return;

	glm::mat4 wvp = glm::ortho( 0.0f, (float)Width, 0.0f, (float)Height );
	wvp = glm::scale( wvp, glm::vec3( 256.0f * m_tweakBar.parameters.debugTextureScale ) );
	
	m_textureShader.bind();
	m_textureShader.setUniform( "wvp", wvp );
	m_textureShader.setUniform( "tex", textureType );
	m_textureShader.setUniform( "type", textureType );

	m_planet.bindTexture( (PlanetTexture)textureType, 0 );
	
	m_quad.render( m_target );
}

// TODO: process key presses in camera controller class

void Application::processEvents( float deltaTime )
{
	if( isKeyPressed( GLFW_KEY_W ) )
		m_camera.moveForward( deltaTime );
	
	if( isKeyPressed( GLFW_KEY_S ) )
		m_camera.moveBackward( deltaTime );
	
	if( isKeyPressed( GLFW_KEY_D ) )
		m_camera.moveRight( deltaTime );
	
	if( isKeyPressed( GLFW_KEY_A ) )
		m_camera.moveLeft( deltaTime );
	
	if( isKeyPressed( GLFW_KEY_E ) )
		m_camera.moveUp( deltaTime );
	
	if( isKeyPressed( GLFW_KEY_R ) )
		m_camera.moveDown( deltaTime );
	
	if( isKeyPressed( GLFW_KEY_1 ) )
		m_camera.setSpeed( 10e-1 );
	
	if( isKeyPressed( GLFW_KEY_2 ) )
		m_camera.setSpeed( 10e0 );
	
	if( isKeyPressed( GLFW_KEY_3 ) )
		m_camera.setSpeed( 10e1 );
	
	if( isKeyPressed( GLFW_KEY_4 ) )
		m_camera.setSpeed( 10e2 );
	
	if( isKeyPressed( GLFW_KEY_5 ) )
		m_camera.setSpeed( 10e3 );
	
	if( isKeyPressed( GLFW_KEY_6 ) )
		m_camera.setSpeed( 10e4 );
	
	if( isKeyPressed( GLFW_KEY_7 ) )
		m_camera.setSpeed( 10e5 );
	
	if( isKeyPressed( GLFW_KEY_8 ) )
		m_camera.setSpeed( 10e6 );
	
	if( isKeyPressed( GLFW_KEY_9 ) )
		m_camera.setSpeed( 10e7 );
	
	if( isKeyPressed( GLFW_KEY_0 ) )
		m_camera.setSpeed( 10e8 );
}

void Application::mouseMoved( float x, float y )
{
	//if( m_tweakBar.isActive() )
	//	return;
	
	static glm::vec2 old( x, y );

	glm::vec2 cursor( x, y );

	if( isButtonPressed( GLFW_MOUSE_BUTTON_RIGHT ) )
		m_camera.orientate( cursor.x - old.x, old.y - cursor.y );
	
	old = cursor;
}