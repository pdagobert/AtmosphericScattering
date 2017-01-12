#include "GenerationFunctions.hpp"
#include "Simplex.h"

double generateHeight( const glm::dvec3& position )
{
	double height = Simplex::ridgedMF( Simplex::ridgedMF( glm::vec3( position ), 0.0f, 32, 2.0f, 0.5f ) );
	return height;
}

glm::vec3 generateNormal( int x, int y, const std::function< double( int, int ) >& heightmapFunc, double strength, double maxHeight )
{
	double top = heightmapFunc( x, y + 1 ) / maxHeight;
	double left = heightmapFunc( x - 1, y ) / maxHeight;
	double right = heightmapFunc( x + 1, y ) / maxHeight;
	double bottom = heightmapFunc( x, y - 1 ) / maxHeight;

	double sx = right - left;
	double sz = bottom - top;
	
	return glm::normalize( glm::vec3( sx, strength, sz ) );
}