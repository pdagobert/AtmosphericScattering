#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__

#include "Window.hpp"
#include "RenderTarget.hpp"
#include "OpenGL/Shader.hpp"
#include "Mesh.hpp"
#include "Camera.hpp"
#include "UI/TweakBar.hpp"
#include "Terrain.hpp"
#include "Planet.hpp"
#include "Atmosphere.hpp"
#include "CelestialBody.hpp"
#include "Skybox.hpp"
#include "Sun.hpp"

class Application : public Window
{
public:
	Application();

	virtual void processEvents( float deltaTime );
	virtual void render( float );

	virtual void mouseMoved( float x, float y );

private:
	void renderTerrainTexture();

private:
	RenderTarget m_target;
	Shader m_shader, m_debugShader, m_boxShader, m_flatTerrainShader, m_atlasShader1, m_textureShader, m_heightShader, m_planetShader;
	Mesh m_quad, m_sphere;
	Mesh m_box;

	Camera m_camera;

	TweakBar m_tweakBar;

	Terrain m_terrain;
	Planet m_planet;
	Atmosphere m_atmosphere;

	CelestialBody m_moon, m_jupiter;

	Skybox m_skybox;

	//Sun m_sun;

	glm::vec3 m_lightDir;
};
#endif