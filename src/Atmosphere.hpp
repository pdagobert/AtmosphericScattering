#ifndef __ATMOSPHERE_HPP__
#define __ATMOSPHERE_HPP__

#include "OpenGL/Shader.hpp"
#include "Mesh.hpp"

class RenderTarget;
class Camera;

class Atmosphere
{
public:
	Atmosphere( double planetRadius, double atmosphereHeight );

	void render( RenderTarget& target, const Camera& camera, const glm::vec3& lightDir, int toneMapping, int gamma ) const;

private:
	double m_radius;

	Shader m_shader;
	Mesh m_sphere;
};
#endif