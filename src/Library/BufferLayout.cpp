#include "BufferLayout.hpp"
#include <stdexcept>
#include <string>

BufferLayout::BufferLayout() :
	m_flag( 0 ),
	m_size( 0 )
{

}

void BufferLayout::add( int id, int size, int divisor )
{
	auto it = m_attributes.find( id );
	if( it != m_attributes.end() )
		throw std::runtime_error( "Attribute already exists :" + std::to_string( id ) );
	
	m_attributes[ id ] = { size, m_size, divisor };

	m_flag |= id;
	m_size += size;
}

void BufferLayout::forEachAttributes( std::function< void( int, int, int ) > func ) const
{
	for( auto attribute : m_attributes )
	{
		const Attribute& attrib = attribute.second;
		func( attrib.size, attrib.offset, attrib.divisor );
	}
}

bool BufferLayout::has( int id ) const
{
	return m_flag & id;
}

int BufferLayout::getVertexSize() const
{
	return m_size;
}

int BufferLayout::getAttributeSize( int id ) const
{
	auto it = m_attributes.find( id );
	if( it == m_attributes.end() )
		throw std::runtime_error( "Attribute already not exist" + std::to_string( id ) );
	
	return it->second.size;
}

int BufferLayout::getAttributeOffset( int id ) const
{
	auto it = m_attributes.find( id );
	if( it == m_attributes.end() )
		throw std::runtime_error( "Attribute does not exist" + std::to_string( id ) );
	
	return it->second.offset;
}