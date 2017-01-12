#ifndef __INTERSECTION_HPP__
#define __INTERSECTION_HPP__

#include "AABB.hpp"
#include "Sphere.hpp"

namespace Intersection
{
	bool intersectAABBSphere( const AABB& aabb, const Sphere& sphere );
}
#endif