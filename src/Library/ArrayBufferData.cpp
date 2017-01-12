#include "ArrayBufferData.hpp"
#include <GL/glew.h>

ArrayBufferData::ArrayBufferData( const BufferLayout& layout, int numVertices ) :
	m_layout( layout ),
	m_vertices( layout.getVertexSize() * numVertices, 0.0f )
{

}

int ArrayBufferData::bind( int start ) const
{
	int i = 0;

	m_layout.forEachAttributes(
		[ start, &i, this ]( int size, int offset, int divisor )
		{
			int index = start + i;

			glEnableVertexAttribArray( index );
			glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, m_layout.getVertexSize() * 4, reinterpret_cast< void* >( offset * 4 ) );

			if( divisor != 0 )
				glVertexAttribDivisor( index, divisor );
			
			i++;
		} );
	
	return i;
}

void ArrayBufferData::set( int vertexIndex, int attribute, const glm::vec4& value )
{
	int index = getAttributeIndex( vertexIndex, attribute );

	for( int i = 0; i < 4; i++ )
		m_vertices[ index + i ] = value[ i ];
}

void ArrayBufferData::set( int vertexIndex, int attribute, const glm::vec3& value )
{
	int index = getAttributeIndex( vertexIndex, attribute );

	for( int i = 0; i < 3; i++ )
		m_vertices[ index + i ] = value[ i ];
}

void ArrayBufferData::set( int vertexIndex, int attribute, const glm::vec2& value )
{
	int index = getAttributeIndex( vertexIndex, attribute );

	for( int i = 0; i < 2; i++ )
		m_vertices[ index + i ] = value[ i ];
}

void ArrayBufferData::set( int vertexIndex, int attribute, float value )
{
	int index = getAttributeIndex( vertexIndex, attribute );

	m_vertices[ index ] = value;
}

void ArrayBufferData::add( int vertexIndex, int attribute, const glm::vec3& value )
{
	int index = getAttributeIndex( vertexIndex, attribute );

	for( int i = 0; i < 3; i++ )
		m_vertices[ index + i ] += value[ i ];
}

glm::vec3 ArrayBufferData::getVec3( int vertexIndex, int attribute ) const
{
	if( m_layout.getAttributeSize( attribute ) != 3 )
		throw std::runtime_error( "getVec3 used on attribute with size != 3" );
	
	int index = getAttributeIndex( vertexIndex, attribute );

	return glm::vec3( m_vertices[ index ], m_vertices[ index + 1 ], m_vertices[ index + 3 ] );
}

int ArrayBufferData::getAttributeIndex( int vertexIndex, int attribute ) const
{
	return vertexIndex * m_layout.getVertexSize() + m_layout.getAttributeOffset( attribute ); 
}

// TODO: find revelant name to getNumVertices
unsigned int ArrayBufferData::getNumVertices() const
{
	return m_vertices.size() / m_layout.getVertexSize();
}

const std::vector< float >& ArrayBufferData::getData() const
{
	return m_vertices;
}