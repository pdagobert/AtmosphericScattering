#include "TextureAtlas.hpp"
#include <algorithm>
#include <cassert>

TextureAtlas::TextureAtlas( int regionSize, int numRegions, InternalFormat internalFormat ) :
	m_regionSize( regionSize )
{
	Texture2DParameters params;
	params.min = params.mag = TextureFilter::Nearest;
	params.s = params.t = TextureWrap::Clamp;

	int size = regionSize * numRegions;
	std::vector< float > init( size * size, 0.0f );
	m_texture = Texture2D( size, size, &init[ 0 ], internalFormat, PixelFormat::Alpha, PixelType::Float, params );

	initAvailableRegions( numRegions );
}

void TextureAtlas::initAvailableRegions( int numRegions )
{
	m_availableRegions.reserve( numRegions * numRegions );

	for( int y = 0; y < numRegions; y++ )
	{
		for( int x = 0; x < numRegions; x++ )
		{
			m_availableRegions.push_back( glm::ivec2( x, y ) );
		}
	}

	std::reverse( m_availableRegions.begin(), m_availableRegions.end() );
}

glm::ivec2 TextureAtlas::getNextAvailableRegion()
{
	assert( !m_availableRegions.empty() );

	glm::ivec2 region = m_availableRegions.back();
	m_availableRegions.pop_back();

	return region;
}

void TextureAtlas::releaseRegion( const glm::ivec2& region )
{
	m_availableRegions.push_back( region );
}

void TextureAtlas::bind( int unit ) const
{
	m_texture.bind( unit );
}

int TextureAtlas::getRegionSize() const
{
	return m_regionSize;
}