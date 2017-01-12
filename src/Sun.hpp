#ifndef __SUN_HPP__
#define __SUN_HPP__

#include "OpenGL/Shader.hpp"
#include "Mesh.hpp"
#include "OpenGL/Texture.hpp"
#include <glm/glm.hpp>

class RenderTarget;
class Camera;
class Skybox;

class Sun
{
public:
	Sun( double diameter, double distance );

	void render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const Skybox& skybox ) const;

private:
	double m_radius, m_distance;
	glm::dvec3 m_direction;

	Shader m_shader;
	Mesh m_quad;

	Texture2D m_texture;
};
#endif