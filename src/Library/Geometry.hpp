#ifndef __GEOMETRY_HPP__
#define __GEOMETRY_HPP__

#include "ArrayBufferData.hpp"
#include "IndexBufferData.hpp"
#include <glm/glm.hpp>
#include <functional>

class BufferLayout;

struct GeometryData
{
	ArrayBufferData vertices;
	IndexBufferData indices;
};

namespace Geometry
{
	GeometryData createUVSphere( float radius, int precision, const BufferLayout& layout );

	GeometryData createBox( const glm::vec3& midSize, const BufferLayout& layout );
	GeometryData createSubdivisedQuad( int width, int height, float size, const BufferLayout& layout, bool centered = true,
		std::function< float( int, int ) > heightFunc = {} );

	GeometryData createQuad( const glm::vec2& size, const BufferLayout& layout, const glm::vec2& offset = glm::vec2( 0.0f ) );
	GeometryData createUnitQuad( const BufferLayout& layout );

	GeometryData createTerrainPatch( int width, int height, float size, const BufferLayout& layout );

	void computeSmoothNormals( ArrayBufferData& vertices, const IndexBufferData& indices );
}
#endif