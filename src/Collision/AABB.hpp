#ifndef __AABB_HPP__
#define __AABB_HPP__

#include <glm/glm.hpp>

struct AABB
{
	AABB() = default;
	AABB( const glm::dvec3& min, const glm::dvec3& max );

	glm::dvec3 min, max;
};
#endif