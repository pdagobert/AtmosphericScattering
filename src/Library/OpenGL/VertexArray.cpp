#include "VertexArray.hpp"

VertexArray::VertexArray()
{
	glGenVertexArrays( 1, &m_handle );
}

VertexArray::~VertexArray()
{
	if( m_handle != 0 )
		glDeleteVertexArrays( 1, &m_handle );
}

VertexArray::VertexArray( VertexArray&& vertexArray ) :
	m_handle( vertexArray.m_handle )
{
	vertexArray.m_handle = 0;
}

VertexArray& VertexArray::operator=( VertexArray&& vertexArray )
{
	m_handle = vertexArray.m_handle;

	vertexArray.m_handle = 0;
	return *this;
}

void VertexArray::bind() const
{
	glBindVertexArray( m_handle );
}

void VertexArray::unbind() const
{
	glBindVertexArray( 0 );
}