#include "Intersection.hpp"
//#include "AABB.hpp"
//#include "Sphere.hpp"
#include <glm/glm.hpp>

namespace Intersection
{
	bool intersectAABBSphere( const AABB& aabb, const Sphere& sphere )
	{
		glm::dvec3 closest = glm::clamp( sphere.center, aabb.min, aabb.max );
		return glm::distance( closest, sphere.center ) < sphere.radius;
	}
}