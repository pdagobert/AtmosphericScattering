#ifndef __FRAME_BUFFER_HPP__
#define __FRAME_BUFFER_HPP__

#include <GL/glew.h>

class Texture2D;
class CubeMap;
class RenderBuffer;

enum class Attachment
{
	Color0 = GL_COLOR_ATTACHMENT0,
	Color1 = GL_COLOR_ATTACHMENT1,
	Depth = GL_DEPTH_ATTACHMENT
};

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	FrameBuffer( FrameBuffer&& frameBuffer );
	FrameBuffer& operator=( FrameBuffer&& frameBuffer );

	bool isValid() const;

	void attach( Attachment attachment, const Texture2D& texture );
	void attach( Attachment attachment, const CubeMap& cubemap, int face );
	void attach( Attachment attachment, const RenderBuffer& renderBuffer );

	void bind() const;
	void unbind() const;

private:
	GLuint m_handle;
};
#endif