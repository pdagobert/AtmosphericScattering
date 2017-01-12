#ifndef __GENERATION_FUNCTIONS_HPP__
#define __GENERATION_FUNCTIONS_HPP__

#include <functional>
#include <glm/glm.hpp>

double generateHeight( const glm::dvec3& position );
glm::vec3 generateNormal( int x, int y, const std::function< double( int, int ) >& heightmapFunc, double strength, double maxHeight );
#endif