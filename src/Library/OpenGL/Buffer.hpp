#ifndef __BUFFER_HPP__
#define __BUFFER_HPP__

#include <GL/glew.h>
#include <vector>

enum class Usage
{
	Static = GL_STATIC_DRAW,
	Dynamic = GL_DYNAMIC_DRAW
};

class Buffer
{
public:
	Buffer( GLenum type );
	virtual ~Buffer();

	Buffer( Buffer&& other );
	Buffer& operator=( Buffer&& other );

	void bind() const;
	void unbind() const;

	template< typename T >
	void setData( const std::vector< T >& data, Usage usage );

private:
	GLuint m_handle;
	GLenum m_type;
};

template< typename T >
void Buffer::setData( const std::vector< T >& data, Usage usage )
{
	bind();
	glBufferData( m_type, sizeof( T ) * data.size(), &data[ 0 ], static_cast< GLenum >( usage ) );
	unbind();
}

class ArrayBuffer : public Buffer
{
public:
	ArrayBuffer();
};

class IndexBuffer : public Buffer
{
public:
	IndexBuffer();

	void bindToVertexArray();
};
#endif