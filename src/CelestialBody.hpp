#ifndef __CELESTIAL_BODY
#define __CELESTIAL_BODY

#include "OpenGL/Shader.hpp"
#include "Mesh.hpp"
#include "OpenGL/Texture.hpp"
#include <glm/glm.hpp>

class RenderTarget;
class Camera;

class CelestialBody
{
public:
	CelestialBody( double diameter, double distance, const glm::dvec3& direction, const std::string& fragmentFileName, const std::string& textureName );

	void render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma, const glm::vec3& betaR, float betaM ) const;

private:
	double m_radius;

	double m_distance;
	glm::dvec3 m_direction;

	Shader m_shader;
	Mesh m_sphere;

	Texture2D m_texture;
};
#endif