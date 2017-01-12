#include "RenderBuffer.hpp"

RenderBuffer::RenderBuffer() :
	m_handle( 0 )
{

}

RenderBuffer::RenderBuffer( int width, int height, Storage storage )
{
	glGenRenderbuffers( 1, &m_handle );
	glBindRenderbuffer( GL_RENDERBUFFER, m_handle );
	glRenderbufferStorage( GL_RENDERBUFFER, static_cast< GLenum >( storage ), width, height );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}

RenderBuffer::~RenderBuffer()
{
	if( m_handle != 0 )
		glDeleteBuffers( 1, &m_handle );
}

RenderBuffer::RenderBuffer( RenderBuffer&& renderBuffer ) :
	m_handle( renderBuffer.m_handle )
{
	renderBuffer.m_handle = 0;
}

RenderBuffer& RenderBuffer::operator=( RenderBuffer&& renderBuffer )
{
	m_handle = renderBuffer.m_handle;
	renderBuffer.m_handle = 0;

	return *this;
}