#include "Buffer.hpp"

Buffer::Buffer( GLenum type ) :
	m_type( type )
{
	glGenBuffers( 1, &m_handle );
}

Buffer::~Buffer()
{
	if( m_handle != 0 )
		glDeleteBuffers( 1, &m_handle );
}

Buffer::Buffer( Buffer&& other ) :
	m_handle( other.m_handle ),
	m_type( other.m_type )
{
	other.m_handle = 0;
}

Buffer& Buffer::operator=( Buffer&& other )
{
	m_handle = other.m_handle;
	m_type = other.m_type;
	
	other.m_handle = 0;
	return *this;
}

void Buffer::bind() const
{
	glBindBuffer( m_type, m_handle );
}

void Buffer::unbind() const
{
	glBindBuffer( m_type, 0 );
}

ArrayBuffer::ArrayBuffer() :
	Buffer( GL_ARRAY_BUFFER )
{

}

/*void ArrayBuffer::bindToVertexArray( int index, int size, int stride, int offset, int divisor )
{
	glEnableVertexAttribArray( index );
	glVertexAttribPointer( index, size, GL_FLOAT, GL_FALSE, stride, offset );

	if( divisor != 0 )
		glVertexAttribDivisor( index, divisor );
}*/

IndexBuffer::IndexBuffer() :
	Buffer( GL_ELEMENT_ARRAY_BUFFER )
{
	
}