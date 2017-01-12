#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include <GL/glew.h>
#include <vector>

class FrameBuffer;

enum class InternalFormat
{
	Alpha = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	AlphaFloat = GL_R32F,
	RGBFloat = GL_RGB32F,
	RGBAFloat = GL_RGBA32F,
	Depth = GL_DEPTH_COMPONENT,
	SRGB = GL_SRGB,
	SRGBA = GL_SRGB_ALPHA
};

enum class PixelFormat
{
	Alpha = GL_RED,
	RGB = GL_RGB,
	RGBA = GL_RGBA,
	Depth = GL_DEPTH_COMPONENT
};

enum class PixelType
{
	UByte = GL_UNSIGNED_BYTE,
	Float = GL_FLOAT
};

enum class TextureWrap
{
	Clamp = GL_CLAMP_TO_EDGE,
	Repeat = GL_REPEAT,
	Mirrored = GL_MIRRORED_REPEAT,
	ClampToBorder = GL_CLAMP_TO_BORDER
};

enum class TextureFilter
{
	Nearest = GL_NEAREST,
	Linear = GL_LINEAR,
	LinearMipMapLinear = GL_LINEAR_MIPMAP_LINEAR
};

struct Texture2DParameters
{
	TextureFilter min, mag;
	TextureWrap s, t;
	bool mipmaps = false;
	float anisotropy = 0.0f;
};

class Texture2D
{
public:
	Texture2D();
	Texture2D( int width, int height, InternalFormat format, const Texture2DParameters& params );
	Texture2D( int width, int height, const void* pixels, InternalFormat internalFormat, PixelFormat format, PixelType pixelType, const Texture2DParameters& params );
	~Texture2D();

	Texture2D( Texture2D&& texture );
	Texture2D& operator=( Texture2D&& texture );

	void update( const void* pixels, int x, int y, int width, int height, PixelFormat format, PixelType type );

	void setWrap( TextureWrap wrapS, TextureWrap wrapT );
	void setFilter( TextureFilter minFilter, TextureFilter magFilter );
	void setBorder( float value );

	void generateMipMaps();
	void setAnisotropy( float value );

	void setParameters( const Texture2DParameters& params );

	void bind( int unit ) const;
	void unbind() const;

	int getChannelsNum( PixelFormat type ) const;

	std::vector< unsigned char > getPixels( PixelFormat type ) const;
	std::vector< float > getFloatPixels( PixelFormat type ) const;

	int getWidth() const;
	int getHeight() const;

	friend class FrameBuffer;

private:
	GLuint m_handle;

	int m_width, m_height;
};

struct CubeMapParameters
{
	TextureFilter min, mag;
	TextureWrap s, t, r;
	bool mipmaps = false;
	float anisotropy = 0.0f;
};

class CubeMap
{
public:
	CubeMap();
	CubeMap( int size, const std::vector< const void* >& pixels, InternalFormat internalFormat, PixelFormat format, PixelType pixelType );
	~CubeMap();

	CubeMap( CubeMap&& cubeMap );
	CubeMap& operator=( CubeMap&& cubeMap );

	void setWrap( TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR );
	void setFilter( TextureFilter minFilter, TextureFilter magFilter );

	void generateMipMaps();
	void setAnisotropy( float value );

	void setParameters( const CubeMapParameters& params );

	void bind( int unit = 0 ) const;

	friend class FrameBuffer;

private:
	GLuint m_handle;
};
#endif