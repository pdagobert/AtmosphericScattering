#ifndef __RENDER_BUFFER_HPP__
#define __RENDER_BUFFER_HPP__

#include <GL/glew.h>

enum class Storage
{
	Depth24 = GL_DEPTH_COMPONENT24,
	Depth32 = GL_DEPTH_COMPONENT32F
};

class FrameBuffer;

class RenderBuffer
{
public:
	RenderBuffer();
	RenderBuffer( int width, int height, Storage storage );
	~RenderBuffer();

	RenderBuffer( RenderBuffer&& renderBuffer );
	RenderBuffer& operator=( RenderBuffer&& renderBuffer );

	friend class FrameBuffer;
	
private:
	GLuint m_handle;
};
#endif