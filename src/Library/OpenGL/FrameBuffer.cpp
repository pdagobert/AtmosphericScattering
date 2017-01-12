#include "FrameBuffer.hpp"
#include "Texture.hpp"
#include "RenderBuffer.hpp"

FrameBuffer::FrameBuffer()
{
	glGenFramebuffers( 1, &m_handle );
}

FrameBuffer::~FrameBuffer()
{
	if( m_handle != 0 )
		glDeleteFramebuffers( 1, &m_handle );
}

FrameBuffer::FrameBuffer( FrameBuffer&& frameBuffer ) :
	m_handle( frameBuffer.m_handle )
{
	frameBuffer.m_handle = 0;
}

FrameBuffer& FrameBuffer::operator=( FrameBuffer&& frameBuffer )
{
	m_handle = frameBuffer.m_handle;

	frameBuffer.m_handle = 0;
	return *this;
}

bool FrameBuffer::isValid() const
{
	bind();

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( status == GL_FRAMEBUFFER_COMPLETE )
		return true;

	unbind();
	return false;
}

void FrameBuffer::attach( Attachment attachment, const Texture2D& texture )
{
	bind();
	glFramebufferTexture2D( GL_FRAMEBUFFER, static_cast< GLenum >( attachment ), GL_TEXTURE_2D, texture.m_handle, 0 );
	unbind();
}

void FrameBuffer::attach( Attachment attachment, const CubeMap& cubemap, int face )
{
	bind();
	glFramebufferTexture2D( GL_FRAMEBUFFER, static_cast< GLenum >( attachment ), GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, cubemap.m_handle, 0 );
	unbind();
}

void FrameBuffer::attach( Attachment attachment, const RenderBuffer& renderBuffer )
{
	bind();
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, static_cast< GLenum >( attachment ), GL_RENDERBUFFER, renderBuffer.m_handle );
	unbind();
}

void FrameBuffer::bind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, m_handle );
}

void FrameBuffer::unbind() const
{
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
}
