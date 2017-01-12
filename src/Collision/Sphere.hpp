#ifndef __SPHERE_HPP__
#define __SPHERE_HPP__

#include <glm/glm.hpp>

struct Sphere
{
	Sphere( const glm::dvec3& center, double radius );

	glm::dvec3 center;
	double radius;

};
#endif