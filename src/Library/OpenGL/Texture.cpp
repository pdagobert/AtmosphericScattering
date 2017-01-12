#include "Texture.hpp"
#include <stdexcept>

Texture2D::Texture2D() :
	m_handle( 0 ),
	m_width( 0 ),
	m_height( 0 )
{

}

Texture2D::Texture2D( int width, int height, InternalFormat format, const Texture2DParameters& params ) :
	m_width( width ),
	m_height( height )
{
	glGenTextures( 1, &m_handle );

	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexImage2D( GL_TEXTURE_2D, 0, static_cast< GLint >( format ), width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr );
	glBindTexture( GL_TEXTURE_2D, 0 );

	setParameters( params );
}

Texture2D::Texture2D( int width, int height, const void* pixels, InternalFormat internalFormat, PixelFormat format, PixelType pixelType, const Texture2DParameters& params ) :
	m_width( width ),
	m_height( height )
{
	glGenTextures( 1, &m_handle );

	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexImage2D( GL_TEXTURE_2D, 0, static_cast< GLint >( internalFormat ), width, height, 0, static_cast< GLenum >( format ), static_cast< GLenum >( pixelType ), pixels );
	glBindTexture( GL_TEXTURE_2D, 0 );

	setParameters( params );
}

Texture2D::~Texture2D()
{
	if( m_handle != 0 )
		glDeleteTextures( 1, &m_handle );
}

Texture2D::Texture2D( Texture2D&& texture ) :
	m_handle( texture.m_handle ),
	m_width( texture.m_width ),
	m_height( texture.m_height )
{
	texture.m_handle = 0;
}

Texture2D& Texture2D::operator=( Texture2D&& texture )
{
	m_handle = texture.m_handle;
	m_width = texture.m_width;
	m_height = texture.m_height;

	texture.m_handle = 0;
	return *this;
}

void Texture2D::update( const void* pixels, int x, int y, int width, int height, PixelFormat format, PixelType type )
{
	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexSubImage2D( GL_TEXTURE_2D, 0, x, y, width, height, static_cast< GLenum >( format ), static_cast< GLenum >( type ), pixels );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::setWrap( TextureWrap wrapS, TextureWrap wrapT )
{
	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast< GLint >( wrapS ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast< GLint >( wrapT ) );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::setFilter( TextureFilter minFilter, TextureFilter magFilter )
{
	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast< GLint >( minFilter ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast< GLint >( magFilter ) );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::setBorder( float value )
{
	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, value );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::generateMipMaps()
{
	glBindTexture( GL_TEXTURE_2D, m_handle );
	glGenerateMipmap( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::setAnisotropy( float value )
{
	glBindTexture( GL_TEXTURE_2D, m_handle );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, value );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void Texture2D::setParameters( const Texture2DParameters& params )
{
	setFilter( params.min, params.mag );
	setWrap( params.s, params.t );

	if( params.mipmaps )
		generateMipMaps();

	if( params.anisotropy > 0.0f )
		setAnisotropy( params.anisotropy );
}

void Texture2D::bind( int unit ) const
{
	glActiveTexture( GL_TEXTURE0 + unit );
	glBindTexture( GL_TEXTURE_2D, m_handle );
}

void Texture2D::unbind() const
{
	glBindTexture( GL_TEXTURE_2D, 0 );
}

int Texture2D::getChannelsNum( PixelFormat type ) const
{
	switch( type )
	{
		case PixelFormat::Alpha:
			return 1;

		case PixelFormat::RGB:
			return 3;

		case PixelFormat::RGBA:
			return 4;

		case PixelFormat::Depth:
			return 1;
		
		default:
			throw std::runtime_error( "Texture2D::getChannelsNum() : invalid parameter" );
	}
}

std::vector< unsigned char > Texture2D::getPixels( PixelFormat type ) const
{
	std::vector< unsigned char > pixels( m_width * m_height * getChannelsNum( type ), 0 );

	bind( 0 );
	glGetTexImage( GL_TEXTURE_2D, 0, static_cast< GLenum >( type ), GL_UNSIGNED_BYTE, &pixels[ 0 ] );
	unbind();

	return pixels;
}

std::vector< float > Texture2D::getFloatPixels( PixelFormat type ) const
{
	std::vector< float > pixels( m_width * m_height * getChannelsNum( type ), 0 );

	bind( 0 );
	glGetTexImage( GL_TEXTURE_2D, 0, static_cast< GLenum >( type ), GL_FLOAT, &pixels[ 0 ] );
	unbind();

	return pixels;
}

int Texture2D::getWidth() const
{
	return m_width;
}

int Texture2D::getHeight() const
{
	return m_height;
}

CubeMap::CubeMap()
{

}

CubeMap::CubeMap( int size, const std::vector< const void* >& pixels, InternalFormat internalFormat, PixelFormat format, PixelType pixelType )
{
	glGenTextures( 1, &m_handle );

	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle );

	for( int i = 0; i < 6; i++ )
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, static_cast< GLint >( internalFormat ), size, size, 0, static_cast< GLenum >( format ), static_cast< GLenum >( pixelType ), pixels[ i ] );
	
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

CubeMap::~CubeMap()
{
	if( m_handle != 0 )
		glDeleteTextures( 1, &m_handle );
}

CubeMap::CubeMap( CubeMap&& cubeMap ) :
	m_handle( cubeMap.m_handle )
{
	cubeMap.m_handle = 0;
}

CubeMap& CubeMap::operator=( CubeMap&& cubeMap )
{
	m_handle = cubeMap.m_handle;

	cubeMap.m_handle = 0;
	return *this;
}

void CubeMap::setWrap( TextureWrap wrapS, TextureWrap wrapT, TextureWrap wrapR )
{
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast< GLint >( wrapS ) );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, static_cast< GLint >( wrapT ) );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, static_cast< GLint >( wrapR ) );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void CubeMap::setFilter( TextureFilter minFilter, TextureFilter magFilter )
{
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast< GLint >( minFilter ) );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast< GLint >( magFilter ) );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void CubeMap::CubeMap::generateMipMaps()
{
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle );
	glGenerateMipmap( GL_TEXTURE_CUBE_MAP );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void CubeMap::setAnisotropy( float value )
{
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle );
	glTexParameterf( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, value );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
}

void CubeMap::setParameters( const CubeMapParameters& params )
{
	setFilter( params.min, params.mag );
	setWrap( params.s, params.t, params.r );

	if( params.mipmaps )
		generateMipMaps();

	if( params.anisotropy > 0.0f )
		setAnisotropy( params.anisotropy );
}

void CubeMap::bind( int unit ) const
{
	glActiveTexture( GL_TEXTURE0 + unit );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_handle );
}