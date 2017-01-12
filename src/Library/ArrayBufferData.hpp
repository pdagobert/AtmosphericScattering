#ifndef __ARRAY_BUFFER_DATA_HPP__
#define __ARRAY_BUFFER_DATA_HPP__

#include "BufferLayout.hpp"
#include <vector>
#include <glm/glm.hpp>

class ArrayBufferData
{
public:
	ArrayBufferData( const BufferLayout& layout, int numVertices );

	int bind( int start ) const;

	void set( int vertexIndex, int attribute, const glm::vec4& value );
	void set( int vertexIndex, int attribute, const glm::vec3& value );
	void set( int vertexIndex, int attribute, const glm::vec2& value );
	void set( int vertexIndex, int attribute, float value );

	void add( int vertexIndex, int attribute, const glm::vec3& value );

	glm::vec3 getVec3( int vertexIndex, int attribute ) const;

	unsigned int getNumVertices() const;
	const std::vector< float >& getData() const;

private:
	int getAttributeIndex( int vertexIndex, int attribute ) const;

private:
	BufferLayout m_layout;
	std::vector< float > m_vertices;
};
#endif