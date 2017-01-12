#ifndef __TEXTURE_ATLAS_HPP__
#define __TEXTURE_ATLAS_HPP__

#include "OpenGL/Texture.hpp"
#include <vector>
#include <glm/glm.hpp>

class TextureAtlas
{
public:
	TextureAtlas( int regionSize, int numRegions, InternalFormat internalFormat );

	glm::ivec2 getNextAvailableRegion();
	void releaseRegion( const glm::ivec2& region );

	void bind( int unit ) const;

	template< class T >
	void update( const glm::ivec2& region, const std::vector< T >& data, PixelFormat format, PixelType type )
	{
		glm::ivec2 position = region * m_regionSize;
		m_texture.update( &data[ 0 ], position.x, position.y, m_regionSize, m_regionSize, format, type );
	}

	int getRegionSize() const;

private:
	void initAvailableRegions( int numRegions );

private:
	int m_regionSize;

	Texture2D m_texture;
	std::vector< glm::ivec2 > m_availableRegions;
};
#endif