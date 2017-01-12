#ifndef __RENDER_TARGET_HPP__
#define __RENDER_TARGET_HPP__

#include "OpenGL/FrameBuffer.hpp"
#include "OpenGL/Texture.hpp"
#include "OpenGL/RenderBuffer.hpp"
#include <glm/glm.hpp>
#include <map>
#include <memory>

class RenderCommand;

struct Viewport
{
	Viewport() = default;
	
	Viewport( int width, int height ) :
		size( width, height )
	{

	}

	glm::ivec2 size, position;
};

enum Primitive
{
	Triangles = GL_TRIANGLES,
	Points = GL_POINTS
};

struct RenderCommand
{
	Primitive primitive;
	unsigned int numIndices, instanceCount;

	unsigned int offset;
};

class RenderTarget
{
public:
	RenderTarget( const Viewport& viewport );

	RenderTarget( RenderTarget&& ) = default;
	RenderTarget& operator=( RenderTarget&& ) = default;

	static RenderTarget defaultRenderTarget( const Viewport& viewport );

	bool isValid() const;

	void prepare();
	void draw( const RenderCommand& command );

	void attach( Attachment attachment, Texture2D&& texture );
	void attach( Attachment attachment, RenderBuffer&& renderBuffer );
	void attach( Attachment attachment, const CubeMap& cubemap, int face );

	const Texture2D& getTexture( Attachment attachment ) const;
	Texture2D& getTextureRef( Attachment attachment );

private:
	RenderTarget();

private:
	Viewport m_viewport;
	std::unique_ptr< FrameBuffer > m_frameBuffer;

	std::map< Attachment, Texture2D > m_textures;
	std::map< Attachment, RenderBuffer > m_renderBuffers;
};
#endif