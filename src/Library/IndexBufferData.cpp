#include "IndexBufferData.hpp"

IndexBufferData::IndexBufferData( int numIndices ) :
	m_indices( numIndices )
{

}

unsigned int& IndexBufferData::operator[]( int index )
{
	return m_indices[ index ];
}

const unsigned int& IndexBufferData::operator[]( int index ) const
{
	return m_indices[ index ];
}

unsigned int IndexBufferData::getNumIndices() const
{
	return m_indices.size();
}

const std::vector< unsigned int >& IndexBufferData::getData() const
{
	return m_indices;
}