#ifndef __SKYBOX_HPP__
#define __SKYBOX_HPP__

#include "OpenGL/Texture.hpp"
#include "OpenGL/Shader.hpp"
#include "Mesh.hpp"
#include "RenderTarget.hpp"
#include <vector>
#include <string>

class RenderTarget;
class Camera;

class Skybox
{
public:
	Skybox( const std::vector< std::string >& fileNames, double atmosphereRadius, double sunDiameter, double sunDistance );

	void firstPass( const Camera& camera, const glm::vec3& lightDir );

	void render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const glm::vec3& betaR, float betaM ) const;

	void bind( int unit ) const;

private:
	double m_atmosphereRadius;
	double m_sunRadius, m_sunDistance;

	RenderTarget m_target;
	
	CubeMap m_texture, m_skyboxCubeMap;
	Texture2D m_starGlow;

	Shader m_shader, m_prepassShader, m_billboardShader;
	Mesh m_cube, m_quad;
};
#endif